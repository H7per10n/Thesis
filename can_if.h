#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

/* Extended CAN ID (29-bit) bit layout - used for ALL frames
 * 
 * Bits 27-20: Node ID (8 bits) - 0 to 255
 * Bits 19-16: Packet Type (4 bits)
 * Bits 15-8:  Register/Event (8 bits)
 * Bits 7-0:   Motor Index (8 bits, always 0)
 */
#define CAN_ADDRESS_SHIFT       20    /* Node ID */
#define CAN_PACKET_TYPE_SHIFT   16    /* Packet type */
#define CAN_REGISTER_SHIFT      8     /* Register/Event */
#define CAN_MOTOR_INDEX_SHIFT   0     /* Motor index (always 0) */

/* Packet types */
#define PACKET_TYPE_COMMAND     0x2   /* Commands TO nodes */
#define PACKET_TYPE_EVENT       0x4   /* Events FROM nodes */

/* Registers */
#define REG_CMD_CALIBRATE       0xF1
#define REG_CMD_ENABLE          0xF2
#define REG_CMD_STOP            0xF3
#define REG_CMD_RECOVER         0xF4
#define REG_CMD_CAL_REJECT      0xF5
#define REG_VELOCITY_TARGET     0xF6
#define REG_CMD_REBOOT          0xF8

/* Event codes */
typedef enum {
    EVT_NONE = 0x00,
    EVT_SETUP_OK = 0x01,
    EVT_SETUP_ERROR = 0x02,
    EVT_CAL_SUCCESS = 0x03,
    EVT_CAL_FAILED = 0x04,
    EVT_FAULT = 0x05,
    EVT_NO_POWER = 0x06,
    EVT_POWER_RESTORE = 0x07,
    EVT_STOPPED = 0x08,
    EVT_OVERHEATED = 0x09,
    EVT_COOLED = 0x0A,
    EVT_FAULT_CLEARED = 0x0B,
    EVT_RECOVER_FAILED = 0x0C,
} CANEvent;

typedef struct {
    uint8_t node_id;
    CANEvent event;
    uint8_t sequence;
    uint64_t timestamp_us;   
} DecodedEvent;

/* Init */
int can_init(const char *interface, uint32_t bitrate);
void can_close(void);

/* Read events (packet type 0x4) */
int can_read_events(void);

/* Send commands (packet type 0x2) */
int can_cmd_simple(uint8_t node_id, uint8_t reg);           /* No data */
int can_cmd_velocity(uint8_t node_id, float velocity);      /* 4-byte float */

/* Event queue */
bool can_event_triggered(uint8_t node_id, CANEvent event);
void can_event_queue_clear(void);
int can_event_queue_count(void);
DecodedEvent* can_event_queue_get(int index);

#endif