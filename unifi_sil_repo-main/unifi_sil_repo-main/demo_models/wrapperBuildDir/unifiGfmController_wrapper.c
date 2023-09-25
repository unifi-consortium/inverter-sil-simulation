/*********************************************************************************
 *      Software-in-the-Loop (SIL) Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         wrapper.c
 *      @ Description:  SIL wrapper shell
 ********************************************************************************/

#include <unifiGfmController_wrapper.h>
#include <unifiGfmController_interface.h>
#include <string.h>

feedbacks		fb;
controlOutputs 	ctrlOut;
disParam		dispatch;
statusMon		status;
float 			debugOutputVars[NUMBER_OF_DEBUG_OUTPUTS];
float 			debugInputVars[NUMBER_OF_DEBUG_INPUTS];
float 			*debugInputVarsPtr 	= &debugInputVars[0];
float 			*debugOutputVarsPtr = &debugOutputVars[0];

/* Copy control outputs from wrapper to simulink */
void update_wrapper_Out(float *v) 
{
	memcpy(v,(const unsigned char*)&ctrlOut,sizeof(ctrlOut));
}

/* Copy feedback values from simulink to wrapper */
void update_wrapper_in(float *v) 
{
	memcpy(&fb,(const unsigned char*)v,sizeof(fb));
}

/* Copy status monitors from wrapper to simulink */
void update_status_monitors(float *v) 
{
	memcpy(v,(const unsigned char*)&status,sizeof(status));
}

/* Copy dispatch parameters from simulink to wrapper */
void update_dispatch_parameters(float *v) 
{
	memcpy(&dispatch,(const unsigned char*)v,sizeof(dispatch));
}

/* Copy debug outputs from wrapper to simulink */
void updateWrapperDebugOutVars(float *v) 
{		
	memcpy(v,(const unsigned char*)&debugOutputVars,sizeof(debugOutputVars));
}

/* Copy debug inputs from simulink to wrapper */
void updateWrapperDebugInVars(float *v) 
{	
	memcpy(debugInputVarsPtr,(const unsigned char*)v,sizeof(debugInputVars));

}

/* Update control task at sampling rate */
void wrapper_isr(void)
{
	//sampFreq_controlTask_update(&fb, &ctrlOut, debugInputVarsPtr, debugOutputVarsPtr);
	sampFreq_controlTask_update(&fb, &ctrlOut, debugInputVars, debugOutputVars);
}

/* Initialization tasks */
void wrapper_init(void)
{
	initializationTasks();
}

/* Backgound task update - dispatch & stauts monitoring */
void wrapper_background_task_update(void){
	backgroundTask_Update(&dispatch, &status);
}
















































