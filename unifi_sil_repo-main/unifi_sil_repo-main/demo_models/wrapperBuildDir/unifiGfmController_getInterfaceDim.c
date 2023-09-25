/*********************************************************************************
 *      Software-in-the-Loop (SIL) Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         getInterfaceDim.c
 *      @ Description:  script determines wrapper interface dimensions
 ********************************************************************************/
#include <unifiGfmController_getInterfaceDim.h>
#include <unifiGfmController_interface.h>

feedbacks		fb;
controlOutputs 	ctrlOut;
disParam		dispatch;
statusMon		status;
float 			debugOutputVars[NUMBER_OF_DEBUG_OUTPUTS];
float 			debugInputVars[NUMBER_OF_DEBUG_INPUTS];

/* Find dimensions of the interface variables/parameters */
void findInterfaceDimensions(float *v) 
{
	int byteSize = 0;

	byteSize = sizeof(debugInputVars);
	*(v) 		= byteSize/4;
	byteSize	= sizeof(debugOutputVars);
	*(v + 1) 	= byteSize/4;
	byteSize 	= sizeof(feedbacks);
	*(v + 2)	= byteSize/4;
	byteSize 	= sizeof(ctrlOut);
	*(v + 3) 	= byteSize/4;
	byteSize 	= sizeof(dispatch);
	*(v + 4) 	= byteSize/4;
	byteSize 	= sizeof(status);
	*(v + 5) 	= byteSize/4;
}


