/*********************************************************************************
 *      Software-in-the-Loop (SIL) Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         interface.c
 *      @ Description:  SIL wrapper interface functions
 ********************************************************************************/

#include <interface.h>
/*********************************************************************
*
* Include custom headers here, which are required for custom codes 
* to be added in initializationTasks(), sampFreq_controlTask_update(),
* and backgroundTask_Update().
*
**********************************************************************/

/* Function Definitions */

/* Initialization Tasks */
void initializationTasks(void){
    /*
    * Custom code goes here.
    */
}

/* Control Update Tasks run at Sampling Rate */
void sampFreq_controlTask_update(feedbacks *fb, controlOutputs *ctrlOut, float *dbIn, float *dbOut){
    /*
    * Custom code goes here.
    */
}

/* Background Tasks run */
void backgroundTask_Update(disParam *dispatch, statusMon *status){
    /*
    * Custom code goes here.
    */    
}

