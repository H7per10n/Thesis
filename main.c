/*
 * main.c – NET supervisor with ANSI TUI
 *
 * HMI:  S=start  X=stop  R=reset  Q=quit  Up/Down or +/-=velocity
 * Loop @ ~20 Hz.  Velocity clamped to [0..300] per the CIF model range.
 */

#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "can_if.h"
#include "NET_engine.h"
#include "net_tui.h"


#define CONTROL_CYCLE 10e3 // 10000us , 10ms

/* ── globals ──────────────────────────────────────────────────── */
static volatile sig_atomic_t sig_quit = 0;
static uint64_t start_us = 0;

static int hmi_start = 0;
static int hmi_stop  = 0;
static int hmi_reset = 0;
static int hmi_quit  = 0;

/* velocity setpoint — adjustable via arrow keys */
static int velocity_sp = 300;
#define VEL_MIN   0
#define VEL_MAX 300
#define VEL_STEP 10

/* ── time ─────────────────────────────────────────────────────── */
static inline uint64_t monotonic_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

static inline void format_hms_ms(uint64_t us, char *buf, size_t len) {
    uint64_t total_ms = us / 1000;
    uint64_t ms  = total_ms % 1000;
    uint64_t sec = (total_ms / 1000) % 60;
    uint64_t min = (total_ms / 60000) % 60;
    uint64_t hr  = total_ms / 3600000;
    snprintf(buf, len, "%02llu:%02llu:%02llu.%03llu",
             (unsigned long long)hr, (unsigned long long)min,
             (unsigned long long)sec, (unsigned long long)ms);
}

/* ── signal ───────────────────────────────────────────────────── */
static void handle_sigint(int sig) { (void)sig; sig_quit = 1; }

/* ── CIF callbacks ────────────────────────────────────────────── */

void NET_AssignInputVariables(void) {
    can_event_queue_clear();
    can_read_events();

    Net_coord_HMI__u_start_            = hmi_start;
    Net_coord_HMI__u_stop_             = hmi_stop;
    Net_coord_HMI__u_technician_reset_ = hmi_reset;
    hmi_start = hmi_stop = hmi_reset = 0;

    Net_NODE1_CAN_Evt_setup_ok_       = can_event_triggered(1, EVT_SETUP_OK);
    Net_NODE1_CAN_Evt_setup_error_    = can_event_triggered(1, EVT_SETUP_ERROR);
    Net_NODE1_CAN_Evt_cal_success_    = can_event_triggered(1, EVT_CAL_SUCCESS);
    Net_NODE1_CAN_Evt_cal_failed_     = can_event_triggered(1, EVT_CAL_FAILED);
    Net_NODE1_CAN_Evt_stopped_        = can_event_triggered(1, EVT_STOPPED);
    Net_NODE1_CAN_Evt_fault_          = can_event_triggered(1, EVT_FAULT);
    Net_NODE1_CAN_Evt_no_power_       = can_event_triggered(1, EVT_NO_POWER);
    Net_NODE1_CAN_Evt_power_restored_ = can_event_triggered(1, EVT_POWER_RESTORE);
    Net_NODE1_CAN_Evt_overheated_     = can_event_triggered(1, EVT_OVERHEATED);
    Net_NODE1_CAN_Evt_cooled_         = can_event_triggered(1, EVT_COOLED);
    Net_NODE1_CAN_Evt_fault_cleared_  = can_event_triggered(1, EVT_FAULT_CLEARED);
    Net_NODE1_CAN_Evt_recover_failed_ = can_event_triggered(1, EVT_RECOVER_FAILED);

    Net_NODE2_CAN_Evt_setup_ok_       = can_event_triggered(2, EVT_SETUP_OK);
    Net_NODE2_CAN_Evt_setup_error_    = can_event_triggered(2, EVT_SETUP_ERROR);
    Net_NODE2_CAN_Evt_cal_success_    = can_event_triggered(2, EVT_CAL_SUCCESS);
    Net_NODE2_CAN_Evt_cal_failed_     = can_event_triggered(2, EVT_CAL_FAILED);
    Net_NODE2_CAN_Evt_stopped_        = can_event_triggered(2, EVT_STOPPED);
    Net_NODE2_CAN_Evt_fault_          = can_event_triggered(2, EVT_FAULT);
    Net_NODE2_CAN_Evt_no_power_       = can_event_triggered(2, EVT_NO_POWER);
    Net_NODE2_CAN_Evt_power_restored_ = can_event_triggered(2, EVT_POWER_RESTORE);
    Net_NODE2_CAN_Evt_overheated_     = can_event_triggered(2, EVT_OVERHEATED);
    Net_NODE2_CAN_Evt_cooled_         = can_event_triggered(2, EVT_COOLED);
    Net_NODE2_CAN_Evt_fault_cleared_  = can_event_triggered(2, EVT_FAULT_CLEARED);
    Net_NODE2_CAN_Evt_recover_failed_ = can_event_triggered(2, EVT_RECOVER_FAILED);
}

void NET_InfoEvent(NET_Event_ event, BoolType pre) {
    if (pre) return;

    switch (event) {
        case Net_NODE1___c_cal_reject_:   can_cmd_simple(1, REG_CMD_CAL_REJECT); break;
        case Net_NODE1___c_calibrate_:    can_cmd_simple(1, REG_CMD_CALIBRATE);  break;
        case Net_NODE1___c_enable_:       can_cmd_simple(1, REG_CMD_ENABLE);     break;
        case Net_NODE1___c_reboot_:       can_cmd_simple(1, REG_CMD_REBOOT);     break;
        case Net_NODE1___c_recover_:      can_cmd_simple(1, REG_CMD_RECOVER);    break;
        case Net_NODE1___c_set_velocity_: can_cmd_velocity(1, (float)Net_NODE1___cur_vel_); break;
        case Net_NODE1___c_stop_:         can_cmd_simple(1, REG_CMD_STOP);       break;

        case Net_NODE2___c_cal_reject_:   can_cmd_simple(2, REG_CMD_CAL_REJECT); break;
        case Net_NODE2___c_calibrate_:    can_cmd_simple(2, REG_CMD_CALIBRATE);  break;
        case Net_NODE2___c_enable_:       can_cmd_simple(2, REG_CMD_ENABLE);     break;
        case Net_NODE2___c_reboot_:       can_cmd_simple(2, REG_CMD_REBOOT);     break;
        case Net_NODE2___c_recover_:      can_cmd_simple(2, REG_CMD_RECOVER);    break;
        case Net_NODE2___c_set_velocity_: can_cmd_velocity(2, (float)Net_NODE2___cur_vel_); break;
        case Net_NODE2___c_stop_:         can_cmd_simple(2, REG_CMD_STOP);       break;

        default: break;
    }

    char ts[32]; format_hms_ms(monotonic_us() - start_us, ts, sizeof(ts));
    char line[128]; snprintf(line, sizeof(line), "[%s] %s", ts, NET_event_names[event]);
    tui_log_event(line);
}

void NET_PrintOutput(const char *text, const char *fname) {
    (void)text; (void)fname;
}

/* ── main ─────────────────────────────────────────────────────── */
int main(void) {
    start_us = monotonic_us();
	uint32_t delayer_cnt =1;
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);

    const char *interface = "can0";
    if (can_init(interface, 1000000) < 0) {
        fprintf(stderr, "Failed to init CAN on %s\n", interface);
        return 1;
    }

    can_cmd_simple(1, REG_CMD_REBOOT);
    can_cmd_simple(2, REG_CMD_REBOOT);
    //usleep(10000000);

    tui_init();
    NET_EngineFirstStep();
    tui_draw(monotonic_us() - start_us);
    
    while (!hmi_quit && !sig_quit) {
        uint64_t tick_start = monotonic_us();

        /* poll keyboard */
        int key;
        while ((key = tui_poll_key()) != 0) {
            switch (key) {
                case  1: hmi_start = 1; break;
                case  2: hmi_stop  = 1; break;
                case  3: hmi_reset = 1; break;
                case -1: hmi_stop  = 1; hmi_quit = 1; break;
                case 10: /* velocity up */
                    velocity_sp += VEL_STEP;
                    if (velocity_sp > VEL_MAX) velocity_sp = VEL_MAX;
                    break;
                case 11: /* velocity down */
                    velocity_sp -= VEL_STEP;
                    if (velocity_sp < VEL_MIN) velocity_sp = VEL_MIN;
                    break;
            }
        }

        /* write velocity setpoint into the CIF model variable */
        Net_coord___velocity_ = velocity_sp;

        /* timed control cycle */
        uint64_t cyc0 = monotonic_us();
        NET_EngineTimeStep(0.1);
        tui_set_cycle_us(monotonic_us() - cyc0);
		delayer_cnt++;
		//recover delay
		if (delayer_cnt >= 100) {
			Net_NODE1___delay_passed_ = TRUE;
			delayer_cnt = 0;
		} else {
			Net_NODE1___delay_passed_ = FALSE;
		}
        tui_draw(monotonic_us() - start_us);
        
        if (hmi_quit && Net_coord___ == _NET_IDLE) break;
        uint64_t elapsed = monotonic_us() - tick_start;
        if (elapsed < CONTROL_CYCLE) usleep((unsigned int)(CONTROL_CYCLE - elapsed));
    }

    /* graceful stop */
    for (int i = 0; i < 30; i++) {
        hmi_stop = 1;
        NET_EngineTimeStep(0.1);
        tui_draw(monotonic_us() - start_us);
        usleep(1000000);
        if (Net_coord___ == _NET_IDLE) break;
    }

    tui_shutdown();
    can_close();
    printf("Supervisor shutdown complete.\n");
    return 0;
}
