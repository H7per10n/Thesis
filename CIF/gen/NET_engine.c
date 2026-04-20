/* CIF to C translation of NET.cif
 * Generated file, DO NOT EDIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "NET_engine.h"

#ifndef MAX_NUM_ITERS
#define MAX_NUM_ITERS 1000
#endif

/* What to do if a range error is found in an assignment? */
#ifdef KEEP_RUNNING
static inline void RangeErrorDetected(void) { /* Do nothing, error is already reported. */ }
#else
static inline void RangeErrorDetected(void) { exit(1); }
#endif

/* Type support code. */
int EnumTypePrint(NETEnum value, char *dest, int start, int end) {
    int last = end - 1;
    const char *lit_name = enum_names[value];
    while (start < last && *lit_name) {
        dest[start++] = *lit_name;
        lit_name++;
    }
    dest[start] = '\0';
    return start;
}


/** Event names. */
const char *NET_event_names[] = {
    "initial-step",                   /**< Initial step. */
    "delay-step",                     /**< Delay step. */
    "Net.coord._.c_all_calibrated",   /**< Event "Net.coord._.c_all_calibrated". */
    "Net.coord._.c_all_healthy",      /**< Event "Net.coord._.c_all_healthy". */
    "Net.coord._.c_all_power_lost",   /**< Event "Net.coord._.c_all_power_lost". */
    "Net.coord._.c_all_stopped",      /**< Event "Net.coord._.c_all_stopped". */
    "Net.coord._.c_limits_exceeded",  /**< Event "Net.coord._.c_limits_exceeded". */
    "Net.coord._.c_node_unhealthy",   /**< Event "Net.coord._.c_node_unhealthy". */
    "Net.coord._.c_nodes_ready",      /**< Event "Net.coord._.c_nodes_ready". */
    "Net.coord._.c_power_restored",   /**< Event "Net.coord._.c_power_restored". */
    "Net.coord._.u_start",            /**< Event "Net.coord._.u_start". */
    "Net.coord._.u_stop",             /**< Event "Net.coord._.u_stop". */
    "Net.coord._.u_technician_reset", /**< Event "Net.coord._.u_technician_reset". */
    "Net.NODE1._.c_cal_reject",       /**< Event "Net.NODE1._.c_cal_reject". */
    "Net.NODE1._.c_calibrate",        /**< Event "Net.NODE1._.c_calibrate". */
    "Net.NODE1._.c_enable",           /**< Event "Net.NODE1._.c_enable". */
    "Net.NODE1._.c_reboot",           /**< Event "Net.NODE1._.c_reboot". */
    "Net.NODE1._.c_recover",          /**< Event "Net.NODE1._.c_recover". */
    "Net.NODE1._.c_set_velocity",     /**< Event "Net.NODE1._.c_set_velocity". */
    "Net.NODE1._.c_stop",             /**< Event "Net.NODE1._.c_stop". */
    "Net.NODE1._.u_cal_failed",       /**< Event "Net.NODE1._.u_cal_failed". */
    "Net.NODE1._.u_cal_success",      /**< Event "Net.NODE1._.u_cal_success". */
    "Net.NODE1._.u_cooled",           /**< Event "Net.NODE1._.u_cooled". */
    "Net.NODE1._.u_fault",            /**< Event "Net.NODE1._.u_fault". */
    "Net.NODE1._.u_fault_cleared",    /**< Event "Net.NODE1._.u_fault_cleared". */
    "Net.NODE1._.u_no_power",         /**< Event "Net.NODE1._.u_no_power". */
    "Net.NODE1._.u_overheated",       /**< Event "Net.NODE1._.u_overheated". */
    "Net.NODE1._.u_power_restore",    /**< Event "Net.NODE1._.u_power_restore". */
    "Net.NODE1._.u_recover_failed",   /**< Event "Net.NODE1._.u_recover_failed". */
    "Net.NODE1._.u_setup_error",      /**< Event "Net.NODE1._.u_setup_error". */
    "Net.NODE1._.u_setup_ok",         /**< Event "Net.NODE1._.u_setup_ok". */
    "Net.NODE1._.u_stopped",          /**< Event "Net.NODE1._.u_stopped". */
    "Net.NODE2._.c_cal_reject",       /**< Event "Net.NODE2._.c_cal_reject". */
    "Net.NODE2._.c_calibrate",        /**< Event "Net.NODE2._.c_calibrate". */
    "Net.NODE2._.c_enable",           /**< Event "Net.NODE2._.c_enable". */
    "Net.NODE2._.c_reboot",           /**< Event "Net.NODE2._.c_reboot". */
    "Net.NODE2._.c_recover",          /**< Event "Net.NODE2._.c_recover". */
    "Net.NODE2._.c_set_velocity",     /**< Event "Net.NODE2._.c_set_velocity". */
    "Net.NODE2._.c_stop",             /**< Event "Net.NODE2._.c_stop". */
    "Net.NODE2._.u_cal_failed",       /**< Event "Net.NODE2._.u_cal_failed". */
    "Net.NODE2._.u_cal_success",      /**< Event "Net.NODE2._.u_cal_success". */
    "Net.NODE2._.u_cooled",           /**< Event "Net.NODE2._.u_cooled". */
    "Net.NODE2._.u_fault",            /**< Event "Net.NODE2._.u_fault". */
    "Net.NODE2._.u_fault_cleared",    /**< Event "Net.NODE2._.u_fault_cleared". */
    "Net.NODE2._.u_no_power",         /**< Event "Net.NODE2._.u_no_power". */
    "Net.NODE2._.u_overheated",       /**< Event "Net.NODE2._.u_overheated". */
    "Net.NODE2._.u_power_restore",    /**< Event "Net.NODE2._.u_power_restore". */
    "Net.NODE2._.u_recover_failed",   /**< Event "Net.NODE2._.u_recover_failed". */
    "Net.NODE2._.u_setup_error",      /**< Event "Net.NODE2._.u_setup_error". */
    "Net.NODE2._.u_setup_ok",         /**< Event "Net.NODE2._.u_setup_ok". */
    "Net.NODE2._.u_stopped",          /**< Event "Net.NODE2._.u_stopped". */
};

/** Enumeration names. */
const char *enum_names[] = {
    /** Literal "BOOTING". */
    "BOOTING",

    /** Literal "CAL_FAILED". */
    "CAL_FAILED",

    /** Literal "CALIBRATING". */
    "CALIBRATING",

    /** Literal "ELECTRICAL". */
    "ELECTRICAL",

    /** Literal "EMERGENCY". */
    "EMERGENCY",

    /** Literal "FAULT". */
    "FAULT",

    /** Literal "IDLE". */
    "IDLE",

    /** Literal "INIT". */
    "INIT",

    /** Literal "NEEDS_RECAL". */
    "NEEDS_RECAL",

    /** Literal "NO_POWER". */
    "NO_POWER",

    /** Literal "NONE". */
    "NONE",

    /** Literal "POWER_RESTORE". */
    "POWER_RESTORE",

    /** Literal "RECOVERING". */
    "RECOVERING",

    /** Literal "RUNNING". */
    "RUNNING",

    /** Literal "SETUP_ERROR". */
    "SETUP_ERROR",

    /** Literal "STOPPING". */
    "STOPPING",

    /** Literal "THERMAL". */
    "THERMAL",
};

/* Constants. */

/** Constant "MAX_FAULTS". */
IntType MAX_FAULTS_;

/** Constant "MAX_POWER". */
IntType MAX_POWER_;

/** Constant "MAX_CAL". */
IntType MAX_CAL_;

/** Constant "MAX_RECOVERY". */
IntType MAX_RECOVERY_;

/** Constant "MAX_REBOOT". */
IntType MAX_REBOOT_;

/** Constant "Net.NODE1._.MAX_RANGE". */
IntType Net_NODE1___MAX_RANGE_;

/** Constant "Net.NODE2._.MAX_RANGE". */
IntType Net_NODE2___MAX_RANGE_;

/* Functions. */


/* Input variables. */

/** Input variable "bool Net.coord.HMI._u_start". */
BoolType Net_coord_HMI__u_start_;

/** Input variable "bool Net.coord.HMI._u_stop". */
BoolType Net_coord_HMI__u_stop_;

/** Input variable "bool Net.coord.HMI._u_technician_reset". */
BoolType Net_coord_HMI__u_technician_reset_;

/** Input variable "bool Net.NODE1.CAN.Evt_setup_ok". */
BoolType Net_NODE1_CAN_Evt_setup_ok_;

/** Input variable "bool Net.NODE1.CAN.Evt_setup_error". */
BoolType Net_NODE1_CAN_Evt_setup_error_;

/** Input variable "bool Net.NODE1.CAN.Evt_cal_success". */
BoolType Net_NODE1_CAN_Evt_cal_success_;

/** Input variable "bool Net.NODE1.CAN.Evt_cal_failed". */
BoolType Net_NODE1_CAN_Evt_cal_failed_;

/** Input variable "bool Net.NODE1.CAN.Evt_stopped". */
BoolType Net_NODE1_CAN_Evt_stopped_;

/** Input variable "bool Net.NODE1.CAN.Evt_fault". */
BoolType Net_NODE1_CAN_Evt_fault_;

/** Input variable "bool Net.NODE1.CAN.Evt_no_power". */
BoolType Net_NODE1_CAN_Evt_no_power_;

/** Input variable "bool Net.NODE1.CAN.Evt_power_restored". */
BoolType Net_NODE1_CAN_Evt_power_restored_;

/** Input variable "bool Net.NODE1.CAN.Evt_overheated". */
BoolType Net_NODE1_CAN_Evt_overheated_;

/** Input variable "bool Net.NODE1.CAN.Evt_cooled". */
BoolType Net_NODE1_CAN_Evt_cooled_;

/** Input variable "bool Net.NODE1.CAN.Evt_fault_cleared". */
BoolType Net_NODE1_CAN_Evt_fault_cleared_;

/** Input variable "bool Net.NODE1.CAN.Evt_recover_failed". */
BoolType Net_NODE1_CAN_Evt_recover_failed_;

/** Input variable "bool Net.NODE2.CAN.Evt_setup_ok". */
BoolType Net_NODE2_CAN_Evt_setup_ok_;

/** Input variable "bool Net.NODE2.CAN.Evt_setup_error". */
BoolType Net_NODE2_CAN_Evt_setup_error_;

/** Input variable "bool Net.NODE2.CAN.Evt_cal_success". */
BoolType Net_NODE2_CAN_Evt_cal_success_;

/** Input variable "bool Net.NODE2.CAN.Evt_cal_failed". */
BoolType Net_NODE2_CAN_Evt_cal_failed_;

/** Input variable "bool Net.NODE2.CAN.Evt_stopped". */
BoolType Net_NODE2_CAN_Evt_stopped_;

/** Input variable "bool Net.NODE2.CAN.Evt_fault". */
BoolType Net_NODE2_CAN_Evt_fault_;

/** Input variable "bool Net.NODE2.CAN.Evt_no_power". */
BoolType Net_NODE2_CAN_Evt_no_power_;

/** Input variable "bool Net.NODE2.CAN.Evt_power_restored". */
BoolType Net_NODE2_CAN_Evt_power_restored_;

/** Input variable "bool Net.NODE2.CAN.Evt_overheated". */
BoolType Net_NODE2_CAN_Evt_overheated_;

/** Input variable "bool Net.NODE2.CAN.Evt_cooled". */
BoolType Net_NODE2_CAN_Evt_cooled_;

/** Input variable "bool Net.NODE2.CAN.Evt_fault_cleared". */
BoolType Net_NODE2_CAN_Evt_fault_cleared_;

/** Input variable "bool Net.NODE2.CAN.Evt_recover_failed". */
BoolType Net_NODE2_CAN_Evt_recover_failed_;

/* State variables. */

/** Discrete variable "int[0..300] Net.coord._.velocity". */
IntType Net_coord___velocity_;

/** Discrete variable "E Net.coord._". */
NETEnum Net_coord___;

/** Discrete variable "int[0..10] Net.NODE1._.fault_cnt". */
IntType Net_NODE1___fault_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.power_cc_cnt". */
IntType Net_NODE1___power_cc_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.recovery_att_cnt". */
IntType Net_NODE1___recovery_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.calibration_att_cnt". */
IntType Net_NODE1___calibration_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE1._.reboot_cnt". */
IntType Net_NODE1___reboot_cnt_;

/** Discrete variable "int[0..300] Net.NODE1._.cur_vel". */
IntType Net_NODE1___cur_vel_;

/** Discrete variable "bool Net.NODE1._.rec_flag". */
BoolType Net_NODE1___rec_flag_;

/** Discrete variable "E Net.NODE1._.fault_reason". */
NETEnum Net_NODE1___fault_reason_;

/** Discrete variable "E Net.NODE1._". */
NETEnum Net_NODE1___;

/** Discrete variable "int[0..10] Net.NODE2._.fault_cnt". */
IntType Net_NODE2___fault_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.power_cc_cnt". */
IntType Net_NODE2___power_cc_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.recovery_att_cnt". */
IntType Net_NODE2___recovery_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.calibration_att_cnt". */
IntType Net_NODE2___calibration_att_cnt_;

/** Discrete variable "int[0..10] Net.NODE2._.reboot_cnt". */
IntType Net_NODE2___reboot_cnt_;

/** Discrete variable "int[0..300] Net.NODE2._.cur_vel". */
IntType Net_NODE2___cur_vel_;

/** Discrete variable "bool Net.NODE2._.rec_flag". */
BoolType Net_NODE2___rec_flag_;

/** Discrete variable "E Net.NODE2._.fault_reason". */
NETEnum Net_NODE2___fault_reason_;

/** Discrete variable "E Net.NODE2._". */
NETEnum Net_NODE2___;

RealType model_time; /**< Current model time. */

/** Initialize constants. */
static void InitConstants(void) {
    MAX_FAULTS_ = 5;
    MAX_POWER_ = 3;
    MAX_CAL_ = 3;
    MAX_RECOVERY_ = 3;
    MAX_REBOOT_ = 3;
    Net_NODE1___MAX_RANGE_ = 10;
    Net_NODE2___MAX_RANGE_ = 10;
}

/** Print function. */
#if PRINT_OUTPUT
static void PrintOutput(NET_Event_ event, BoolType pre) {
}
#endif

/* Edge execution code. */

/**
 * Execute code for edge with index 0 and event "Net.coord._.u_start".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge0(void) {
    BoolType guard = ((Net_coord___) == (_NET_IDLE)) && ((Net_coord_HMI__u_start_) && (!(Net_coord_HMI__u_stop_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___u_start_, TRUE);
    #endif

    Net_coord___ = _NET_BOOTING;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___u_start_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 1 and event "Net.coord._.u_stop".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge1(void) {
    BoolType guard = (((Net_coord___) == (_NET_BOOTING)) || (((Net_coord___) == (_NET_CALIBRATING)) || ((Net_coord___) == (_NET_RUNNING)))) && ((Net_coord_HMI__u_stop_) && (!(Net_coord_HMI__u_start_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___u_stop_, TRUE);
    #endif

    if ((Net_coord___) == (_NET_BOOTING)) {
        Net_coord___ = _NET_IDLE;
    } else if ((Net_coord___) == (_NET_CALIBRATING)) {
        Net_coord___ = _NET_STOPPING;
    } else if ((Net_coord___) == (_NET_RUNNING)) {
        Net_coord___ = _NET_STOPPING;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___u_stop_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 2 and event "Net.coord._.u_technician_reset".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge2(void) {
    BoolType guard = (((Net_coord___) == (_NET_EMERGENCY)) && (Net_coord_HMI__u_technician_reset_)) && ((((((Net_NODE1___) == (_NET_INIT)) || ((Net_NODE1___) == (_NET_SETUP_ERROR))) || (((Net_NODE1___) == (_NET_NEEDS_RECAL)) || (((Net_NODE1___) == (_NET_CALIBRATING)) || ((Net_NODE1___) == (_NET_CAL_FAILED))))) || ((((Net_NODE1___) == (_NET_IDLE)) || ((Net_NODE1___) == (_NET_RUNNING))) || (((Net_NODE1___) == (_NET_STOPPING)) || (((Net_NODE1___) == (_NET_FAULT)) || ((Net_NODE1___) == (_NET_NO_POWER)))))) && (((((Net_NODE2___) == (_NET_INIT)) || ((Net_NODE2___) == (_NET_SETUP_ERROR))) || (((Net_NODE2___) == (_NET_NEEDS_RECAL)) || (((Net_NODE2___) == (_NET_CALIBRATING)) || ((Net_NODE2___) == (_NET_CAL_FAILED))))) || ((((Net_NODE2___) == (_NET_IDLE)) || ((Net_NODE2___) == (_NET_RUNNING))) || (((Net_NODE2___) == (_NET_STOPPING)) || (((Net_NODE2___) == (_NET_FAULT)) || ((Net_NODE2___) == (_NET_NO_POWER)))))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___u_technician_reset_, TRUE);
    #endif

    Net_coord___ = _NET_BOOTING;
    if ((Net_NODE1___) == (_NET_INIT)) {
        Net_NODE1___ = _NET_INIT;
    } else if ((Net_NODE1___) == (_NET_SETUP_ERROR)) {
        Net_NODE1___fault_cnt_ = 0;
        Net_NODE1___recovery_att_cnt_ = 0;
        Net_NODE1___calibration_att_cnt_ = 0;
        Net_NODE1___power_cc_cnt_ = 0;
        Net_NODE1___reboot_cnt_ = 0;
    } else if ((Net_NODE1___) == (_NET_NEEDS_RECAL)) {
        Net_NODE1___fault_cnt_ = 0;
        Net_NODE1___recovery_att_cnt_ = 0;
        Net_NODE1___calibration_att_cnt_ = 0;
        Net_NODE1___power_cc_cnt_ = 0;
        Net_NODE1___reboot_cnt_ = 0;
    } else if ((Net_NODE1___) == (_NET_CALIBRATING)) {
        Net_NODE1___ = _NET_CALIBRATING;
    } else if ((Net_NODE1___) == (_NET_CAL_FAILED)) {
        Net_NODE1___fault_cnt_ = 0;
        Net_NODE1___recovery_att_cnt_ = 0;
        Net_NODE1___calibration_att_cnt_ = 0;
        Net_NODE1___power_cc_cnt_ = 0;
        Net_NODE1___reboot_cnt_ = 0;
    } else if ((Net_NODE1___) == (_NET_IDLE)) {
        Net_NODE1___ = _NET_IDLE;
    } else if ((Net_NODE1___) == (_NET_RUNNING)) {
        Net_NODE1___ = _NET_RUNNING;
    } else if ((Net_NODE1___) == (_NET_STOPPING)) {
        Net_NODE1___ = _NET_STOPPING;
    } else if ((Net_NODE1___) == (_NET_FAULT)) {
        Net_NODE1___fault_cnt_ = 0;
        Net_NODE1___recovery_att_cnt_ = 0;
        Net_NODE1___calibration_att_cnt_ = 0;
        Net_NODE1___power_cc_cnt_ = 0;
        Net_NODE1___reboot_cnt_ = 0;
    } else if ((Net_NODE1___) == (_NET_NO_POWER)) {
        Net_NODE1___ = _NET_NO_POWER;
    }
    if ((Net_NODE2___) == (_NET_INIT)) {
        Net_NODE2___ = _NET_INIT;
    } else if ((Net_NODE2___) == (_NET_SETUP_ERROR)) {
        Net_NODE2___fault_cnt_ = 0;
        Net_NODE2___recovery_att_cnt_ = 0;
        Net_NODE2___calibration_att_cnt_ = 0;
        Net_NODE2___power_cc_cnt_ = 0;
        Net_NODE2___reboot_cnt_ = 0;
    } else if ((Net_NODE2___) == (_NET_NEEDS_RECAL)) {
        Net_NODE2___fault_cnt_ = 0;
        Net_NODE2___recovery_att_cnt_ = 0;
        Net_NODE2___calibration_att_cnt_ = 0;
        Net_NODE2___power_cc_cnt_ = 0;
        Net_NODE2___reboot_cnt_ = 0;
    } else if ((Net_NODE2___) == (_NET_CALIBRATING)) {
        Net_NODE2___ = _NET_CALIBRATING;
    } else if ((Net_NODE2___) == (_NET_CAL_FAILED)) {
        Net_NODE2___fault_cnt_ = 0;
        Net_NODE2___recovery_att_cnt_ = 0;
        Net_NODE2___calibration_att_cnt_ = 0;
        Net_NODE2___power_cc_cnt_ = 0;
        Net_NODE2___reboot_cnt_ = 0;
    } else if ((Net_NODE2___) == (_NET_IDLE)) {
        Net_NODE2___ = _NET_IDLE;
    } else if ((Net_NODE2___) == (_NET_RUNNING)) {
        Net_NODE2___ = _NET_RUNNING;
    } else if ((Net_NODE2___) == (_NET_STOPPING)) {
        Net_NODE2___ = _NET_STOPPING;
    } else if ((Net_NODE2___) == (_NET_FAULT)) {
        Net_NODE2___fault_cnt_ = 0;
        Net_NODE2___recovery_att_cnt_ = 0;
        Net_NODE2___calibration_att_cnt_ = 0;
        Net_NODE2___power_cc_cnt_ = 0;
        Net_NODE2___reboot_cnt_ = 0;
    } else if ((Net_NODE2___) == (_NET_NO_POWER)) {
        Net_NODE2___ = _NET_NO_POWER;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___u_technician_reset_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 3 and event "Net.NODE1._.u_cal_failed".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge3(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_CALIBRATING)) && (Net_NODE1_CAN_Evt_cal_failed_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_cal_failed_, TRUE);
    #endif

    Net_NODE1___calibration_att_cnt_ = IntegerMin((Net_NODE1___calibration_att_cnt_) + (1), Net_NODE1___MAX_RANGE_);
    Net_NODE1___ = _NET_CAL_FAILED;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_cal_failed_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 4 and event "Net.NODE1._.u_cal_success".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge4(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_CALIBRATING)) && (Net_NODE1_CAN_Evt_cal_success_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_cal_success_, TRUE);
    #endif

    Net_NODE1___calibration_att_cnt_ = 0;
    Net_NODE1___ = _NET_IDLE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_cal_success_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 5 and event "Net.NODE1._.u_cooled".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge5(void) {
    BoolType guard = (((Net_NODE1___) == (_NET_FAULT)) && ((Net_NODE1___fault_reason_) == (_NET_THERMAL))) && ((Net_NODE1_CAN_Evt_cooled_) && (!(Net_NODE1_CAN_Evt_overheated_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_cooled_, TRUE);
    #endif

    Net_NODE1___recovery_att_cnt_ = 0;
    Net_NODE1___fault_reason_ = _NET_NONE;
    Net_NODE1___rec_flag_ = TRUE;
    Net_NODE1___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_cooled_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 6 and event "Net.NODE1._.u_fault".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge6(void) {
    BoolType guard = ((((Net_NODE1___) == (_NET_INIT)) || (((Net_NODE1___) == (_NET_NEEDS_RECAL)) || ((Net_NODE1___) == (_NET_CALIBRATING)))) || ((((Net_NODE1___) == (_NET_CAL_FAILED)) || ((Net_NODE1___) == (_NET_IDLE))) || (((Net_NODE1___) == (_NET_RUNNING)) || ((Net_NODE1___) == (_NET_STOPPING))))) && (Net_NODE1_CAN_Evt_fault_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_fault_, TRUE);
    #endif

    if ((Net_NODE1___) == (_NET_INIT)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_NEEDS_RECAL)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_CALIBRATING)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_CAL_FAILED)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_IDLE)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_RUNNING)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___cur_vel_ = 0;
        Net_NODE1___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_STOPPING)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___cur_vel_ = 0;
        Net_NODE1___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE1___ = _NET_FAULT;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_fault_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 7 and event "Net.NODE1._.u_fault_cleared".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge7(void) {
    BoolType guard = (((Net_NODE1___) == (_NET_FAULT)) && (((Net_NODE1___fault_reason_) == (_NET_ELECTRICAL)) || ((Net_NODE1___fault_reason_) == (_NET_POWER_RESTORE)))) && ((Net_NODE1_CAN_Evt_fault_cleared_) && (!(Net_NODE1_CAN_Evt_fault_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_fault_cleared_, TRUE);
    #endif

    Net_NODE1___recovery_att_cnt_ = 0;
    Net_NODE1___fault_reason_ = _NET_NONE;
    Net_NODE1___rec_flag_ = TRUE;
    Net_NODE1___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_fault_cleared_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 8 and event "Net.NODE1._.u_no_power".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge8(void) {
    BoolType guard = (((((Net_NODE1___) == (_NET_INIT)) || ((Net_NODE1___) == (_NET_NEEDS_RECAL))) || (((Net_NODE1___) == (_NET_CALIBRATING)) || ((Net_NODE1___) == (_NET_CAL_FAILED)))) || ((((Net_NODE1___) == (_NET_IDLE)) || ((Net_NODE1___) == (_NET_RUNNING))) || (((Net_NODE1___) == (_NET_STOPPING)) || ((Net_NODE1___) == (_NET_FAULT))))) && (Net_NODE1_CAN_Evt_no_power_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_no_power_, TRUE);
    #endif

    if ((Net_NODE1___) == (_NET_INIT)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___ = _NET_NO_POWER;
    } else if ((Net_NODE1___) == (_NET_NEEDS_RECAL)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___ = _NET_NO_POWER;
    } else if ((Net_NODE1___) == (_NET_CALIBRATING)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___ = _NET_NO_POWER;
    } else if ((Net_NODE1___) == (_NET_CAL_FAILED)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___ = _NET_NO_POWER;
    } else if ((Net_NODE1___) == (_NET_IDLE)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___ = _NET_NO_POWER;
    } else if ((Net_NODE1___) == (_NET_RUNNING)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___cur_vel_ = 0;
        Net_NODE1___ = _NET_NO_POWER;
    } else if ((Net_NODE1___) == (_NET_STOPPING)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___cur_vel_ = 0;
        Net_NODE1___ = _NET_NO_POWER;
    } else if ((Net_NODE1___) == (_NET_FAULT)) {
        Net_NODE1___power_cc_cnt_ = IntegerMin((Net_NODE1___power_cc_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___rec_flag_ = TRUE;
        Net_NODE1___fault_reason_ = _NET_NONE;
        Net_NODE1___ = _NET_NO_POWER;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_no_power_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 9 and event "Net.NODE1._.u_overheated".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge9(void) {
    BoolType guard = ((((Net_NODE1___) == (_NET_INIT)) || (((Net_NODE1___) == (_NET_NEEDS_RECAL)) || ((Net_NODE1___) == (_NET_CALIBRATING)))) || ((((Net_NODE1___) == (_NET_CAL_FAILED)) || ((Net_NODE1___) == (_NET_IDLE))) || (((Net_NODE1___) == (_NET_RUNNING)) || ((Net_NODE1___) == (_NET_STOPPING))))) && (Net_NODE1_CAN_Evt_overheated_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_overheated_, TRUE);
    #endif

    if ((Net_NODE1___) == (_NET_INIT)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_THERMAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_NEEDS_RECAL)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_THERMAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_CALIBRATING)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_THERMAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_CAL_FAILED)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_THERMAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_IDLE)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___fault_reason_ = _NET_THERMAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_RUNNING)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___cur_vel_ = 0;
        Net_NODE1___fault_reason_ = _NET_THERMAL;
        Net_NODE1___ = _NET_FAULT;
    } else if ((Net_NODE1___) == (_NET_STOPPING)) {
        Net_NODE1___fault_cnt_ = IntegerMin((Net_NODE1___fault_cnt_) + (1), Net_NODE1___MAX_RANGE_);
        Net_NODE1___cur_vel_ = 0;
        Net_NODE1___fault_reason_ = _NET_THERMAL;
        Net_NODE1___ = _NET_FAULT;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_overheated_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 10 and event "Net.NODE1._.u_power_restore".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge10(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_NO_POWER)) && ((Net_NODE1_CAN_Evt_power_restored_) && (!(Net_NODE1_CAN_Evt_no_power_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_power_restore_, TRUE);
    #endif

    Net_NODE1___fault_reason_ = _NET_POWER_RESTORE;
    Net_NODE1___ = _NET_FAULT;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_power_restore_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 11 and event "Net.NODE1._.u_recover_failed".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge11(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_FAULT)) && ((!(Net_NODE1___rec_flag_)) && (Net_NODE1_CAN_Evt_recover_failed_));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_recover_failed_, TRUE);
    #endif

    Net_NODE1___recovery_att_cnt_ = IntegerMin((Net_NODE1___recovery_att_cnt_) + (1), Net_NODE1___MAX_RANGE_);
    Net_NODE1___rec_flag_ = !(Net_NODE1___rec_flag_);

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_recover_failed_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 12 and event "Net.NODE1._.u_setup_error".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge12(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_INIT)) && (Net_NODE1_CAN_Evt_setup_error_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_setup_error_, TRUE);
    #endif

    Net_NODE1___ = _NET_SETUP_ERROR;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_setup_error_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 13 and event "Net.NODE1._.u_setup_ok".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge13(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_INIT)) && (Net_NODE1_CAN_Evt_setup_ok_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_setup_ok_, TRUE);
    #endif

    Net_NODE1___reboot_cnt_ = 0;
    Net_NODE1___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_setup_ok_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 14 and event "Net.NODE1._.u_stopped".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge14(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_STOPPING)) && (Net_NODE1_CAN_Evt_stopped_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_stopped_, TRUE);
    #endif

    Net_NODE1___cur_vel_ = 0;
    Net_NODE1___ = _NET_IDLE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___u_stopped_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 15 and event "Net.NODE2._.u_cal_failed".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge15(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_CALIBRATING)) && (Net_NODE2_CAN_Evt_cal_failed_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_cal_failed_, TRUE);
    #endif

    Net_NODE2___calibration_att_cnt_ = IntegerMin((Net_NODE2___calibration_att_cnt_) + (1), Net_NODE2___MAX_RANGE_);
    Net_NODE2___ = _NET_CAL_FAILED;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_cal_failed_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 16 and event "Net.NODE2._.u_cal_success".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge16(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_CALIBRATING)) && (Net_NODE2_CAN_Evt_cal_success_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_cal_success_, TRUE);
    #endif

    Net_NODE2___calibration_att_cnt_ = 0;
    Net_NODE2___ = _NET_IDLE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_cal_success_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 17 and event "Net.NODE2._.u_cooled".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge17(void) {
    BoolType guard = (((Net_NODE2___) == (_NET_FAULT)) && ((Net_NODE2___fault_reason_) == (_NET_THERMAL))) && ((Net_NODE2_CAN_Evt_cooled_) && (!(Net_NODE2_CAN_Evt_overheated_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_cooled_, TRUE);
    #endif

    Net_NODE2___recovery_att_cnt_ = 0;
    Net_NODE2___fault_reason_ = _NET_NONE;
    Net_NODE2___rec_flag_ = TRUE;
    Net_NODE2___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_cooled_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 18 and event "Net.NODE2._.u_fault".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge18(void) {
    BoolType guard = ((((Net_NODE2___) == (_NET_INIT)) || (((Net_NODE2___) == (_NET_NEEDS_RECAL)) || ((Net_NODE2___) == (_NET_CALIBRATING)))) || ((((Net_NODE2___) == (_NET_CAL_FAILED)) || ((Net_NODE2___) == (_NET_IDLE))) || (((Net_NODE2___) == (_NET_RUNNING)) || ((Net_NODE2___) == (_NET_STOPPING))))) && (Net_NODE2_CAN_Evt_fault_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_fault_, TRUE);
    #endif

    if ((Net_NODE2___) == (_NET_INIT)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_NEEDS_RECAL)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_CALIBRATING)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_CAL_FAILED)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_IDLE)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_RUNNING)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___cur_vel_ = 0;
        Net_NODE2___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_STOPPING)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___cur_vel_ = 0;
        Net_NODE2___fault_reason_ = _NET_ELECTRICAL;
        Net_NODE2___ = _NET_FAULT;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_fault_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 19 and event "Net.NODE2._.u_fault_cleared".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge19(void) {
    BoolType guard = (((Net_NODE2___) == (_NET_FAULT)) && (((Net_NODE2___fault_reason_) == (_NET_ELECTRICAL)) || ((Net_NODE2___fault_reason_) == (_NET_POWER_RESTORE)))) && ((Net_NODE2_CAN_Evt_fault_cleared_) && (!(Net_NODE2_CAN_Evt_fault_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_fault_cleared_, TRUE);
    #endif

    Net_NODE2___recovery_att_cnt_ = 0;
    Net_NODE2___fault_reason_ = _NET_NONE;
    Net_NODE2___rec_flag_ = TRUE;
    Net_NODE2___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_fault_cleared_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 20 and event "Net.NODE2._.u_no_power".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge20(void) {
    BoolType guard = (((((Net_NODE2___) == (_NET_INIT)) || ((Net_NODE2___) == (_NET_NEEDS_RECAL))) || (((Net_NODE2___) == (_NET_CALIBRATING)) || ((Net_NODE2___) == (_NET_CAL_FAILED)))) || ((((Net_NODE2___) == (_NET_IDLE)) || ((Net_NODE2___) == (_NET_RUNNING))) || (((Net_NODE2___) == (_NET_STOPPING)) || ((Net_NODE2___) == (_NET_FAULT))))) && (Net_NODE2_CAN_Evt_no_power_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_no_power_, TRUE);
    #endif

    if ((Net_NODE2___) == (_NET_INIT)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___ = _NET_NO_POWER;
    } else if ((Net_NODE2___) == (_NET_NEEDS_RECAL)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___ = _NET_NO_POWER;
    } else if ((Net_NODE2___) == (_NET_CALIBRATING)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___ = _NET_NO_POWER;
    } else if ((Net_NODE2___) == (_NET_CAL_FAILED)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___ = _NET_NO_POWER;
    } else if ((Net_NODE2___) == (_NET_IDLE)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___ = _NET_NO_POWER;
    } else if ((Net_NODE2___) == (_NET_RUNNING)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___cur_vel_ = 0;
        Net_NODE2___ = _NET_NO_POWER;
    } else if ((Net_NODE2___) == (_NET_STOPPING)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___cur_vel_ = 0;
        Net_NODE2___ = _NET_NO_POWER;
    } else if ((Net_NODE2___) == (_NET_FAULT)) {
        Net_NODE2___power_cc_cnt_ = IntegerMin((Net_NODE2___power_cc_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___rec_flag_ = TRUE;
        Net_NODE2___fault_reason_ = _NET_NONE;
        Net_NODE2___ = _NET_NO_POWER;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_no_power_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 21 and event "Net.NODE2._.u_overheated".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge21(void) {
    BoolType guard = ((((Net_NODE2___) == (_NET_INIT)) || (((Net_NODE2___) == (_NET_NEEDS_RECAL)) || ((Net_NODE2___) == (_NET_CALIBRATING)))) || ((((Net_NODE2___) == (_NET_CAL_FAILED)) || ((Net_NODE2___) == (_NET_IDLE))) || (((Net_NODE2___) == (_NET_RUNNING)) || ((Net_NODE2___) == (_NET_STOPPING))))) && (Net_NODE2_CAN_Evt_overheated_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_overheated_, TRUE);
    #endif

    if ((Net_NODE2___) == (_NET_INIT)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_THERMAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_NEEDS_RECAL)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_THERMAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_CALIBRATING)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_THERMAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_CAL_FAILED)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_THERMAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_IDLE)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___fault_reason_ = _NET_THERMAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_RUNNING)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___cur_vel_ = 0;
        Net_NODE2___fault_reason_ = _NET_THERMAL;
        Net_NODE2___ = _NET_FAULT;
    } else if ((Net_NODE2___) == (_NET_STOPPING)) {
        Net_NODE2___fault_cnt_ = IntegerMin((Net_NODE2___fault_cnt_) + (1), Net_NODE2___MAX_RANGE_);
        Net_NODE2___cur_vel_ = 0;
        Net_NODE2___fault_reason_ = _NET_THERMAL;
        Net_NODE2___ = _NET_FAULT;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_overheated_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 22 and event "Net.NODE2._.u_power_restore".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge22(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_NO_POWER)) && ((Net_NODE2_CAN_Evt_power_restored_) && (!(Net_NODE2_CAN_Evt_no_power_)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_power_restore_, TRUE);
    #endif

    Net_NODE2___fault_reason_ = _NET_POWER_RESTORE;
    Net_NODE2___ = _NET_FAULT;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_power_restore_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 23 and event "Net.NODE2._.u_recover_failed".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge23(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_FAULT)) && ((!(Net_NODE2___rec_flag_)) && (Net_NODE2_CAN_Evt_recover_failed_));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_recover_failed_, TRUE);
    #endif

    Net_NODE2___recovery_att_cnt_ = IntegerMin((Net_NODE2___recovery_att_cnt_) + (1), Net_NODE2___MAX_RANGE_);
    Net_NODE2___rec_flag_ = !(Net_NODE2___rec_flag_);

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_recover_failed_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 24 and event "Net.NODE2._.u_setup_error".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge24(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_INIT)) && (Net_NODE2_CAN_Evt_setup_error_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_setup_error_, TRUE);
    #endif

    Net_NODE2___ = _NET_SETUP_ERROR;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_setup_error_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 25 and event "Net.NODE2._.u_setup_ok".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge25(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_INIT)) && (Net_NODE2_CAN_Evt_setup_ok_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_setup_ok_, TRUE);
    #endif

    Net_NODE2___reboot_cnt_ = 0;
    Net_NODE2___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_setup_ok_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 26 and event "Net.NODE2._.u_stopped".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge26(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_STOPPING)) && (Net_NODE2_CAN_Evt_stopped_);
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_stopped_, TRUE);
    #endif

    Net_NODE2___cur_vel_ = 0;
    Net_NODE2___ = _NET_IDLE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___u_stopped_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 27 and event "Net.coord._.c_all_calibrated".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge27(void) {
    BoolType guard = (((Net_coord___) == (_NET_CALIBRATING)) && ((Net_NODE1___) == (_NET_IDLE))) && (((Net_NODE2___) == (_NET_IDLE)) && (!(LIMIT_EXHAUSTED_())));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_calibrated_, TRUE);
    #endif

    Net_coord___ = _NET_RUNNING;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_calibrated_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 28 and event "Net.coord._.c_all_healthy".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge28(void) {
    BoolType guard = (((Net_coord___) == (_NET_RECOVERING)) && ((Net_coord___) == (_NET_RECOVERING))) && ((((Net_NODE1___) == (_NET_IDLE)) || ((Net_NODE1___) == (_NET_NEEDS_RECAL))) && (((Net_NODE2___) == (_NET_IDLE)) || ((Net_NODE2___) == (_NET_NEEDS_RECAL))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_healthy_, TRUE);
    #endif

    Net_coord___ = _NET_BOOTING;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_healthy_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 29 and event "Net.coord._.c_all_power_lost".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge29(void) {
    BoolType guard = ((((Net_coord___) == (_NET_IDLE)) || (((Net_coord___) == (_NET_BOOTING)) || ((Net_coord___) == (_NET_CALIBRATING)))) || ((((Net_coord___) == (_NET_RUNNING)) || ((Net_coord___) == (_NET_STOPPING))) || (((Net_coord___) == (_NET_RECOVERING)) || ((Net_coord___) == (_NET_EMERGENCY))))) && (((Net_NODE1___) == (_NET_NO_POWER)) && ((Net_NODE2___) == (_NET_NO_POWER)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_power_lost_, TRUE);
    #endif

    if ((Net_coord___) == (_NET_IDLE)) {
        Net_coord___ = _NET_NO_POWER;
    } else if ((Net_coord___) == (_NET_BOOTING)) {
        Net_coord___ = _NET_NO_POWER;
    } else if ((Net_coord___) == (_NET_CALIBRATING)) {
        Net_coord___ = _NET_NO_POWER;
    } else if ((Net_coord___) == (_NET_RUNNING)) {
        Net_coord___ = _NET_NO_POWER;
    } else if ((Net_coord___) == (_NET_STOPPING)) {
        Net_coord___ = _NET_NO_POWER;
    } else if ((Net_coord___) == (_NET_RECOVERING)) {
        Net_coord___ = _NET_NO_POWER;
    } else if ((Net_coord___) == (_NET_EMERGENCY)) {
        Net_coord___ = _NET_NO_POWER;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_power_lost_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 30 and event "Net.coord._.c_all_stopped".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge30(void) {
    BoolType guard = (((Net_coord___) == (_NET_STOPPING)) && ((Net_coord___) == (_NET_STOPPING))) && ((((Net_NODE1___) == (_NET_IDLE)) || ((Net_NODE1___) == (_NET_NEEDS_RECAL))) && (((Net_NODE2___) == (_NET_IDLE)) || ((Net_NODE2___) == (_NET_NEEDS_RECAL))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_stopped_, TRUE);
    #endif

    Net_coord___ = _NET_IDLE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_all_stopped_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 31 and event "Net.coord._.c_limits_exceeded".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge31(void) {
    BoolType guard = (((Net_coord___) == (_NET_BOOTING)) || (((Net_coord___) == (_NET_CALIBRATING)) || ((Net_coord___) == (_NET_RECOVERING)))) && (LIMIT_EXHAUSTED_());
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_limits_exceeded_, TRUE);
    #endif

    if ((Net_coord___) == (_NET_BOOTING)) {
        Net_coord___ = _NET_EMERGENCY;
    } else if ((Net_coord___) == (_NET_CALIBRATING)) {
        Net_coord___ = _NET_EMERGENCY;
    } else if ((Net_coord___) == (_NET_RECOVERING)) {
        Net_coord___ = _NET_EMERGENCY;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_limits_exceeded_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 32 and event "Net.coord._.c_node_unhealthy".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge32(void) {
    BoolType guard = ((((Net_coord___) == (_NET_BOOTING)) || ((Net_coord___) == (_NET_CALIBRATING))) || (((Net_coord___) == (_NET_RUNNING)) || ((Net_coord___) == (_NET_STOPPING)))) && ((((Net_NODE1___) == (_NET_FAULT)) || (((Net_NODE1___) == (_NET_SETUP_ERROR)) || ((Net_NODE2___) == (_NET_FAULT)))) || (((Net_NODE2___) == (_NET_SETUP_ERROR)) || ((((Net_NODE1___) == (_NET_NO_POWER)) && ((Net_NODE2___ != _NET_NO_POWER))) || (((Net_NODE2___) == (_NET_NO_POWER)) && ((Net_NODE1___ != _NET_NO_POWER))))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_node_unhealthy_, TRUE);
    #endif

    if ((Net_coord___) == (_NET_BOOTING)) {
        Net_coord___ = _NET_RECOVERING;
    } else if ((Net_coord___) == (_NET_CALIBRATING)) {
        Net_coord___ = _NET_RECOVERING;
    } else if ((Net_coord___) == (_NET_RUNNING)) {
        Net_coord___ = _NET_RECOVERING;
    } else if ((Net_coord___) == (_NET_STOPPING)) {
        Net_coord___ = _NET_RECOVERING;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_node_unhealthy_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 33 and event "Net.coord._.c_nodes_ready".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge33(void) {
    BoolType guard = ((Net_coord___) == (_NET_BOOTING)) && (((((Net_NODE1___) == (_NET_NEEDS_RECAL)) || ((Net_NODE1___) == (_NET_IDLE))) || ((Net_NODE1___) == (_NET_CAL_FAILED))) && ((((Net_NODE2___) == (_NET_NEEDS_RECAL)) || ((Net_NODE2___) == (_NET_IDLE))) || ((Net_NODE2___) == (_NET_CAL_FAILED))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_nodes_ready_, TRUE);
    #endif

    Net_coord___ = _NET_CALIBRATING;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_nodes_ready_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 34 and event "Net.coord._.c_power_restored".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge34(void) {
    BoolType guard = (((Net_coord___) == (_NET_NO_POWER)) && ((Net_coord___) == (_NET_NO_POWER))) && (((Net_NODE1___ != _NET_NO_POWER)) && ((Net_NODE2___ != _NET_NO_POWER)));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_power_restored_, TRUE);
    #endif

    Net_coord___ = _NET_RECOVERING;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_coord___c_power_restored_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 35 and event "Net.NODE1._.c_cal_reject".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge35(void) {
    BoolType guard = FALSE;
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_cal_reject_, TRUE);
    #endif

    Net_NODE1___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_cal_reject_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 36 and event "Net.NODE1._.c_calibrate".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge36(void) {
    BoolType guard = ((((Net_NODE1___) == (_NET_NEEDS_RECAL)) || ((Net_NODE1___) == (_NET_CAL_FAILED))) && (((Net_coord___) == (_NET_CALIBRATING)) && ((Net_NODE2___ != _NET_CALIBRATING)))) && ((((Net_NODE1___calibration_att_cnt_) < (MAX_CAL_)) && ((Net_NODE1___power_cc_cnt_) < (MAX_POWER_))) && (((!((((Net_NODE2___) == (_NET_FAULT)) || ((Net_NODE2___) == (_NET_SETUP_ERROR))) || ((Net_NODE2___) == (_NET_NO_POWER)))) || ((Net_coord___) == (_NET_RECOVERING))) && (!(LIMIT_EXHAUSTED_()))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_calibrate_, TRUE);
    #endif

    if ((Net_NODE1___) == (_NET_NEEDS_RECAL)) {
        Net_NODE1___ = _NET_CALIBRATING;
    } else if ((Net_NODE1___) == (_NET_CAL_FAILED)) {
        Net_NODE1___ = _NET_CALIBRATING;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_calibrate_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 37 and event "Net.NODE1._.c_enable".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge37(void) {
    BoolType guard = (((((Net_NODE1___) == (_NET_IDLE)) || ((Net_NODE1___) == (_NET_STOPPING))) && ((Net_coord___) == (_NET_RUNNING))) && (((Net_NODE2___ != _NET_FAULT)) && ((Net_NODE2___ != _NET_NO_POWER)))) && ((((Net_NODE2___ != _NET_SETUP_ERROR)) && ((Net_NODE2___ != _NET_CAL_FAILED))) && (((Net_NODE1___fault_cnt_) < (MAX_FAULTS_)) && ((Net_NODE2___fault_cnt_) < (MAX_FAULTS_))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_enable_, TRUE);
    #endif

    if ((Net_NODE1___) == (_NET_IDLE)) {
        Net_NODE1___ = _NET_RUNNING;
    } else if ((Net_NODE1___) == (_NET_STOPPING)) {
        Net_NODE1___ = _NET_RUNNING;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_enable_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 38 and event "Net.NODE1._.c_reboot".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge38(void) {
    BoolType guard = (((Net_NODE1___) == (_NET_SETUP_ERROR)) && (((Net_NODE1___reboot_cnt_) < (MAX_REBOOT_)) && ((Net_coord___) == (_NET_RECOVERING)))) && ((!(LIMIT_EXHAUSTED_())) && (((Net_NODE2___ != _NET_RUNNING)) && ((Net_NODE2___) == (_NET_IDLE))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_reboot_, TRUE);
    #endif

    Net_NODE1___reboot_cnt_ = IntegerMin((Net_NODE1___reboot_cnt_) + (1), Net_NODE1___MAX_RANGE_);
    Net_NODE1___ = _NET_INIT;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_reboot_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 39 and event "Net.NODE1._.c_recover".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge39(void) {
    BoolType guard = (((Net_NODE1___) == (_NET_FAULT)) && (Net_NODE1___rec_flag_)) && (((Net_coord___) == (_NET_RECOVERING)) && (((Net_NODE1___recovery_att_cnt_) < (MAX_RECOVERY_)) && (!(LIMIT_EXHAUSTED_()))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_recover_, TRUE);
    #endif

    Net_NODE1___rec_flag_ = !(Net_NODE1___rec_flag_);

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_recover_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 40 and event "Net.NODE1._.c_set_velocity".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge40(void) {
    BoolType guard = (((Net_NODE1___) == (_NET_RUNNING)) && ((Net_NODE1___cur_vel_) != (Net_NODE1___tar_vel_()))) && (((Net_coord___) == (_NET_RUNNING)) && (!((((Net_NODE2___) == (_NET_FAULT)) || ((Net_NODE2___) == (_NET_SETUP_ERROR))) || ((Net_NODE2___) == (_NET_NO_POWER)))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_set_velocity_, TRUE);
    #endif

    Net_NODE1___cur_vel_ = Net_NODE1___tar_vel_();

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_set_velocity_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 41 and event "Net.NODE1._.c_stop".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge41(void) {
    BoolType guard = ((Net_NODE1___) == (_NET_RUNNING)) && (((Net_coord___) == (_NET_STOPPING)) || (((Net_coord___) == (_NET_RECOVERING)) || ((Net_coord___) == (_NET_EMERGENCY))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_stop_, TRUE);
    #endif

    Net_NODE1___ = _NET_STOPPING;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE1___c_stop_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 42 and event "Net.NODE2._.c_cal_reject".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge42(void) {
    BoolType guard = FALSE;
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_cal_reject_, TRUE);
    #endif

    Net_NODE2___ = _NET_NEEDS_RECAL;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_cal_reject_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 43 and event "Net.NODE2._.c_calibrate".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge43(void) {
    BoolType guard = (((((Net_NODE2___) == (_NET_NEEDS_RECAL)) || ((Net_NODE2___) == (_NET_CAL_FAILED))) && ((Net_coord___) == (_NET_CALIBRATING))) && (((Net_NODE1___ != _NET_CALIBRATING)) && ((Net_NODE2___calibration_att_cnt_) < (MAX_CAL_)))) && ((((Net_NODE2___power_cc_cnt_) < (MAX_POWER_)) && ((!((((Net_NODE1___) == (_NET_FAULT)) || ((Net_NODE1___) == (_NET_SETUP_ERROR))) || ((Net_NODE1___) == (_NET_NO_POWER)))) || ((Net_coord___) == (_NET_RECOVERING)))) && ((!(LIMIT_EXHAUSTED_())) && (((Net_NODE1___ != _NET_NEEDS_RECAL)) && ((Net_NODE1___ != _NET_CAL_FAILED)))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_calibrate_, TRUE);
    #endif

    if ((Net_NODE2___) == (_NET_NEEDS_RECAL)) {
        Net_NODE2___ = _NET_CALIBRATING;
    } else if ((Net_NODE2___) == (_NET_CAL_FAILED)) {
        Net_NODE2___ = _NET_CALIBRATING;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_calibrate_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 44 and event "Net.NODE2._.c_enable".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge44(void) {
    BoolType guard = (((((Net_NODE2___) == (_NET_IDLE)) || ((Net_NODE2___) == (_NET_STOPPING))) && ((Net_coord___) == (_NET_RUNNING))) && (((Net_NODE1___ != _NET_FAULT)) && ((Net_NODE1___ != _NET_NO_POWER)))) && ((((Net_NODE1___ != _NET_SETUP_ERROR)) && ((Net_NODE1___ != _NET_CAL_FAILED))) && (((Net_NODE1___fault_cnt_) < (MAX_FAULTS_)) && ((Net_NODE2___fault_cnt_) < (MAX_FAULTS_))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_enable_, TRUE);
    #endif

    if ((Net_NODE2___) == (_NET_IDLE)) {
        Net_NODE2___ = _NET_RUNNING;
    } else if ((Net_NODE2___) == (_NET_STOPPING)) {
        Net_NODE2___ = _NET_RUNNING;
    }

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_enable_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 45 and event "Net.NODE2._.c_reboot".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge45(void) {
    BoolType guard = (((Net_NODE2___) == (_NET_SETUP_ERROR)) && (((Net_NODE2___reboot_cnt_) < (MAX_REBOOT_)) && ((Net_coord___) == (_NET_RECOVERING)))) && ((!(LIMIT_EXHAUSTED_())) && (((Net_NODE1___ != _NET_RUNNING)) && ((Net_NODE1___) == (_NET_IDLE))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_reboot_, TRUE);
    #endif

    Net_NODE2___reboot_cnt_ = IntegerMin((Net_NODE2___reboot_cnt_) + (1), Net_NODE2___MAX_RANGE_);
    Net_NODE2___ = _NET_INIT;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_reboot_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 46 and event "Net.NODE2._.c_recover".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge46(void) {
    BoolType guard = (((Net_NODE2___) == (_NET_FAULT)) && ((Net_NODE2___rec_flag_) && ((Net_coord___) == (_NET_RECOVERING)))) && (((Net_NODE1___ != _NET_FAULT)) && (((Net_NODE2___recovery_att_cnt_) < (MAX_RECOVERY_)) && (!(LIMIT_EXHAUSTED_()))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_recover_, TRUE);
    #endif

    Net_NODE2___rec_flag_ = !(Net_NODE2___rec_flag_);

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_recover_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 47 and event "Net.NODE2._.c_set_velocity".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge47(void) {
    BoolType guard = (((Net_NODE2___) == (_NET_RUNNING)) && ((Net_NODE2___cur_vel_) != (Net_NODE2___tar_vel_()))) && (((Net_coord___) == (_NET_RUNNING)) && (!((((Net_NODE1___) == (_NET_FAULT)) || ((Net_NODE1___) == (_NET_SETUP_ERROR))) || ((Net_NODE1___) == (_NET_NO_POWER)))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_set_velocity_, TRUE);
    #endif

    Net_NODE2___cur_vel_ = Net_NODE2___tar_vel_();

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_set_velocity_, FALSE);
    #endif
    return TRUE;
}

/**
 * Execute code for edge with index 48 and event "Net.NODE2._.c_stop".
 *
 * @return Whether the edge was performed.
 */
static BoolType execEdge48(void) {
    BoolType guard = ((Net_NODE2___) == (_NET_RUNNING)) && (((Net_coord___) == (_NET_STOPPING)) || (((Net_coord___) == (_NET_RECOVERING)) || ((Net_coord___) == (_NET_EMERGENCY))));
    if (!guard) return FALSE;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_stop_, TRUE);
    #endif

    Net_NODE2___ = _NET_STOPPING;

    #if EVENT_OUTPUT
        NET_InfoEvent(Net_NODE2___c_stop_, FALSE);
    #endif
    return TRUE;
}

/**
 * Normalize and check the new value of a continuous variable after an update.
 * @param new_value Unnormalized new value of the continuous variable.
 * @param var_name Name of the continuous variable in the CIF model.
 * @return The normalized new value of the continuous variable.
 */
static inline RealType UpdateContValue(RealType new_value, const char *var_name) {
    if (isfinite(new_value)) {
        return (new_value == -0.0) ? 0.0 : new_value;
    }

    const char *err_type;
    if (isnan(new_value)) {
        err_type = "NaN";
    } else if (new_value > 0) {
        err_type = "+inf";
    } else {
        err_type = "-inf";
    }
    fprintf(stderr, "Continuous variable \"%s\" has become %s.\n", var_name, err_type);

#ifdef KEEP_RUNNING
    return 0.0;
#else
    exit(1);
#endif
}

/** Repeatedly perform discrete event steps, until no progress can be made any more. */
static void PerformEdges(void) {
    /* Uncontrollables. */
    int count = 0;
    for (;;) {
        count++;
        if (count > MAX_NUM_ITERS) { /* 'Infinite' loop detection. */
            fprintf(stderr, "Warning: Quitting after performing %d uncontrollable events, infinite loop?\n", count);
            break;
        }

        BoolType edgeExecuted = false;

        edgeExecuted |= execEdge0(); /* (Try to) perform edge with index 0 and event "Net.coord._.u_start". */
        edgeExecuted |= execEdge1(); /* (Try to) perform edge with index 1 and event "Net.coord._.u_stop". */
        edgeExecuted |= execEdge2(); /* (Try to) perform edge with index 2 and event "Net.coord._.u_technician_reset". */
        edgeExecuted |= execEdge3(); /* (Try to) perform edge with index 3 and event "Net.NODE1._.u_cal_failed". */
        edgeExecuted |= execEdge4(); /* (Try to) perform edge with index 4 and event "Net.NODE1._.u_cal_success". */
        edgeExecuted |= execEdge5(); /* (Try to) perform edge with index 5 and event "Net.NODE1._.u_cooled". */
        edgeExecuted |= execEdge6(); /* (Try to) perform edge with index 6 and event "Net.NODE1._.u_fault". */
        edgeExecuted |= execEdge7(); /* (Try to) perform edge with index 7 and event "Net.NODE1._.u_fault_cleared". */
        edgeExecuted |= execEdge8(); /* (Try to) perform edge with index 8 and event "Net.NODE1._.u_no_power". */
        edgeExecuted |= execEdge9(); /* (Try to) perform edge with index 9 and event "Net.NODE1._.u_overheated". */
        edgeExecuted |= execEdge10(); /* (Try to) perform edge with index 10 and event "Net.NODE1._.u_power_restore". */
        edgeExecuted |= execEdge11(); /* (Try to) perform edge with index 11 and event "Net.NODE1._.u_recover_failed". */
        edgeExecuted |= execEdge12(); /* (Try to) perform edge with index 12 and event "Net.NODE1._.u_setup_error". */
        edgeExecuted |= execEdge13(); /* (Try to) perform edge with index 13 and event "Net.NODE1._.u_setup_ok". */
        edgeExecuted |= execEdge14(); /* (Try to) perform edge with index 14 and event "Net.NODE1._.u_stopped". */
        edgeExecuted |= execEdge15(); /* (Try to) perform edge with index 15 and event "Net.NODE2._.u_cal_failed". */
        edgeExecuted |= execEdge16(); /* (Try to) perform edge with index 16 and event "Net.NODE2._.u_cal_success". */
        edgeExecuted |= execEdge17(); /* (Try to) perform edge with index 17 and event "Net.NODE2._.u_cooled". */
        edgeExecuted |= execEdge18(); /* (Try to) perform edge with index 18 and event "Net.NODE2._.u_fault". */
        edgeExecuted |= execEdge19(); /* (Try to) perform edge with index 19 and event "Net.NODE2._.u_fault_cleared". */
        edgeExecuted |= execEdge20(); /* (Try to) perform edge with index 20 and event "Net.NODE2._.u_no_power". */
        edgeExecuted |= execEdge21(); /* (Try to) perform edge with index 21 and event "Net.NODE2._.u_overheated". */
        edgeExecuted |= execEdge22(); /* (Try to) perform edge with index 22 and event "Net.NODE2._.u_power_restore". */
        edgeExecuted |= execEdge23(); /* (Try to) perform edge with index 23 and event "Net.NODE2._.u_recover_failed". */
        edgeExecuted |= execEdge24(); /* (Try to) perform edge with index 24 and event "Net.NODE2._.u_setup_error". */
        edgeExecuted |= execEdge25(); /* (Try to) perform edge with index 25 and event "Net.NODE2._.u_setup_ok". */
        edgeExecuted |= execEdge26(); /* (Try to) perform edge with index 26 and event "Net.NODE2._.u_stopped". */

        if (!edgeExecuted) {
            break; /* No edge fired, done with discrete steps. */
        }
    }

    /* Controllables. */
    count = 0;
    for (;;) {
        count++;
        if (count > MAX_NUM_ITERS) { /* 'Infinite' loop detection. */
            fprintf(stderr, "Warning: Quitting after performing %d controllable events, infinite loop?\n", count);
            break;
        }

        BoolType edgeExecuted = false;

        edgeExecuted |= execEdge27(); /* (Try to) perform edge with index 27 and event "Net.coord._.c_all_calibrated". */
        edgeExecuted |= execEdge28(); /* (Try to) perform edge with index 28 and event "Net.coord._.c_all_healthy". */
        edgeExecuted |= execEdge29(); /* (Try to) perform edge with index 29 and event "Net.coord._.c_all_power_lost". */
        edgeExecuted |= execEdge30(); /* (Try to) perform edge with index 30 and event "Net.coord._.c_all_stopped". */
        edgeExecuted |= execEdge31(); /* (Try to) perform edge with index 31 and event "Net.coord._.c_limits_exceeded". */
        edgeExecuted |= execEdge32(); /* (Try to) perform edge with index 32 and event "Net.coord._.c_node_unhealthy". */
        edgeExecuted |= execEdge33(); /* (Try to) perform edge with index 33 and event "Net.coord._.c_nodes_ready". */
        edgeExecuted |= execEdge34(); /* (Try to) perform edge with index 34 and event "Net.coord._.c_power_restored". */
        edgeExecuted |= execEdge35(); /* (Try to) perform edge with index 35 and event "Net.NODE1._.c_cal_reject". */
        edgeExecuted |= execEdge36(); /* (Try to) perform edge with index 36 and event "Net.NODE1._.c_calibrate". */
        edgeExecuted |= execEdge37(); /* (Try to) perform edge with index 37 and event "Net.NODE1._.c_enable". */
        edgeExecuted |= execEdge38(); /* (Try to) perform edge with index 38 and event "Net.NODE1._.c_reboot". */
        edgeExecuted |= execEdge39(); /* (Try to) perform edge with index 39 and event "Net.NODE1._.c_recover". */
        edgeExecuted |= execEdge40(); /* (Try to) perform edge with index 40 and event "Net.NODE1._.c_set_velocity". */
        edgeExecuted |= execEdge41(); /* (Try to) perform edge with index 41 and event "Net.NODE1._.c_stop". */
        edgeExecuted |= execEdge42(); /* (Try to) perform edge with index 42 and event "Net.NODE2._.c_cal_reject". */
        edgeExecuted |= execEdge43(); /* (Try to) perform edge with index 43 and event "Net.NODE2._.c_calibrate". */
        edgeExecuted |= execEdge44(); /* (Try to) perform edge with index 44 and event "Net.NODE2._.c_enable". */
        edgeExecuted |= execEdge45(); /* (Try to) perform edge with index 45 and event "Net.NODE2._.c_reboot". */
        edgeExecuted |= execEdge46(); /* (Try to) perform edge with index 46 and event "Net.NODE2._.c_recover". */
        edgeExecuted |= execEdge47(); /* (Try to) perform edge with index 47 and event "Net.NODE2._.c_set_velocity". */
        edgeExecuted |= execEdge48(); /* (Try to) perform edge with index 48 and event "Net.NODE2._.c_stop". */

        if (!edgeExecuted) {
            break; /* No edge fired, done with discrete steps. */
        }
    }
}

/** First model call, initializing, and performing discrete events before the first time step. */
void NET_EngineFirstStep(void) {
    InitConstants();

    model_time = 0.0;
    NET_AssignInputVariables();
    Net_coord___velocity_ = 1;
    Net_coord___ = _NET_IDLE;
    Net_NODE1___fault_cnt_ = 0;
    Net_NODE1___power_cc_cnt_ = 0;
    Net_NODE1___recovery_att_cnt_ = 0;
    Net_NODE1___calibration_att_cnt_ = 0;
    Net_NODE1___reboot_cnt_ = 0;
    Net_NODE1___cur_vel_ = 0;
    Net_NODE1___rec_flag_ = TRUE;
    Net_NODE1___fault_reason_ = _NET_NONE;
    Net_NODE1___ = _NET_INIT;
    Net_NODE2___fault_cnt_ = 0;
    Net_NODE2___power_cc_cnt_ = 0;
    Net_NODE2___recovery_att_cnt_ = 0;
    Net_NODE2___calibration_att_cnt_ = 0;
    Net_NODE2___reboot_cnt_ = 0;
    Net_NODE2___cur_vel_ = 0;
    Net_NODE2___rec_flag_ = TRUE;
    Net_NODE2___fault_reason_ = _NET_NONE;
    Net_NODE2___ = _NET_INIT;

    #if PRINT_OUTPUT
        /* pre-initial and post-initial prints. */
        PrintOutput(EVT_INITIAL_, TRUE);
        PrintOutput(EVT_INITIAL_, FALSE);
    #endif

    PerformEdges();

    #if PRINT_OUTPUT
        /* pre-timestep print. */
        PrintOutput(EVT_DELAY_, TRUE);
    #endif
}

/**
 * Engine takes a time step of length \a delta.
 * @param delta Length of the time step.
 */
void NET_EngineTimeStep(double delta) {
    NET_AssignInputVariables();

    /* Update continuous variables. */
    if (delta > 0.0) {

        model_time += delta;
    }

    #if PRINT_OUTPUT
        /* post-timestep print. */
        PrintOutput(EVT_DELAY_, FALSE);
    #endif

    PerformEdges();

    #if PRINT_OUTPUT
        /* pre-timestep print. */
        PrintOutput(EVT_DELAY_, TRUE);
    #endif
}

