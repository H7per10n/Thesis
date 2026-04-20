#include "can_if.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

//
static volatile int running = 1;

void sigint_handler(int sig) {
    (void)sig;
    running = 0;
}

void print_event(uint8_t node_id, CANEvent event, uint8_t seq) {
    const char *event_names[] = {
        "NONE", "SETUP_OK", "SETUP_ERROR", "CAL_SUCCESS", "CAL_FAILED",
        "FAULT", "NO_POWER", "POWER_RESTORE", "STOPPED", "OVERHEATED",
        "COOLED", "FAULT_CLEARED", "RECOVER_FAILED"
    };
    
    const char *name = (event <= EVT_RECOVER_FAILED) ? event_names[event] : "UNKNOWN";
    printf("\n[EVENT] Node %d: %s (seq %d)\n", node_id, name, seq);
    fflush(stdout);
}

void* event_monitor_thread(void* arg) {
    (void)arg;
    while (running) {
        can_event_queue_clear();
        can_read_events();
        for (int k = 0; k < can_event_queue_count(); k++) {
            DecodedEvent *evt = can_event_queue_get(k);
            print_event(evt->node_id, evt->event, evt->sequence);
        }
        usleep(50000);
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <can_interface> <node_id>\n", argv[0]);
        return 1;
    }
    
    const char *interface = argv[1];
    uint8_t node_id = atoi(argv[2]);
    signal(SIGINT, sigint_handler);
    
    if (can_init(interface, 1000000) < 0) {
        fprintf(stderr, "Failed to init CAN\n");
        return 1;
    }
    
    printf("=== Motor Test: Node %d ===\n", node_id);
    printf("Monitoring CAN events in background...\n\n");
    
    pthread_t monitor_tid;
    pthread_create(&monitor_tid, NULL, event_monitor_thread, NULL);
    
    printf("Phase 1: Calibration (3x with 15sec interval)\n");
    for (int i = 1; i <= 3 && running; i++) {
        printf("  Calibrate %d/3...\n", i);
        fflush(stdout);
        can_cmd_simple(node_id, REG_CMD_CALIBRATE);
        sleep(15);
    }
    if (!running) goto cleanup;
    printf("\n");
    
    printf("Phase 2: Enable motor\n");
    fflush(stdout);
    can_cmd_simple(node_id, REG_CMD_ENABLE);
    sleep(2);
    if (!running) goto cleanup;
    printf("\n");
    
    
    const float amplitude = 300.0f;
    const float period = 25.0f;
    const float omega = 2.0f * M_PI / period;
    const int update_ms = 1;
    unsigned long time_ms = 0;
    
    printf("Phase 3: Sine wave velocity\n");
    printf("  Amplitude: %.2f rad/s, Period: %.2f sec\n",amplitude, period);
    printf("  Press Ctrl+C to stop\n\n");
    
    while (running) {
        float time_sec = time_ms / 1000.0f;
        float vel = amplitude * sinf(omega * time_sec);
        printf("\r[Time: %6.2fs] Velocity: %+7.2f rad/s", time_sec, vel);
        fflush(stdout);
        can_cmd_velocity(node_id, vel);
        usleep(update_ms * 1000);
        time_ms += update_ms;
    }
    printf("\n\n");
    
cleanup:
    printf("Stopping motor...\n");
    fflush(stdout);
    can_cmd_simple(node_id, REG_CMD_STOP);
    sleep(10);
    running = 0;
    pthread_join(monitor_tid, NULL);
    can_close();
    printf("\nDone.\n");
    return 0;
}
