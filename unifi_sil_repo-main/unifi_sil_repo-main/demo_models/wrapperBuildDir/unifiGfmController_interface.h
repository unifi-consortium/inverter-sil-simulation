/*********************************************************************************
 *      Software-in-the-Loop (SIL) Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         interface.h
 *      @ Description:  SIL wrapper interface
 ********************************************************************************/

#ifndef INCLUDE_INTERFACE_H_
#define INCLUDE_INTERFACE_H_


/**************************** Debug interfaces ************************
*
*	              Do not remove macros; only edit values.
*
***********************************************************************/
#define NUMBER_OF_DEBUG_INPUTS          5
#define NUMBER_OF_DEBUG_OUTPUTS         10


/**************************** Debug interfaces ************************
*
*	              Do not edit/alter structure name.
*           Structure members can be removed/added/altered.
*
***********************************************************************/
typedef struct{				/* Structure members can be removed/added/altered */			
    float   vg_ab;
    float   vg_bc;
    float   ig_a;
    float   ig_b;
    float   ig_c;
    float   vdc;
    float   fwStatus;
} feedbacks;				/* Do not edit/alter structure name */

typedef struct{				/* Structure members can be removed/added/altered */
    float   pwmEnable;      /* Enable PWM */
    float   closeMcb;       /* Close main circuit breaker */
    float   m_u;            /* modulating signal - phase u */
    float   m_v;            /* modulating signal - phase v */
    float   m_w;            /* modulating signal - phase w */
} controlOutputs;			/* Do not edit/alter structure name */

typedef struct{				/* Structure members can be removed/added/altered */
    float   pwDroopCoeff;
    float   qvDroopCoeff;
    float   nomVol;
    float   nomFreq;
    float   nomP;
    float   nomQ;
} disParam;					/* Do not edit/alter structure name */

typedef struct{				/* Structure members can be removed/added/altered */
    float   freq;
    float   vol;
    float   outP;
    float   outQ;
    float   statusWord;
} statusMon; 				/* Do not edit/alter structure name */


/*******************************************************************
*                      Do not edit below this line
********************************************************************/
void initializationTasks(void);
void sampFreq_controlTask_update(feedbacks *fb, controlOutputs *ctrlOut, float *dbIn, float *dbOut);
void backgroundTask_Update(disParam *dispatch, statusMon *status);

#endif /* INCLUDE_INTERFACE_H_ */

