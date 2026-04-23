/* Additional code to allow compilation and test of the generated code.
 *
 * This file is generated, DO NOT EDIT
 */

#include <stdio.h>
#include "NET_engine.h"

/* Assign values to the input variables. */
void NET_AssignInputVariables(void) {
    /* Input variable "Net.coord.HMI._u_start". */
    Net_coord_HMI__u_start_ = FALSE;

    /* Input variable "Net.coord.HMI._u_stop". */
    Net_coord_HMI__u_stop_ = FALSE;

    /* Input variable "Net.coord.HMI._u_technician_reset". */
    Net_coord_HMI__u_technician_reset_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_setup_ok". */
    Net_NODE1_CAN_Evt_setup_ok_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_setup_error". */
    Net_NODE1_CAN_Evt_setup_error_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_cal_success". */
    Net_NODE1_CAN_Evt_cal_success_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_cal_failed". */
    Net_NODE1_CAN_Evt_cal_failed_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_stopped". */
    Net_NODE1_CAN_Evt_stopped_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_fault". */
    Net_NODE1_CAN_Evt_fault_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_no_power". */
    Net_NODE1_CAN_Evt_no_power_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_power_restored". */
    Net_NODE1_CAN_Evt_power_restored_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_overheated". */
    Net_NODE1_CAN_Evt_overheated_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_cooled". */
    Net_NODE1_CAN_Evt_cooled_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_fault_cleared". */
    Net_NODE1_CAN_Evt_fault_cleared_ = FALSE;

    /* Input variable "Net.NODE1.CAN.Evt_recover_failed". */
    Net_NODE1_CAN_Evt_recover_failed_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_setup_ok". */
    Net_NODE2_CAN_Evt_setup_ok_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_setup_error". */
    Net_NODE2_CAN_Evt_setup_error_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_cal_success". */
    Net_NODE2_CAN_Evt_cal_success_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_cal_failed". */
    Net_NODE2_CAN_Evt_cal_failed_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_stopped". */
    Net_NODE2_CAN_Evt_stopped_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_fault". */
    Net_NODE2_CAN_Evt_fault_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_no_power". */
    Net_NODE2_CAN_Evt_no_power_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_power_restored". */
    Net_NODE2_CAN_Evt_power_restored_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_overheated". */
    Net_NODE2_CAN_Evt_overheated_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_cooled". */
    Net_NODE2_CAN_Evt_cooled_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_fault_cleared". */
    Net_NODE2_CAN_Evt_fault_cleared_ = FALSE;

    /* Input variable "Net.NODE2.CAN.Evt_recover_failed". */
    Net_NODE2_CAN_Evt_recover_failed_ = FALSE;
}

void NET_InfoEvent(NET_Event_ event, BoolType pre) {
    const char *prePostText = pre ? "pre" : "post";
    printf("Executing %s-event \"%s\"\n", prePostText, NET_event_names[event]);
}

void NET_PrintOutput(const char *text, const char *fname) {
    printf("Print @ %s: \"%s\"\n", fname, text);
}

int main(void) {
    NET_EngineFirstStep();

    NET_EngineTimeStep(1.0);
    return 0;
}

