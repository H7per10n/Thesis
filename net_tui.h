#ifndef NET_TUI_H
#define NET_TUI_H

#include <stdint.h>
#include "NET_engine.h"

void tui_log_event(const char *msg);
void tui_init(void);
void tui_shutdown(void);
void tui_set_cycle_us(uint64_t us);
void tui_draw(uint64_t elapsed_us);

/* Non-blocking key poll.
 * Returns: 0=nothing, 1=start, 2=stop, 3=reset, -1=quit,
 *          10=velocity up, 11=velocity down.
 */
int tui_poll_key(void);

#endif
