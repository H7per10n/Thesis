#pragma once
#include "comms/can/CANCommander.h"
#include <FlexCAN_T4.h>

// ══════════════════════════════════════════════════
// NODE CONFIGURATION
// ══════════════════════════════════════════════════
#ifndef CAN_NODE_ID
  #define CAN_NODE_ID 1
#endif
#ifndef CAN_BAUD
  #define CAN_BAUD 1000000
#endif
#ifndef CAN_BUS
  #define CAN_BUS CAN1
#endif

// ══════════════════════════════════════════════════
// STATE MACHINE
// ══════════════════════════════════════════════════
enum State : uint8_t {
    INIT          = 0, // initialization 
    SETUP_ERROR   = 1, // failed intitialization
    NEEDS_RECAL   = 2, // in need of calibration
    CALIBRATING   = 3, // actually calibrating
    CAL_FAILED    = 4, // claibration failure
    IDLE          = 5, // IDLE but ready for operation
    RUNNING       = 6, // runninng 
    STOPPING      = 7, // stopping
    FAULT         = 8, // Unified fault state (thermal OR electrical)
    NO_POWER      = 9, // no power  
};

const char* State_Names[10] = {
    "INIT","SETUP_ERROR","NEEDS_RECAL","CALIBRATING","CAL_FAILED",
    "IDLE","RUNNING","STOPPING","FAULT","NO_POWER"
};

// ══════════════════════════════════════════════════
// CAN EVENTS
// ══════════════════════════════════════════════════
enum CANEvent : uint8_t {
    EVT_NONE            = 0x00,
    EVT_SETUP_OK        = 0x01, //setup completed
    EVT_SETUP_ERROR     = 0x02, //setup not completed
    EVT_CAL_SUCCESS     = 0x03, //calibration routine successfull
    EVT_CAL_FAILED      = 0x04, //calibration failed
    EVT_FAULT           = 0x05, // Generic fault entry
    EVT_NO_POWER        = 0x06, // power was cut
    EVT_POWER_RESTORE   = 0x07, // power came back
    EVT_STOPPED         = 0x08, // the motor actually stoped
    EVT_OVERHEATED      = 0x09, // Specific thermal event
    EVT_COOLED          = 0x0A, // Thermal cleared
    EVT_FAULT_CLEARED   = 0x0B, // Electrical fault cleared
    EVT_RECOVER_FAILED  = 0x0C,  // Recovery failed
};

const char* Event_Names[13] = {
    "EVT_NONE","EVT_SETUP_OK","EVT_SETUP_ERROR","EVT_CAL_SUCCESS","EVT_CAL_FAILED",
    "EVT_FAULT","EVT_NO_POWER","EVT_POWER_RESTORE","EVT_STOPPED",
    "EVT_OVERHEATED","EVT_COOLED","EVT_FAULT_CLEARED","EVT_RECOVER_FAILED"
};

// ══════════════════════════════════════════════════
// TRANSITION TABLE
// ══════════════════════════════════════════════════
static const State TRANSITIONS[][2] = {
    { INIT,        NEEDS_RECAL  },
    { INIT,        SETUP_ERROR  },
    { INIT,        FAULT        },
    { INIT,        NO_POWER     },

    { NEEDS_RECAL, CALIBRATING  },
    { NEEDS_RECAL, FAULT        },
    { NEEDS_RECAL, NO_POWER     },

    { CALIBRATING, IDLE         },
    { CALIBRATING, CAL_FAILED   },
    { CALIBRATING, FAULT        },
    { CALIBRATING, NO_POWER     },

    { CAL_FAILED,  CALIBRATING  },
    { CAL_FAILED,  NEEDS_RECAL  },
    { CAL_FAILED,  FAULT        },
    { CAL_FAILED,  NO_POWER     },

    { IDLE,        RUNNING      },
    { IDLE,        CALIBRATING  },
    { IDLE,        FAULT        },
    { IDLE,        NO_POWER     },

    { RUNNING,     STOPPING     }, // when stop is called
    { RUNNING,     FAULT        }, // when fualt happens 
    { RUNNING,     NO_POWER     }, // when power is cut off

    { STOPPING,    IDLE         }, //when actually stopped we go to idle
    { STOPPING,    RUNNING      }, // if while stopping we decide to run again
    { STOPPING,    FAULT        }, // if while stopping we get an error
    { STOPPING,    NO_POWER     }, // if while we are stopping the power gets cut off

    { FAULT,       NEEDS_RECAL  },  // Successfull recovery sends us to NEEDS_RECAL
    { FAULT,       FAULT        }, //  Failed recovery keeps us in the same spot.
    { FAULT,       NO_POWER     }, // power cut off while having fault !

    { NO_POWER,    FAULT        },  // Power restore -> Fault (needs recovery)
};

// ══════════════════════════════════════════════════
// FAULT REASON (INTERNAL ONLY)
// ══════════════════════════════════════════════════
static enum FaultReason : uint8_t {
    FAULT_REASON_NONE,
    FAULT_REASON_ELECTRICAL,
    FAULT_REASON_THERMAL,
    FAULT_REASON_POWER_RESTORE,
} _fault_reason = FAULT_REASON_NONE;


// EXTERN DECLARATIONS
extern BLDCMotor         motor;
extern BLDCDriver3PWM    driver;
extern volatile State    state;
extern volatile float    target_vel;

// CAN HARDWARE
static FlexCAN_T4<CAN_BUS, RX_SIZE_256, TX_SIZE_16> _flexcan;
static TEENSY_CAN<CAN_BUS> can_hw(_flexcan);
static CANCommander _commander(can_hw, CAN_NODE_ID);

// EVENT PUSH
static uint8_t _evt_seq = 0;
static void can_push_event(CANEvent evt) {
    uint32_t can_id = ((uint32_t)CAN_NODE_ID << CAN_ADDRESS_SHIFT)
                    | ((uint32_t)0x4          << CAN_PACKET_TYPE_SHIFT)
                    | ((uint32_t)evt          << CAN_REGISTER_SHIFT)
                    | ((uint32_t)_evt_seq++   << CAN_MOTOR_INDEX_SHIFT);
    uint8_t empty[1] = {0};
    can_hw.write(CanMsg(CanExtendedId(can_id), 0, empty));
}


// TRANSITION FUNCTION

static bool transition_to(State next, CANEvent evt = EVT_NONE) {
    for (auto& t : TRANSITIONS) {
        if (t[0] == state && t[1] == next) {
            state = next;
            if (evt != EVT_NONE) {
                can_push_event(evt);
                Serial.printf(":::[CAN EVENT] %s\n", Event_Names[(int)evt]);
            }
            Serial.printf("[FSM] %s → %s\n", State_Names[(int)t[0]], State_Names[(int)next]);
            return true;
        }
    }
    Serial.printf("[FSM] ILLEGAL: %s → %s\n", State_Names[(int)state], State_Names[(int)next]);
    return false;
}


// EN_GATE RESET

static bool drv_reset() {
    driver.disable();
    delayMicroseconds(10);
    driver.enable();
    delay(15);
    return digitalRead(PIN_NFAULT);
}


// REGISTER HANDLERS


// 0xF0 — state (read-only)
static bool _reg_state_read(RegisterIO& io, FOCMotor*) {
    io << (uint8_t)state;
    return true;
}
static bool _reg_state_write(RegisterIO&, FOCMotor*) { return false; }

// 0xF1 — cmd_calibrate
static bool _reg_calibrate_read(RegisterIO& io, FOCMotor*) {
    io << (uint8_t)(state == CALIBRATING ? 1 : 0);
    return true;
}
static bool _reg_calibrate_write(RegisterIO&, FOCMotor*) {
    return transition_to(CALIBRATING);
}

// 0xF2 — cmd_enable
static bool _reg_enable_read(RegisterIO& io, FOCMotor*) {
    io << (uint8_t)(state == RUNNING ? 1 : 0);
    return true;
}
static bool _reg_enable_write(RegisterIO&, FOCMotor* m) {
    m->enable();
    return transition_to(RUNNING);
}

// 0xF3 — cmd_stop
static bool _reg_stop_read(RegisterIO& io, FOCMotor*) {
    io << (uint8_t)(state == STOPPING ? 1 : 0);
    return true;
}
static bool _reg_stop_write(RegisterIO&, FOCMotor*) {
    return transition_to(STOPPING);
}

// 0xF4 — cmd_recover (FIXED)
static bool _reg_recover_read(RegisterIO& io, FOCMotor*) {
    io << (uint8_t)(state == FAULT ? 1 : 0);  // Only FAULT state now
    return true;
}

static bool _reg_recover_write(RegisterIO&, FOCMotor*) {
    if (state != FAULT) return false;
    bool pwrgd  = digitalRead(PIN_PWRGD);
    bool nfault = digitalRead(PIN_NFAULT);
    bool noctw  = digitalRead(PIN_NOCTW);
    Serial.printf("[RECOVER] PWRGD=%d nFAULT=%d nOCTW=%d reason=%d\n", 
                  pwrgd, nfault, noctw, _fault_reason);
    // if (!pwrgd) {
    //     Serial.println("[RECOVER] FAIL: No power");
    //     transition_to(FAULT, EVT_RECOVER_FAILED);
    //     return false;
    // }
    if (_fault_reason == FAULT_REASON_THERMAL && !noctw) {
        Serial.println("[RECOVER] FAIL: Still overheated");
        transition_to(FAULT, EVT_RECOVER_FAILED);
        return false;
    }
    
    if (!nfault) {
        Serial.println("[RECOVER] Toggling EN_GATE...");
        if (!drv_reset()) {
            Serial.println("[RECOVER] FAIL: EN_GATE reset failed");
            transition_to(FAULT, EVT_RECOVER_FAILED);
            return false;
        }
    }
    
    delay(15);
    pwrgd  = digitalRead(PIN_PWRGD);
    nfault = digitalRead(PIN_NFAULT);
    noctw  = digitalRead(PIN_NOCTW);
    
    if (pwrgd && nfault && noctw) {
        CANEvent evt;
        switch(_fault_reason) {
            case FAULT_REASON_THERMAL:
                evt = EVT_COOLED;
                Serial.println("[RECOVER] Thermal fault cleared");
                break;
            case FAULT_REASON_ELECTRICAL:
                evt = EVT_FAULT_CLEARED;
                Serial.println("[RECOVER] Electrical fault cleared");
                break;
            case FAULT_REASON_POWER_RESTORE:
                evt = EVT_FAULT_CLEARED;
                Serial.println("[RECOVER] Power restored, fault cleared");
                break;
            default:
                evt = EVT_FAULT_CLEARED;
                break;
        }
        _fault_reason = FAULT_REASON_NONE;
        return transition_to(NEEDS_RECAL, evt);
    }
    
    Serial.printf("[RECOVER] FAIL: Final verification PWRGD=%d nFAULT=%d nOCTW=%d\n",
                  pwrgd, nfault, noctw);
    transition_to(FAULT, EVT_RECOVER_FAILED);
    return false;
}
// 0xF5 — cmd_cal_reject
static bool _reg_cal_reject_read(RegisterIO& io, FOCMotor*) {
    io << (uint8_t)(state == CAL_FAILED ? 1 : 0);
    return true;
}
static bool _reg_cal_reject_write(RegisterIO&, FOCMotor*) {
    return transition_to(NEEDS_RECAL);
}

// 0xF6 — velocity_target (FIXED - was 0xF7 but registered as 0xF6)
static bool _reg_velocity_read(RegisterIO& io, FOCMotor* m) {
    io << m->target;
    return true;
}
static bool _reg_velocity_write(RegisterIO& io, FOCMotor* m) {
    if (state != RUNNING) return false;
    float v ; io >> v;
    target_vel = v;
    return true;
}


static bool _reg_reboot_write(RegisterIO&, FOCMotor*) {
    Serial.println("[CMD] Rebooting...");
    delay(20);  // Give time for serial output
    SCB_AIRCR = 0x05FA0004;  // Reboot
    return true;  // Never actually returns
}

inline void can_node_setup() {
    _flexcan.begin();
    _flexcan.setBaudRate(CAN_BAUD);
    _commander.init();
    _commander.addMotor(&motor);
    _commander.addCustomRegister(0xF0, 1, _reg_state_read,       _reg_state_write);
    _commander.addCustomRegister(0xF1, 1, _reg_calibrate_read,   _reg_calibrate_write);
    _commander.addCustomRegister(0xF2, 1, _reg_enable_read,      _reg_enable_write);
    _commander.addCustomRegister(0xF3, 1, _reg_stop_read,        _reg_stop_write);
    _commander.addCustomRegister(0xF4, 1, _reg_recover_read,     _reg_recover_write);
    _commander.addCustomRegister(0xF5, 1, _reg_cal_reject_read,  _reg_cal_reject_write);
    _commander.addCustomRegister(0xF6, 4, _reg_velocity_read,    _reg_velocity_write);
    _commander.addCustomRegister(0xF8, 0, nullptr, _reg_reboot_write);
    Serial.printf("[CAN] Node %d ready\n", CAN_NODE_ID);
}
inline void can_node_run() {
    _commander.run();
    
    // Only disable motor if not in RUNNING, CALIBRATING, or STOPPING
    // STOPPING needs motor enabled for FOC control
    if (motor.enabled && state != RUNNING && state != CALIBRATING && state != STOPPING) {
        motor.disable();
    }
    // Don't zero target during STOPPING - let decay handle it
    // Only zero target in states where motor shouldn't move
    if (state != RUNNING && state != STOPPING && motor.target != 0.0f) {
        motor.target = target_vel = 0.0f;
    }
}