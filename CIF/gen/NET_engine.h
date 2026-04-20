/* Headers for the CIF to C translation of NET.cif
 * Generated file, DO NOT EDIT
 */

#ifndef CIF_C_NET_ENGINE_H
#define CIF_C_NET_ENGINE_H

#include "NET_library.h"

/* Types of the specification.
 * Note that integer ranges are ignored in C.
 */
enum EnumNET_ {
    /** Literal "BOOTING". */
    _NET_BOOTING,

    /** Literal "CAL_FAILED". */
    _NET_CAL_FAILED,

    /** Literal "CALIBRATING". */
    _NET_CALIBRATING,

    /** Literal "ELECTRICAL". */
    _NET_ELECTRICAL,

    /** Literal "EMERGENCY". */
    _NET_EMERGENCY,

    /** Literal "FAULT". */
    _NET_FAULT,

    /** Literal "IDLE". */
    _NET_IDLE,

    /** Literal "INIT". */
    _NET_INIT,

    /** Literal "NEEDS_RECAL". */
    _NET_NEEDS_RECAL,

    /** Literal "NO_POWER". */
    _NET_NO_POWER,

    /** Literal "NONE". */
    _NET_NONE,

    /** Literal "POWER_RESTORE". */
    _NET_POWER_RESTORE,

    /** Literal "RECOVERING". */
    _NET_RECOVERING,

    /** Literal "RUNNING". */
    _NET_RUNNING,

    /** Literal "SETUP_ERROR". */
    _NET_SETUP_ERROR,

    /** Literal "STOPPING". */
    _NET_STOPPING,

    /** Literal "THERMAL". */
    _NET_THERMAL,
};
typedef enum EnumNET_ NETEnum;

extern const char *enum_names[];
extern int EnumTypePrint(NETEnum value, char *dest, int start, int end);


/* Event declarations. */
enum NETEventEnum_ {
    /** Initial step. */
    EVT_INITIAL_,

    /** Delay step. */
    EVT_DELAY_,

    /** Event "Net.coord._.c_all_calibrated". */
    Net_coord___c_all_calibrated_,

    /** Event "Net.coord._.c_all_healthy". */
    Net_coord___c_all_healthy_,

    /** Event "Net.coord._.c_all_power_lost". */
    Net_coord___c_all_power_lost_,

    /** Event "Net.coord._.c_all_stopped". */
    Net_coord___c_all_stopped_,

    /** Event "Net.coord._.c_limits_exceeded". */
    Net_coord___c_limits_exceeded_,

    /** Event "Net.coord._.c_node_unhealthy". */
    Net_coord___c_node_unhealthy_,

    /** Event "Net.coord._.c_nodes_ready". */
    Net_coord___c_nodes_ready_,

    /** Event "Net.coord._.c_power_restored". */
    Net_coord___c_power_restored_,

    /** Event "Net.coord._.u_start". */
    Net_coord___u_start_,

    /** Event "Net.coord._.u_stop". */
    Net_coord___u_stop_,

    /** Event "Net.coord._.u_technician_reset". */
    Net_coord___u_technician_reset_,

    /** Event "Net.NODE1._.c_cal_reject". */
    Net_NODE1___c_cal_reject_,

    /** Event "Net.NODE1._.c_calibrate". */
    Net_NODE1___c_calibrate_,

    /** Event "Net.NODE1._.c_enable". */
    Net_NODE1___c_enable_,

    /** Event "Net.NODE1._.c_reboot". */
    Net_NODE1___c_reboot_,

    /** Event "Net.NODE1._.c_recover". */
    Net_NODE1___c_recover_,

    /** Event "Net.NODE1._.c_set_velocity". */
    Net_NODE1___c_set_velocity_,

    /** Event "Net.NODE1._.c_stop". */
    Net_NODE1___c_stop_,

    /** Event "Net.NODE1._.u_cal_failed". */
    Net_NODE1___u_cal_failed_,

    /** Event "Net.NODE1._.u_cal_success". */
    Net_NODE1___u_cal_success_,

    /** Event "Net.NODE1._.u_cooled". */
    Net_NODE1___u_cooled_,

    /** Event "Net.NODE1._.u_fault". */
    Net_NODE1___u_fault_,

    /** Event "Net.NODE1._.u_fault_cleared". */
    Net_NODE1___u_fault_cleared_,

    /** Event "Net.NODE1._.u_no_power". */
    Net_NODE1___u_no_power_,

    /** Event "Net.NODE1._.u_overheated". */
    Net_NODE1___u_overheated_,

    /** Event "Net.NODE1._.u_power_restore". */
    Net_NODE1___u_power_restore_,

    /** Event "Net.NODE1._.u_recover_failed". */
    Net_NODE1___u_recover_failed_,

    /** Event "Net.NODE1._.u_setup_error". */
    Net_NODE1___u_setup_error_,

    /** Event "Net.NODE1._.u_setup_ok". */
    Net_NODE1___u_setup_ok_,

    /** Event "Net.NODE1._.u_stopped". */
    Net_NODE1___u_stopped_,

    /** Event "Net.NODE2._.c_cal_reject". */
    Net_NODE2___c_cal_reject_,

    /** Event "Net.NODE2._.c_calibrate". */
    Net_NODE2___c_calibrate_,

    /** Event "Net.NODE2._.c_enable". */
    Net_NODE2___c_enable_,

    /** Event "Net.NODE2._.c_reboot". */
    Net_NODE2___c_reboot_,

    /** Event "Net.NODE2._.c_recover". */
    Net_NODE2___c_recover_,

    /** Event "Net.NODE2._.c_set_velocity". */
    Net_NODE2___c_set_velocity_,

    /** Event "Net.NODE2._.c_stop". */
    Net_NODE2___c_stop_,

    /** Event "Net.NODE2._.u_cal_failed". */
    Net_NODE2___u_cal_failed_,

    /** Event "Net.NODE2._.u_cal_success". */
    Net_NODE2___u_cal_success_,

    /** Event "Net.NODE2._.u_cooled". */
    Net_NODE2___u_cooled_,

    /** Event "Net.NODE2._.u_fault". */
    Net_NODE2___u_fault_,

    /** Event "Net.NODE2._.u_fault_cleared". */
    Net_NODE2___u_fault_cleared_,

    /** Event "Net.NODE2._.u_no_power". */
    Net_NODE2___u_no_power_,

    /** Event "Net.NODE2._.u_overheated". */
    Net_NODE2___u_overheated_,

    /** Event "Net.NODE2._.u_power_restore". */
    Net_NODE2___u_power_restore_,

    /** Event "Net.NODE2._.u_recover_failed". */
    Net_NODE2___u_recover_failed_,

    /** Event "Net.NODE2._.u_setup_error". */
    Net_NODE2___u_setup_error_,

    /** Event "Net.NODE2._.u_setup_ok". */
    Net_NODE2___u_setup_ok_,

    /** Event "Net.NODE2._.u_stopped". */
    Net_NODE2___u_stopped_,
};
typedef enum NETEventEnum_ NET_Event_;

/** Names of all the events. */
extern const char *NET_event_names[];

/* Constants. */

/** Constant "MAX_FAULTS". */
extern IntType MAX_FAULTS_;

/** Constant "MAX_POWER". */
extern IntType MAX_POWER_;

/** Constant "MAX_CAL". */
extern IntType MAX_CAL_;

/** Constant "MAX_RECOVERY". */
extern IntType MAX_RECOVERY_;

/** Constant "MAX_REBOOT". */
extern IntType MAX_REBOOT_;

/** Constant "Net.NODE1._.MAX_RANGE". */
extern IntType Net_NODE1___MAX_RANGE_;

/** Constant "Net.NODE2._.MAX_RANGE". */
extern IntType Net_NODE2___MAX_RANGE_;

/* Input variables. */

/** Input variable "bool Net.coord.HMI._u_start". */
extern BoolType Net_coord_HMI__u_start_;

/** Input variable "bool Net.coord.HMI._u_stop". */
extern BoolType Net_coord_HMI__u_stop_;

/** Input variable "bool Net.coord.HMI._u_technician_reset". */
extern BoolType Net_coord_HMI__u_technician_reset_;

/** Input variable "bool Net.NODE1.CAN.Evt_setup_ok". */
extern BoolType Net_NODE1_CAN_Evt_setup_ok_;

/** Input variable "bool Net.NODE1.CAN.Evt_setup_error". */
extern BoolType Net_NODE1_CAN_Evt_setup_error_;

/** Input variable "bool Net.NODE1.CAN.Evt_cal_success". */
extern BoolType Net_NODE1_CAN_Evt_cal_success_;

/** Input variable "bool Net.NODE1.CAN.Evt_cal_failed". */
extern BoolType Net_NODE1_CAN_Evt_cal_failed_;

/** Input variable "bool Net.NODE1.CAN.Evt_stopped". */
extern BoolType Net_NODE1_CAN_Evt_stopped_;

/** Input variable "bool Net.NODE1.CAN.Evt_fault". */
extern BoolType Net_NODE1_CAN_Evt_fault_;

/** Input variable "bool Net.NODE1.CAN.Evt_no_power". */
extern BoolType Net_NODE1_CAN_Evt_no_power_;

/** Input variable "bool Net.NODE1.CAN.Evt_power_restored". */
extern BoolType Net_NODE1_CAN_Evt_power_restored_;

/** Input variable "bool Net.NODE1.CAN.Evt_overheated". */
extern BoolType Net_NODE1_CAN_Evt_overheated_;

/** Input variable "bool Net.NODE1.CAN.Evt_cooled". */
extern BoolType Net_NODE1_CAN_Evt_cooled_;

/** Input variable "bool Net.NODE1.CAN.Evt_fault_cleared". */
extern BoolType Net_NODE1_CAN_Evt_fault_cleared_;

/** Input variable "bool Net.NODE1.CAN.Evt_recover_failed". */
extern BoolType Net_NODE1_CAN_Evt_recover_failed_;

/** Input variable "bool Net.NODE2.CAN.Evt_setup_ok". */
extern BoolType Net_NODE2_CAN_Evt_setup_ok_;

/** Input variable "bool Net.NODE2.CAN.Evt_setup_error". */
extern BoolType Net_NODE2_CAN_Evt_setup_error_;

/** Input variable "bool Net.NODE2.CAN.Evt_cal_success". */
extern BoolType Net_NODE2_CAN_Evt_cal_success_;

/** Input variable "bool Net.NODE2.CAN.Evt_cal_failed". */
extern BoolType Net_NODE2_CAN_Evt_cal_failed_;

/** Input variable "bool Net.NODE2.CAN.Evt_stopped". */
extern BoolType Net_NODE2_CAN_Evt_stopped_;

/** Input variable "bool Net.NODE2.CAN.Evt_fault". */
extern BoolType Net_NODE2_CAN_Evt_fault_;

/** Input variable "bool Net.NODE2.CAN.Evt_no_power". */
extern BoolType Net_NODE2_CAN_Evt_no_power_;

/** Input variable "bool Net.NODE2.CAN.Evt_power_restored". */
extern BoolType Net_NODE2_CAN_Evt_power_restored_;

/** Input variable "bool Net.NODE2.CAN.Evt_overheated". */
extern BoolType Net_NODE2_CAN_Evt_overheated_;

/** Input variable "bool Net.NODE2.CAN.Evt_cooled". */
extern BoolType Net_NODE2_CAN_Evt_cooled_;

/** Input variable "bool Net.NODE2.CAN.Evt_fault_cleared". */
extern BoolType Net_NODE2_CAN_Evt_fault_cleared_;

/** Input variable "bool Net.NODE2.CAN.Evt_recover_failed". */
extern BoolType Net_NODE2_CAN_Evt_recover_failed_;

extern void NET_AssignInputVariables();

/* Declaration of internal functions. */


/* State variables (use for output only). */
extern RealType model_time; /**< Current model time. */

/** Discrete variable "int[0..300] Net.coord._.velocity". */
extern IntType Net_coord___velocity_;

/** Discrete variable "E Net.coord._". */
extern NETEnum Net_coord___;

/** Discrete variable "int[0..10] Net.NODE1._.fault_cnt". */
extern IntType Net_NODE1___fault_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.power_cc_cnt". */
extern IntType Net_NODE1___power_cc_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.recovery_att_cnt". */
extern IntType Net_NODE1___recovery_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.calibration_att_cnt". */
extern IntType Net_NODE1___calibration_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.reboot_cnt". */
extern IntType Net_NODE1___reboot_cnt_;

/** Discrete variable "int[0..300] Net.NODE1._.cur_vel". */
extern IntType Net_NODE1___cur_vel_;

/** Discrete variable "bool Net.NODE1._.rec_flag". */
extern BoolType Net_NODE1___rec_flag_;

/** Discrete variable "E Net.NODE1._.fault_reason". */
extern NETEnum Net_NODE1___fault_reason_;

/** Discrete variable "E Net.NODE1._". */
extern NETEnum Net_NODE1___;

/** Discrete variable "int[0..10] Net.NODE2._.fault_cnt". */
extern IntType Net_NODE2___fault_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.power_cc_cnt". */
extern IntType Net_NODE2___power_cc_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.recovery_att_cnt". */
extern IntType Net_NODE2___recovery_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.calibration_att_cnt". */
extern IntType Net_NODE2___calibration_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.reboot_cnt". */
extern IntType Net_NODE2___reboot_cnt_;

/** Discrete variable "int[0..300] Net.NODE2._.cur_vel". */
extern IntType Net_NODE2___cur_vel_;

/** Discrete variable "bool Net.NODE2._.rec_flag". */
extern BoolType Net_NODE2___rec_flag_;

/** Discrete variable "E Net.NODE2._.fault_reason". */
extern NETEnum Net_NODE2___fault_reason_;

/** Discrete variable "E Net.NODE2._". */
extern NETEnum Net_NODE2___;

/* Algebraic and derivative functions (use for output only). */

static inline BoolType LIMIT_EXHAUSTED_(void);
static inline IntType Net_NODE1_node_id_(void);
static inline IntType Net_NODE1_tar_vel_(void);
static inline IntType Net_NODE2_node_id_(void);
static inline IntType Net_NODE2_tar_vel_(void);
static inline IntType Net_NODE1___tar_vel_(void);
static inline IntType Net_NODE2___tar_vel_(void);



/** Algebraic variable LIMIT_EXHAUSTED = M.Net_NODE1___fault_cnt >= MAX_FAULTS or M.Net_NODE1___power_cc_cnt >= MAX_POWER or M.Net_NODE1___recovery_att_cnt >= MAX_RECOVERY or M.Net_NODE1___calibration_att_cnt >= MAX_CAL or M.Net_NODE1___reboot_cnt >= MAX_REBOOT or M.Net_NODE2___fault_cnt >= MAX_FAULTS or M.Net_NODE2___power_cc_cnt >= MAX_POWER or M.Net_NODE2___recovery_att_cnt >= MAX_RECOVERY or M.Net_NODE2___calibration_att_cnt >= MAX_CAL or M.Net_NODE2___reboot_cnt >= MAX_REBOOT. */
static inline BoolType LIMIT_EXHAUSTED_(void) {
    return ((((((((((Net_NODE1___fault_cnt_) >= (MAX_FAULTS_)) || ((Net_NODE1___power_cc_cnt_) >= (MAX_POWER_))) || ((Net_NODE1___recovery_att_cnt_) >= (MAX_RECOVERY_))) || ((Net_NODE1___calibration_att_cnt_) >= (MAX_CAL_))) || ((Net_NODE1___reboot_cnt_) >= (MAX_REBOOT_))) || ((Net_NODE2___fault_cnt_) >= (MAX_FAULTS_))) || ((Net_NODE2___power_cc_cnt_) >= (MAX_POWER_))) || ((Net_NODE2___recovery_att_cnt_) >= (MAX_RECOVERY_))) || ((Net_NODE2___calibration_att_cnt_) >= (MAX_CAL_))) || ((Net_NODE2___reboot_cnt_) >= (MAX_REBOOT_));
}

/** Algebraic variable Net.NODE1.node_id = 1. */
static inline IntType Net_NODE1_node_id_(void) {
    return 1;
}

/** Algebraic variable Net.NODE1.tar_vel = M.Net_coord___velocity. */
static inline IntType Net_NODE1_tar_vel_(void) {
    return Net_coord___velocity_;
}

/** Algebraic variable Net.NODE2.node_id = 2. */
static inline IntType Net_NODE2_node_id_(void) {
    return 2;
}

/** Algebraic variable Net.NODE2.tar_vel = M.Net_coord___velocity. */
static inline IntType Net_NODE2_tar_vel_(void) {
    return Net_coord___velocity_;
}

/** Algebraic variable Net.NODE1._.tar_vel = Net.NODE1.tar_vel. */
static inline IntType Net_NODE1___tar_vel_(void) {
    return Net_NODE1_tar_vel_();
}

/** Algebraic variable Net.NODE2._.tar_vel = Net.NODE2.tar_vel. */
static inline IntType Net_NODE2___tar_vel_(void) {
    return Net_NODE2_tar_vel_();
}

/* Code entry points. */
void NET_EngineFirstStep(void);
void NET_EngineTimeStep(double delta);

#if EVENT_OUTPUT
/**
 * External callback function reporting about the execution of an event.
 * @param event Event being executed.
 * @param pre If \c TRUE, event is about to be executed. If \c FALSE, event has been executed.
 * @note Function must be implemented externally.
 */
extern void NET_InfoEvent(NET_Event_ event, BoolType pre);
#endif

#if PRINT_OUTPUT
/**
 * External callback function to output the given text-line to the given filename.
 * @param text Text to print (does not have a EOL character).
 * @param fname Name of the file to print to.
 */
extern void NET_PrintOutput(const char *text, const char *fname);
#endif

#endif

