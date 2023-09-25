/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         interface.c
 *      @ Description:  interface functions between software & firmware/upper-
 *                      layer 
 ********************************************************************************/

#include "interface.h"
#include "vectorControl.h"

/* Function Definitions */

/* Initialization Tasks */
void initializationTasks(void){
    stop_vectorControl((unsigned char) 1);
}

/* Control Update Tasks run at Sampling Rate */
void sampFreq_controlTask_update(feedbacks *fb, controlOutputs *ctrlOut, float *dbIn, float *dbOut){
    update_vectorControl(fb, ctrlOut, dbIn, dbOut);
}

/* Background Tasks run */
void backgroundTask_Update(disParam *dispatch, statusMon *status){
    update_dispatchMonitor(dispatch, status);
}

