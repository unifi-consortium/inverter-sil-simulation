/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         vectorControl.h
 *      @ Description:  Type & function definitions for vector control
 ********************************************************************************/

#ifndef INCLUDE_VECTORCONTROL_H_
#define INCLUDE_VECTORCONTROL_H_

#include "interface.h"
#include "variableTypes.h"

/* Firmware status definitions */
#define FW_STATE_READY                  0x01
#define FW_STATE_RUN_ENABLE             0x02

/* Power Stage/ Software status */
#define SW_STATE_PS_STATUS_BITS         0x07
#define SW_STATE_PS_FAULT_ID            0x31

/* Typedefs */
typedef enum{
    stopState,
    preSynchronizationState,
    runState,
    faultState
} fsmState;                          /* State definitions for finite state machine */

void stop_vectorControl(bool initialize);
void update_vectorControl(feedbacks *fb, controlOutputs *ctrlOut, float *dbIn, float *dbOut);
void update_dispatchMonitor(disParam *dis, statusMon *st);
void psFSM(controlOutputs *ctrlOut);

#endif /* INCLUDE_VECTORCONTROL_H_ */
