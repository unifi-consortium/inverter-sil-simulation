/*********************************************************************************
 *      Software-in-the-Loop (SIL) Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         wrapper.h
 *      @ Description:  SIL wrapper shell
 ********************************************************************************/
#ifndef WRAPPER_MAA_H
#define WRAPPER_MAA_H

extern void update_wrapper_in(float *v);
extern void update_wrapper_Out(float *v);

extern void updateWrapperDebugOutVars(float *v);
extern void updateWrapperDebugInVars(float *v);
extern void wrapper_background_task_update(void);
extern void update_dispatch_parameters(float *v);
extern void update_status_monitors(float *v);

extern void wrapper_init(void);
extern void wrapper_isr(void);

#endif














































