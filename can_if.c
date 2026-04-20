#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include "can_if.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

/* State */
static int can_rx_socket = -1;
static int can_tx_socket = -1;
static int can_if_index = 0;

#define MAX_EVENT_QUEUE 128
static DecodedEvent event_queue[MAX_EVENT_QUEUE];
static int event_queue_count = 0;

// Init
int can_init(const char *interface, uint32_t bitrate)
{
    struct ifreq ifr;
    struct sockaddr_can addr;
    int temp;

    /* Open temporary socket to query interface */
    temp = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (temp < 0) {
        perror("[CAN] socket");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);

    /* Check interface exists */
    if (ioctl(temp, SIOCGIFINDEX, &ifr) < 0) {
        fprintf(stderr, "[CAN] Interface %s not found.\n", interface);
        fprintf(stderr, "Setup with:\n");
        fprintf(stderr, "  ip link set %s type can bitrate %u\n", interface, bitrate);
        fprintf(stderr, "  ip link set %s up\n", interface);
        close(temp);
        return -1;
    }

    can_if_index = ifr.ifr_ifindex;

    /* Check interface flags (UP/DOWN) */
    if (ioctl(temp, SIOCGIFFLAGS, &ifr) < 0) {
        perror("[CAN] SIOCGIFFLAGS");
        close(temp);
        return -1;
    }

    if (!(ifr.ifr_flags & IFF_UP)) {
        fprintf(stderr, "[CAN] Interface %s is DOWN\n", interface);
        fprintf(stderr, "Run:\n");
        fprintf(stderr, "  ip link set %s type can bitrate %u\n", interface, bitrate);
        fprintf(stderr, "  ip link set %s up\n", interface);
        close(temp);
        return -1;
    }

    close(temp);

    /* ---------------- RX SOCKET ---------------- */

    can_rx_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_rx_socket < 0) {
        perror("[CAN] RX socket");
        return -1;
    }

    /* Non-blocking RX */
    fcntl(can_rx_socket, F_SETFL,
          fcntl(can_rx_socket, F_GETFL, 0) | O_NONBLOCK);

    /* Event filter */
    struct can_filter rfilter[1];
    rfilter[0].can_id =
        (PACKET_TYPE_EVENT << CAN_PACKET_TYPE_SHIFT) | CAN_EFF_FLAG;

    rfilter[0].can_mask =
        (0xF << CAN_PACKET_TYPE_SHIFT) | CAN_EFF_FLAG | CAN_RTR_FLAG;

    if (setsockopt(can_rx_socket, SOL_CAN_RAW, CAN_RAW_FILTER,
                   &rfilter, sizeof(rfilter)) < 0) {
        perror("[CAN] filter");
        close(can_rx_socket);
        return -1;
    }

    /* Enable timestamps */
    int ts_on = 1;
    if (setsockopt(can_rx_socket, SOL_SOCKET,
                   SO_TIMESTAMPNS, &ts_on, sizeof(ts_on)) < 0) {
        fprintf(stderr, "[CAN] Warning: SO_TIMESTAMPNS failed: %s\n",
                strerror(errno));
    }

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = can_if_index;

    if (bind(can_rx_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("[CAN] RX bind");
        close(can_rx_socket);
        return -1;
    }

    /* ---------------- TX SOCKET ---------------- */

    can_tx_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_tx_socket < 0) {
        perror("[CAN] TX socket");
        close(can_rx_socket);
        return -1;
    }

    if (bind(can_tx_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("[CAN] TX bind");
        close(can_rx_socket);
        close(can_tx_socket);
        return -1;
    }

    printf("[CAN] %s ready (RX fd=%d TX fd=%d)\n",
           interface, can_rx_socket, can_tx_socket);

    return 0;
}

void can_close(void) {
    if (can_rx_socket >= 0) close(can_rx_socket);
    if (can_tx_socket >= 0) close(can_tx_socket);
    can_rx_socket = -1;
    can_tx_socket = -1;
}

int can_read_events(void) {
    if (can_rx_socket < 0) return -1;

    struct can_frame frame;
    struct iovec iov = { .iov_base = &frame, .iov_len = sizeof(frame) };
    char cmsg_buf[CMSG_SPACE(sizeof(struct timespec))];
    struct msghdr msg = {
        .msg_name    = NULL,
        .msg_namelen = 0,
        .msg_iov     = &iov,
        .msg_iovlen  = 1,
        .msg_control = cmsg_buf,
        .msg_controllen = sizeof(cmsg_buf),
    };

    int count = 0;

    while (1) {
        msg.msg_controllen = sizeof(cmsg_buf);
        int nbytes = recvmsg(can_rx_socket, &msg, MSG_DONTWAIT);
        if (nbytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            return -1;
        }
        if (nbytes < (int)sizeof(frame)) continue;

        /* Extract kernel timestamp from ancillary data */
        uint64_t ts_us = 0;
        for (struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
             cmsg != NULL;
             cmsg = CMSG_NXTHDR(&msg, cmsg)) {
            if (cmsg->cmsg_level == SOL_SOCKET &&
                cmsg->cmsg_type == SO_TIMESTAMPNS) {
                struct timespec *ts = (struct timespec *)CMSG_DATA(cmsg);
                ts_us = (uint64_t)ts->tv_sec * 1000000 + ts->tv_nsec / 1000;
                break;
            }
        }

        if (frame.can_id & CAN_EFF_FLAG) {
            uint32_t id = frame.can_id & CAN_EFF_MASK;
            uint8_t node = (id >> CAN_ADDRESS_SHIFT) & 0xFF;
            uint8_t evt  = (id >> CAN_REGISTER_SHIFT) & 0xFF;
            uint8_t seq  = id & 0xFF;

            if (event_queue_count < MAX_EVENT_QUEUE) {
                event_queue[event_queue_count].node_id    = node;
                event_queue[event_queue_count].event      = (CANEvent)evt;
                event_queue[event_queue_count].sequence    = seq;
                event_queue[event_queue_count].timestamp_us = ts_us;
                event_queue_count++;
                count++;
            }
        }
    }
    return count;
}

/* Send commands */
static uint32_t encode_can_id(uint8_t node_id, uint8_t packet_type, uint8_t reg, uint8_t motor_idx) {
    return ((uint32_t)node_id << CAN_ADDRESS_SHIFT)
         | ((uint32_t)packet_type << CAN_PACKET_TYPE_SHIFT)
         | ((uint32_t)reg << CAN_REGISTER_SHIFT)
         | ((uint32_t)motor_idx << CAN_MOTOR_INDEX_SHIFT);
}

int can_cmd_simple(uint8_t node_id, uint8_t reg) {
    if (can_tx_socket < 0) return -1;
    
    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));
    
    /* Extended CAN ID: [node][0x2][reg][0x00] */
    frame.can_id = encode_can_id(node_id, PACKET_TYPE_COMMAND, reg, 0) | CAN_EFF_FLAG;
    frame.can_dlc = 0;  /* No data */
    
    return write(can_tx_socket, &frame, sizeof(frame)) < 0 ? -1 : 0;
}

int can_cmd_velocity(uint8_t node_id, float velocity) {
    if (can_tx_socket < 0) return -1;
    
    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));
    
    /* Extended CAN ID: [node][0x2][0xF6][0x00] */
    frame.can_id = encode_can_id(node_id, PACKET_TYPE_COMMAND, REG_VELOCITY_TARGET, 0) | CAN_EFF_FLAG;
    
    /* 4-byte float data (little-endian) */
    memcpy(frame.data, &velocity, 4);
    frame.can_dlc = 4;
    
    return write(can_tx_socket, &frame, sizeof(frame)) < 0 ? -1 : 0;
}

/* Event queue */
bool can_event_triggered(uint8_t node_id, CANEvent event) {
    for (int i = 0; i < event_queue_count; i++) {
        if (event_queue[i].node_id == node_id && event_queue[i].event == event)
            return true;
    }
    return false;
}

void can_event_queue_clear(void) {
    event_queue_count = 0;
}

int can_event_queue_count(void) {
    return event_queue_count;
}

DecodedEvent* can_event_queue_get(int index) {
    return (index >= 0 && index < event_queue_count) ? &event_queue[index] : NULL;
}
