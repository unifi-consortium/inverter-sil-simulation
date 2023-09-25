/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         interface.h
 *      @ Description:  interface definitions between software & firmware/upper-
 *                      layer 
 ********************************************************************************/

#ifndef INCLUDE_INTERFACE_H_
#define INCLUDE_INTERFACE_H_

/* Constant defines */
#define NUMBER_OF_DEBUG_INPUTS          5
#define NUMBER_OF_DEBUG_OUTPUTS         10
#define NUMBER_OF_FEEDBACK_INPUTS       7
#define NUMBER_OF_CTRL_OUTPUTS          5
#define NUMBER_OF_DISPATCH_PARAMETERS   6
#define NUMBER_OF_STATUS_MON_SIGNALS    4

/* Typedefs */
typedef struct{
    float   vg_ab;
    float   vg_bc;
    float   ig_a;
    float   ig_b;
    float   ig_c;
    float   vdc;
    float   fwStatus;
} feedbacks;

typedef struct{
    float   pwmEnable;      /* Enable PWM */
    float   closeMcb;       /* Close main circuit breaker */
    float   m_u;            /* modulating signal - phase u */
    float   m_v;            /* modulating signal - phase v */
    float   m_w;            /* modulating signal - phase w */
} controlOutputs;

typedef struct{
    float   pwDroopCoeff;
    float   qvDroopCoeff;
    float   nomVol;
    float   nomFreq;
    float   nomP;
    float   nomQ;
} disParam;

typedef struct{
    float   freq;
    float   vol;
    float   outP;
    float   outQ;
    float   statusWord;
} statusMon;

void initializationTasks(void);
void sampFreq_controlTask_update(feedbacks *fb, controlOutputs *ctrlOut, float *dbIn, float *dbOut);
void backgroundTask_Update(disParam *dispatch, statusMon *status);

#endif /* INCLUDE_INTERFACE_H_ */
