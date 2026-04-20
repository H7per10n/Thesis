/*
 * net_tui.c – Non-blocking industrial TUI via ANSI escape sequences
 *
 * IMPORTANT: DIM attribute is NOT used anywhere. It is invisible on many
 * dark-background terminals over SSH.  All text uses normal or BOLD weight.
 */

#define _DEFAULT_SOURCE
#include "net_tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

/* ── ANSI codes (NO DIM) ─────────────────────────────────────── */
#define ESC          "\033"
#define CSI          ESC "["
#define RST          CSI "0m"
#define BOLD         CSI "1m"
#define FG_RED       CSI "31m"
#define FG_GREEN     CSI "32m"
#define FG_YELLOW    CSI "33m"
#define FG_BLUE      CSI "34m"
#define FG_MAGENTA   CSI "35m"
#define FG_CYAN      CSI "36m"
#define FG_WHITE     CSI "37m"
#define FG_BLACK     CSI "30m"
#define BG_WHITE     CSI "47m"
#define HIDE_CUR     CSI "?25l"
#define SHOW_CUR     CSI "?25h"
#define EL           CSI "2K"
#define HOME         CSI "H"

/* ── render buffer ────────────────────────────────────────────── */
#define RBUF_SZ 8192

static const char *st_name(NETEnum s) {
    int idx = (int)s;
    if (idx >= 0 && idx < 16)
        return enum_names[idx];
    return "???";
}

static char rbuf[RBUF_SZ];
static int  rpos = 0;

static void rb_clear(void) { rpos = 0; }

static void rb_puts(const char *s) {
    while (*s && rpos < RBUF_SZ - 1) rbuf[rpos++] = *s++;
}

__attribute__((format(printf,1,2)))
static void rb_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int avail = RBUF_SZ - rpos;
    if (avail > 1) {
        int n = vsnprintf(rbuf + rpos, avail, fmt, ap);
        if (n > 0) rpos += (n < avail) ? n : avail - 1;
    }
    va_end(ap);
}

static void rb_flush(void) {
    if (rpos == 0) return;
    ssize_t r = write(STDOUT_FILENO, rbuf, rpos);
    (void)r;
    rpos = 0;
}

static void direct_write(const char *s) {
    ssize_t r = write(STDOUT_FILENO, s, strlen(s));
    (void)r;
}

/* ── cursor / line ────────────────────────────────────────────── */
static void rb_move(int r, int c) { rb_printf(CSI "%d;%dH", r, c); }
static void rb_el(void)           { rb_puts(EL); }
static void rb_line(int r)        { rb_move(r, 1); rb_el(); }

/* ── terminal size ────────────────────────────────────────────── */
static int tw(void) {
    struct winsize w;
    return (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) ? w.ws_col : 80;
}
static int th(void) {
    struct winsize w;
    return (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_row > 0) ? w.ws_row : 24;
}

/* ── raw mode ─────────────────────────────────────────────────── */
static struct termios orig_tio;
static int tio_saved = 0;
static int orig_out_fl = -1;

static void raw_on(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_tio);
    tio_saved = 1;
    raw = orig_tio;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    int fl = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, fl | O_NONBLOCK);
    orig_out_fl = fcntl(STDOUT_FILENO, F_GETFL, 0);
    fcntl(STDOUT_FILENO, F_SETFL, orig_out_fl | O_NONBLOCK);
}

static void raw_off(void) {
    if (orig_out_fl >= 0) fcntl(STDOUT_FILENO, F_SETFL, orig_out_fl);
    if (tio_saved) tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_tio);
}

/* ── event log ────────────────────────────────────────────────── */
#define LOG_LINES 12
#define LOG_WIDTH 120
static char evlog[LOG_LINES][LOG_WIDTH];
static int  log_hd  = 0;
static int  log_cnt = 0;

void tui_log_event(const char *msg) {
    snprintf(evlog[log_hd], LOG_WIDTH, "%s", msg);
    log_hd = (log_hd + 1) % LOG_LINES;
    if (log_cnt < LOG_LINES) log_cnt++;
}

/* ── cycle timing ─────────────────────────────────────────────── */
static uint64_t cycle_us = 0;
void tui_set_cycle_us(uint64_t us) { cycle_us = us; }

/* ── state colours — explicit case for EVERY enum value ───────── */
static const char *st_ansi(NETEnum s) {
    switch (s) {
        case _NET_BOOTING:      return FG_WHITE;
        case _NET_CAL_FAILED:   return BOLD FG_RED;
        case _NET_CALIBRATING:  return BOLD FG_MAGENTA;
        case _NET_ELECTRICAL:   return BOLD FG_RED;
        case _NET_EMERGENCY:    return BOLD FG_RED;
        case _NET_FAULT:        return BOLD FG_RED;
        case _NET_IDLE:         return FG_WHITE;
        case _NET_INIT:         return FG_CYAN;
        case _NET_NEEDS_RECAL:  return BOLD FG_YELLOW;
        case _NET_NO_POWER:     return BOLD FG_YELLOW;
        case _NET_NONE:         return FG_WHITE;
        case _NET_POWER_RESTORE:return BOLD FG_BLUE;
        case _NET_RECOVERING:   return BOLD FG_CYAN;
        case _NET_RUNNING:      return BOLD FG_GREEN;
        case _NET_SETUP_ERROR:  return BOLD FG_RED;
        case _NET_STOPPING:     return FG_WHITE;
        case _NET_THERMAL:      return BOLD FG_RED;
    }
    return BOLD FG_WHITE;
}

static const char *fr_str(NETEnum r) {
    switch (r) {
        case _NET_NONE:          return "---";
        case _NET_ELECTRICAL:    return "ELECTRICAL";
        case _NET_THERMAL:       return "THERMAL";
        case _NET_POWER_RESTORE: return "PWR_RESTORE";
        default:                 return "???";
    }
}

/* ── drawing helpers ──────────────────────────────────────────── */
static void draw_hr(int row, int w) {
    rb_line(row);
    rb_puts(FG_WHITE);
    for (int i = 0; i < w; i++) rb_puts("-");
    rb_puts(RST);
}

/* Print "label          <coloured>value         <reset>" */
static void draw_field(int row, int col, const char *label, const char *val, const char *ansi) {
    rb_move(row, col);
    rb_puts(RST);  /* ensure clean state before printing */
    rb_printf("%-22s", label);
    rb_puts(ansi);
    rb_printf("%-14s", val);
    rb_puts(RST);
}

static void draw_int_field(int row, int col, const char *label, int val, int limit) {
    char b[16]; snprintf(b, sizeof(b), "%d", val);
    draw_field(row, col, label, b, (val >= limit) ? BOLD FG_RED : FG_WHITE);
}

static void draw_node(int *row, int nd) {
    NETEnum ns, fr;
    int fc, pc, rc, cc, rbc, cv;
    if (nd == 0) {
        ns=Net_NODE1___;       fr=Net_NODE1___fault_reason_;
        fc=Net_NODE1___fault_cnt_;       pc=Net_NODE1___power_cc_cnt_;
        rc=Net_NODE1___recovery_att_cnt_; cc=Net_NODE1___calibration_att_cnt_;
        rbc=Net_NODE1___reboot_cnt_;     cv=Net_NODE1___cur_vel_;
    } else {
        ns=Net_NODE2___;       fr=Net_NODE2___fault_reason_;
        fc=Net_NODE2___fault_cnt_;       pc=Net_NODE2___power_cc_cnt_;
        rc=Net_NODE2___recovery_att_cnt_; cc=Net_NODE2___calibration_att_cnt_;
        rbc=Net_NODE2___reboot_cnt_;     cv=Net_NODE2___cur_vel_;
    }

    int r = *row;
    rb_line(r); rb_move(r, 2); rb_printf(BOLD "NODE %d" RST, nd + 1); r++;
    rb_line(r); draw_field(r, 3, "State:", st_name(ns), st_ansi(ns)); r++;
    rb_line(r); draw_field(r, 3, "Fault reason:", fr_str(fr), st_ansi(fr)); r++;

    char vb[16]; snprintf(vb, sizeof(vb), "%d", cv);
    rb_line(r); draw_field(r, 3, "Cur velocity:", vb, (cv > 0) ? BOLD FG_GREEN : FG_WHITE); r++;

    rb_line(r); rb_move(r, 3); rb_printf(FG_WHITE "Counters:" RST); r++;
    rb_line(r); draw_int_field(r, 4, "fault_cnt",           fc,  MAX_FAULTS_);   r++;
    rb_line(r); draw_int_field(r, 4, "power_cc_cnt",        pc,  MAX_POWER_);    r++;
    rb_line(r); draw_int_field(r, 4, "recovery_att_cnt",    rc,  MAX_RECOVERY_); r++;
    rb_line(r); draw_int_field(r, 4, "calibration_att_cnt", cc,  MAX_CAL_);      r++;
    rb_line(r); draw_int_field(r, 4, "reboot_cnt",          rbc, MAX_REBOOT_);   r++;
    *row = r;
}

/* ── public API ───────────────────────────────────────────────── */
void tui_init(void) {
    raw_on();
    direct_write(HIDE_CUR HOME CSI "2J");
    memset(evlog, 0, sizeof(evlog));
}

void tui_shutdown(void) {
    if (orig_out_fl >= 0) fcntl(STDOUT_FILENO, F_SETFL, orig_out_fl);
    direct_write(SHOW_CUR RST HOME CSI "2J");
    raw_off();
}

void tui_draw(uint64_t elapsed_us) {
    int W = tw();
    int H = th();
    rb_clear();
    rb_puts(HOME);

    int row = 1;

    /* ── title + cycle + uptime ────────────────────────────────── */
    rb_line(row);
    rb_puts(BOLD FG_CYAN " NET SUPERVISOR " RST);

    rb_move(row, 20);
    if (cycle_us < 1000)
        rb_printf(FG_WHITE "Cycle: %llu us" RST, (unsigned long long)cycle_us);
    else
        rb_printf(FG_WHITE "Cycle: %.2f ms" RST, (double)cycle_us / 1000.0);

    uint64_t ms  = (elapsed_us / 1000) % 1000;
    uint64_t sec = (elapsed_us / 1000000) % 60;
    uint64_t mn  = (elapsed_us / 60000000) % 60;
    uint64_t hr  = elapsed_us / 3600000000ULL;
    int uc = W - 19; if (uc < 40) uc = 40;
    rb_move(row, uc);
    rb_printf(FG_WHITE "UP %02llu:%02llu:%02llu.%03llu" RST,
              (unsigned long long)hr, (unsigned long long)mn,
              (unsigned long long)sec, (unsigned long long)ms);
    row++;
    draw_hr(row++, W);

    /* ── coordinator ───────────────────────────────────────────── */
    rb_line(row); rb_move(row, 2); rb_puts(BOLD "COORDINATOR" RST); row++;
    rb_line(row);
    {
        NETEnum cs = Net_coord___;
        draw_field(row, 4, "State:", st_name(cs), st_ansi(cs));
        char vb[16]; snprintf(vb, sizeof(vb), "%d", (int)Net_coord___velocity_);
        draw_field(row, 40, "Vel setpoint:", vb, FG_WHITE);
    }
    row++;
    draw_hr(row++, W);

    /* ── NODE 1 ────────────────────────────────────────────────── */
    draw_node(&row, 0);
    draw_hr(row++, W);

    /* ── NODE 2 ────────────────────────────────────────────────── */
    draw_node(&row, 1);
    draw_hr(row++, W);

    /* ── event log ─────────────────────────────────────────────── */
    rb_line(row); rb_move(row, 2); rb_puts(BOLD "EVENT LOG" RST); row++;

    int log_space = H - row - 1;
    if (log_space > LOG_LINES) log_space = LOG_LINES;
    if (log_space > log_cnt)   log_space = log_cnt;
    if (log_space < 0)         log_space = 0;

    int first = (log_cnt <= log_space)
        ? (log_hd - log_cnt   + LOG_LINES) % LOG_LINES
        : (log_hd - log_space + LOG_LINES) % LOG_LINES;

    for (int i = 0; i < log_space; i++) {
        int idx = (first + i) % LOG_LINES;
        rb_line(row); rb_move(row, 3);
        rb_printf(FG_WHITE "%.*s" RST, W - 4, evlog[idx]);
        row++;
    }
    while (row < H) { rb_line(row); row++; }

    /* ── key bar ───────────────────────────────────────────────── */
    rb_move(H, 1); rb_el();
    rb_puts(BG_WHITE FG_BLACK " S " RST " Start "
            BG_WHITE FG_BLACK " X " RST " Stop "
            BG_WHITE FG_BLACK " R " RST " Reset "
            BG_WHITE FG_BLACK " +/- " RST " Vel "
            BG_WHITE FG_BLACK " Q " RST " Quit ");

    rb_flush();
}

int tui_poll_key(void) {
    unsigned char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0) return 0;

    if (c == 0x1B) {
        unsigned char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) == 1 && seq[0] == '[') {
            if (read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[1] == 'A') return 10;  /* up arrow */
                if (seq[1] == 'B') return 11;  /* down arrow */
            }
        }
        return 0;
    }

    switch (c) {
        case 's': case 'S': return 1;
        case 'x': case 'X': return 2;
        case 'r': case 'R': return 3;
        case 'q': case 'Q': return -1;
        case  3:            return -1;
        case '+': case '=': return 10;
        case '-': case '_': return 11;
        default:            return 0;
    }
}
