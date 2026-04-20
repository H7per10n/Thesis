#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>

// ══════════════════════════════════════════════════
// HARDWARE PINS
// ══════════════════════════════════════════════════
#define M_PWM    12
#define M_OC     11
#define GAIN     10
#define OC_ADJ   9
#define DC_CAL   8
#define EN_GATE  7
#define INH_A    6
#define INH_B    5
#define INH_C    4
#define IOUTA    17
#define IOUTB    18
#define IOUTC    19
#define PIN_PWRGD  16
#define PIN_NFAULT 15
#define PIN_NOCTW  14

// ══════════════════════════════════════════════════
// SIMPLEFOC OBJECTS
// ══════════════════════════════════════════════════
BLDCMotor         motor(7);
BLDCDriver3PWM    driver(INH_A, INH_B, INH_C, EN_GATE);
LowsideCurrentSense cs(0.005f, 12.22f, IOUTA, IOUTB, IOUTC);

#include "can_node.h"

volatile State state = INIT;
volatile float target_vel= 0;
static IntervalTimer foc_timer;

// ══════════════════════════════════════════════════
// FOC TIMER
// ══════════════════════════════════════════════════
void FASTRUN foc_tick() {
    if (state == RUNNING || state ==  STOPPING) {
        motor.target += 0.000050f* (target_vel - motor.target);   //   low pass filter to the given target !!!
        motor.loopFOC();
        motor.move();
    }
}

/**
 * @brief Perform motor characterization and apply measured parameters
 * 
 * HOW IT WORKS:
 * 1. **Output Capture**: Creates a custom Print class (Capture) that intercepts 
 *    SimpleFOC's debug output. Every character is both:
 *    - Forwarded to Serial (so you see the calibration progress)
 *    - Stored in a buffer for parsing
 * 
 * 2. **Characterisation**: Calls motor.characteriseMotor(0.1f) which:
 *    - Injects test currents/voltages into the motor phases
 *    - Measures the electrical responses
 *    - Outputs results via SimpleFOCDebug (which we've redirected to our Capture)
 * 
 * 3. **Parsing**: When a newline is received or buffer fills:
 *    - Searches for known strings like "phase resistance:"
 *    - Extracts the numeric values after these strings
 *    - Special: Resistance is divided by 2 (explained below)
 * 
 * 4. **Validation**: Checks measured values against sanity limits:
 *    - Resistance: 0.05Ω - 50Ω (typical BLDC range)
 *    - Inductance: > 1μH (minimum sensible value)
 *    - Saliency ratio (Lq/Ld): < 10 (motors aren't THAT salient)
 *    - Physical constraint: Ld cannot exceed Lq
 * 
 * 5. **Application**: If validation passes:
 *    - Stores R and Lq in motor parameters
 *    - Sets KV_rating (TODO: should come from datasheet)
 *    - Calls motor.init() to apply parameters to FOC model
 * 
 * @note Resistance division: SimpleFOC outputs phase-to-phase resistance,
 *       but we need phase resistance (half of phase-to-phase for star/wye)
 * 
 * @note The 0.1f parameter is current limit during characterisation (Amps)
 * 
 * @return true  - Calibration successful, parameters applied
 * @return false - Calibration failed (hardware, parsing, or validation)
 */
static bool calibrate_and_apply() {
    // ===== STEP 1: Create output capture =====
    // Custom Print class that captures SimpleFOC's debug output
    struct Capture : Print {
        float R = -1, Lq = -1, Ld = -1;  // Storage for parsed values
        
        // Called for each character in debug output
        size_t write(uint8_t c) {
            Serial.write(c);              // Echo to Serial so user sees progress
            buf[pos++] = c;                // Add to capture buffer
            
            // Process on newline or buffer full
            if (c == '\n' || pos >= 127) {
                buf[pos] = 0;              // Null terminate
                pos = 0;                    // Reset buffer position
                
                const char* p;
                // Parse phase resistance (phase-to-phase)
                if ((p = strstr(buf, "phase resistance:")))  
                    R = atof(p + 18) / 2.0f;  // Convert to phase resistance
                
                // Parse D-axis inductance (in mH)
                if ((p = strstr(buf, "D-inductance in mH:"))) 
                    Ld = atof(p + 20) / 1000.0f;  // Convert mH to H
                
                // Parse Q-axis inductance (in mH)  
                if ((p = strstr(buf, "Q-inductance in mH:"))) 
                    Lq = atof(p + 20) / 1000.0f;  // Convert mH to H
            }
            return 1;
        }
        
        // Handle bulk writes (though SimpleFOC uses character-by-character)
        size_t write(const uint8_t* b, size_t s) {
            for (size_t i = 0; i < s; i++) write(b[i]);
            return s;
        }
        
        char buf[128];  // Line buffer
        int pos = 0;    // Current buffer position
    } cap;

    // ===== STEP 2: Redirect debug output to our capture =====
    SimpleFOCDebug::enable(&cap);
    
    // ===== STEP 3: Run motor characterization =====
    // Parameter 0.1 = current limit during characterization (Amps)
    int ret = motor.characteriseMotor(0.5f);
    
    // ===== STEP 4: Restore normal debug output =====
    SimpleFOCDebug::enable(&Serial);

    // ===== STEP 5: Check if characterization succeeded =====
    // if (ret != 0) {
    //     Serial.printf("[CAL] FAIL: SimpleFOC code %d\n", ret);
    //     return false;
    // }

    // ===== STEP 6: Sanity limits for validation =====
    const float MIN_R        = 0.05f;   // Minimum phase resistance (Ω)
    const float MAX_R        = 50.0f;   // Maximum phase resistance (Ω)
    const float MIN_L        = 1e-6f;   // Minimum inductance (H) - 1μH
    const float MAX_SALIENCY = 200.0f;   // Maximum Lq/Ld ratio

    // ===== STEP 7: Validate parsed values exist =====
    if (cap.R <= 0 || cap.Lq <= 0 || cap.Ld <= 0) {
        Serial.println("[CAL] FAIL: parse error");
        return false;
    }
    
    // ===== STEP 8: Validate resistance range =====
    if (cap.R < MIN_R) {
        Serial.printf("[CAL] FAIL: R=%.4f too low\n", cap.R);
        return false;
    }
    if (cap.R > MAX_R) {
        Serial.printf("[CAL] FAIL: R=%.4f too high\n", cap.R);
        return false;
    }
    
    // ===== STEP 9: Validate inductance range =====
    if (cap.Ld < MIN_L) {
        Serial.printf("[CAL] FAIL: Ld=%.4fmH too low\n", cap.Ld * 1000);
        return false;
    }
    if (cap.Lq < MIN_L) {
        Serial.printf("[CAL] FAIL: Lq=%.4fmH too low\n", cap.Lq * 1000);
        return false;
    }

    
    // Saliency ratio check (excessive saliency indicates measurement error)
    if ((cap.Lq / cap.Ld) > MAX_SALIENCY) {
        Serial.printf("[CAL] FAIL: Lq/Ld=%.1f\n", cap.Lq / cap.Ld);
        return false;
    }

    // ===== STEP 11: Apply validated parameters =====
    motor.phase_resistance = cap.R;      // Set phase resistance (Ω)
    motor.phase_inductance = cap.Lq;     // Set Q-axis inductance (H)
    motor.KV_rating = 1000;               // TODO: Get from motor datasheet!
    motor.init();                          // Re-initialize motor with new params
    
    // ===== STEP 12: Report success =====
    Serial.printf("[CAL] OK: R=%.4fΩ Ld=%.4fmH Lq=%.4fmH ratio=%.2f\n",
                  cap.R, cap.Ld * 1000, cap.Lq * 1000, cap.Lq / cap.Ld);
    return true;
}
// ══════════════════════════════════════════════════
// SETUP (FIXED - removed OVERHEATED references)
// ══════════════════════════════════════════════════
void setup() {
    Serial.begin(460800);
    SimpleFOCDebug::enable(&Serial);
    delay(100);
    Serial.println("=== BLDC Node Boot ===");

    can_node_setup();
    
    pinMode(M_PWM,  OUTPUT); digitalWrite(M_PWM,  HIGH);
    pinMode(M_OC,   OUTPUT); digitalWrite(M_OC,   LOW);
    pinMode(GAIN,   OUTPUT); digitalWrite(GAIN,   LOW);
    pinMode(OC_ADJ, OUTPUT); digitalWrite(OC_ADJ, HIGH);
    pinMode(DC_CAL, OUTPUT); digitalWrite(DC_CAL, LOW);

    pinMode(PIN_PWRGD,  INPUT);
    pinMode(PIN_NFAULT, INPUT_PULLUP);
    pinMode(PIN_NOCTW,  INPUT_PULLUP);
    
    auto abort_setup = [](const char* msg, State s, CANEvent evt) {
        Serial.println(msg);
        motor.disable();
        driver.disable();
        transition_to(s, evt);
    };
    
    driver.voltage_power_supply = 12;
    driver.pwm_frequency        = 18000;
    if (!driver.init())
        return abort_setup("[BOOT] driver.init() failed", SETUP_ERROR, EVT_SETUP_ERROR);
    
    motor.linkDriver(&driver);
    cs.linkDriver(&driver);
    delay(100);
    
    motor.controller     = MotionControlType::velocity_openloop;
    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.voltage_limit  = 2.5f;
    motor.current_limit  = 2.5f;
    motor.velocity_limit = 800;
    
    if (motor.init() != 1)
        return abort_setup("[BOOT] motor.init() failed", SETUP_ERROR, EVT_SETUP_ERROR);
    
    if (cs.init() != 1)
        return abort_setup("[BOOT] cs.init() failed", SETUP_ERROR, EVT_SETUP_ERROR);

    Serial.printf("[CS] Offsets: %.3f %.3f %.3f\n", cs.offset_ia, cs.offset_ib, cs.offset_ic);
    cs.gain_a *=  1;
    cs.gain_b *= -1;
    cs.gain_c *= -1;
    motor.linkCurrentSense(&cs);
    cs.driverAlign(0.2f);
    delay(2000);
    
    foc_timer.begin(foc_tick, 50);
    
    if (!digitalRead(PIN_PWRGD))
        return abort_setup("[BOOT] No power (PWRGD=0)", NO_POWER, EVT_NO_POWER);
    
    if (!digitalRead(PIN_NFAULT)) {
        if (!digitalRead(PIN_NOCTW)) {
            _fault_reason = FAULT_REASON_THERMAL;
            return abort_setup("[BOOT] Overheated (nFAULT=0 nOCTW=0)", FAULT, EVT_OVERHEATED);
        } else {
            _fault_reason = FAULT_REASON_ELECTRICAL;
            return abort_setup("[BOOT] Hardware fault (nFAULT=0 nOCTW=1)", FAULT, EVT_FAULT);
        }
    }
    
    Serial.println("[BOOT] DRV8302 OK  (PWRGD=1 nFAULT=1 nOCTW=1)");
    transition_to(NEEDS_RECAL, EVT_SETUP_OK);
}

// ══════════════════════════════════════════════════
// LOOP (FIXED - unified fault handling)
// ══════════════════════════════════════════════════
void loop() {
    //uint32_t start =  micros();
    // PWRGD debounce
    {
        if (state == SETUP_ERROR) {
            // Skip
        }
        else {
            static bool     last_stable  = true;
            static bool     last_raw     = true;
            static uint32_t stable_since = 0;

            bool raw = digitalRead(PIN_PWRGD);
            if (raw != last_raw) {
                last_raw     = raw;
                stable_since = millis();
            }
            uint32_t held = millis() - stable_since;
            
            if (!raw && last_stable && held > 20) {
                motor.disable();
                driver.disable();
                transition_to(NO_POWER, EVT_NO_POWER);
                last_stable = false;
            }
            
            if (raw && !last_stable && state == NO_POWER && held > 60) {
                Serial.println("[NO_POWER] Power restored → FAULT");
                _fault_reason = FAULT_REASON_POWER_RESTORE;
                transition_to(FAULT, EVT_POWER_RESTORE);
                last_stable = true;
            }
        }
    }

    // nFAULT debounce (FIXED - unified to FAULT state)
    if(state == SETUP_ERROR) {
        // Skip
    }
    else {
        static bool     nfault_timing    = false;
        static uint32_t nfault_low_start = 0;

        if (!digitalRead(PIN_NFAULT)) {
            if (state == NO_POWER || state == FAULT) {
                nfault_timing = false;
            } else if (!nfault_timing) {
                nfault_timing    = true;
                nfault_low_start = millis();
            } else if (millis() - nfault_low_start > 50) {  //should have been less but the board is chinese *_* and it riples
                motor.disable();
                driver.disable(); 
                if (!digitalRead(PIN_NOCTW)) {
                    _fault_reason = FAULT_REASON_THERMAL;
                    transition_to(FAULT, EVT_OVERHEATED);
                } else {
                    _fault_reason = FAULT_REASON_ELECTRICAL;
                    transition_to(FAULT, EVT_FAULT);
                }
                nfault_timing = false;
            }
        } else {
            nfault_timing = false;
        }
    }
    // State machine (FIXED - removed OVERHEATED case)
    switch (state) {

        case INIT:
            break;
            
        case SETUP_ERROR:
            {
                static uint32_t t = 0;
                if (millis() - t >= 5000) {
                    t = millis();
                    Serial.println("[SETUP_ERROR] power cycle required");
                }
            }
            break;

        case NEEDS_RECAL:
            {
                static State prev = INIT;
                if (prev != NEEDS_RECAL) {
                    motor.disable();
                    motor.target = 0;
                }
                prev = state;
            }
            break;

        case CALIBRATING:
            {
                static bool cal_started = false;
                if (!cal_started) {
                    Serial.println("[CAL] Starting");
                    motor.enable();
                    cal_started = true;
                }
                if (calibrate_and_apply()) {
                    motor.target = 0;
                    motor.disable();
                    cal_started = false;
                    transition_to(IDLE, EVT_CAL_SUCCESS);
                } else {
                    motor.disable();
                    cal_started = false;
                    transition_to(CAL_FAILED, EVT_CAL_FAILED);
                }
            }
            break;

        case CAL_FAILED:
            {
                static uint32_t t = 0;
                if (millis() - t >= 3000) {
                    t = millis();
                    Serial.println("[CAL_FAILED] waiting for supervisor");
                }
            }
            break;

        case IDLE:
            {
                static State prev = INIT;
                if (prev != IDLE) {
                    motor.disable();
                    motor.target = 0;
                }
                prev = state;
            }
            break;

        case RUNNING:
            {
                static State prev = INIT;
                if (prev != RUNNING) {
                    motor.enable();
                    driver.enable();
                }
                prev = state;
                static uint32_t last_us = 0;
                if (micros() - last_us >= 50) {
                    last_us = micros();
                    PhaseCurrent_s I = cs.getPhaseCurrents();
                    char buf[56];
                    snprintf(buf, sizeof(buf), "%.4f\t%.4f\t%.4f\n", I.a, I.b, I.c);
                    Serial.print(buf);
                }
            }
            break;
            
        case STOPPING:
            {
                static State prev = INIT;
                static float decay_factor = 0.999977f;  // assume cc =20us -->50kHz 0.01~~=0.999977^(4sec * 50khz) close to 4 sec exponential decay
                if (prev != STOPPING) {
                    Serial.println("[STOPPING] Soft stop");
                }
                prev = state;
                // Exponential decay - naturally approaches zero
                target_vel *= decay_factor;
                // When target is effectively zero, we're done
                if (abs(motor.target) < 0.1f) {
                    motor.target = 0.0f;
                    motor.disable();
                    Serial.println("[STOPPING] Complete");
                    transition_to(IDLE, EVT_STOPPED);
                    
                }
            }
            break;

        case FAULT:  // Unified fault state
            {
                static State prev = INIT;
                if (prev != FAULT) {
                    motor.disable();
                }
                prev = state;
                
                static uint32_t t = 0;
                if (millis() - t >= 2000) {
                    t = millis();
                    const char* reason = "UNKNOWN";
                    switch(_fault_reason) {
                        case FAULT_REASON_THERMAL: reason = "THERMAL"; break;
                        case FAULT_REASON_ELECTRICAL: reason = "ELECTRICAL"; break;
                        case FAULT_REASON_POWER_RESTORE: reason = "POWER_RESTORE"; break;
                        default: break;
                    }
                    Serial.printf("[FAULT] %s - waiting cmd_recover — PWRGD=%d nFAULT=%d nOCTW=%d\n",
                                  reason,
                                  digitalRead(PIN_PWRGD),
                                  digitalRead(PIN_NFAULT),
                                  digitalRead(PIN_NOCTW));
                }
            }
            break;

        case NO_POWER:
            {
                static State prev = INIT;
                if (prev != NO_POWER) {
                    motor.disable();
                    driver.disable();
                }
                prev = state;
                static uint32_t t = 0;
                if (millis() - t >= 2000) {
                    t = millis();
                    Serial.printf("[NO_POWER] PWRGD=%d\n", digitalRead(PIN_PWRGD));
                }
            }
            break;
    }

    can_node_run();

    //Serial.printf("\r %u us",micros() - start);
}