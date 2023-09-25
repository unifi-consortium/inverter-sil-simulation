/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         primaryControl.h
 *      @ Description:  type & function definitions for primary control
 ********************************************************************************/

#ifndef INCLUDE_PRIMARYCONTROL_H_
#define INCLUDE_PRIMARYCONTROL_H_

#include "variableTypes.h"

/* Macro Defines */
/* Define one of {DROOP_CONTROL, UVOC, VSM_CONTROL} */
#define DROOP_CONTROL           

/* Types */
typedef struct{
    lowPassFilter   Pfilt;
    lowPassFilter   Qfilt;
    float           mp;
    float           mq;
    float           V0;
    float           W0;
    float           P0;
    float           Q0;
    float           V;
    float           W;
    float           theta;
    float           Ts;
    abVector        v;
}droopCtrl;

typedef struct {
    abVector        v;              // Osillator states [vAlpha vBeta]^T
    abVector        v1;             // Used for two stage-RK/Heun's method (Osc states at t=(k+1)Ts estimated by using slope at t=kTs)
    abVector        delta1;         // Used for two stage-RK/Heun's method (time-derivative/slope at t=kTs)
    abVector        delta2;         // Used for two stage-RK/Heun's method (time-derivative/slope at t=(k+1)Ts)
    float           mu;             // Voltage amplitude correction gain
    float           eta;            // Phase correction/synchronization gain
    float           W0;             // Nominal frequency
    float           V0;             // Nominal voltage
    float           P0;             // Nominal real power set-point
    float           Q0;             // Nominal reactive power set-point
    float           Tsam;           // Sampling period
    float           Thsam;          // Half of sampling period
} uVoc;

typedef struct{
    lowPassFilter   Pfilt;
    lowPassFilter   Qfilt;
    float           mp;
    float           Dq;
    float           Kq;
    float           V0;
    float           W0;
    float           P0;
    float           Q0;
    float           V;
    float           W;
    float           theta;
    float           Ts;
    abVector        v;
}vsmCtrl;

#ifdef  DROOP_CONTROL
#define init_gfm_control(u, w0, v0, delW, delV, p0, q0, tp, tq, fpq, ts, cp)    init_droopControl(u, w0, v0, delW, delV, p0, q0, tp, tq, fpq, ts, cp)
#define update_gfm_control(u, vo, io, p0, q0)                                   update_droopControl(u, vo, io, p0, q0)

#elif defined UVOC
#define init_gfm_control(u, w0, v0, delW, delV, p0, q0, tp, tq, fpq, ts, cp)    init_uVoc(u, w0, v0, delW, delV, p0, q0, tp, tq, fpq, ts, cp)
#define update_gfm_control(u, vo, io, p0, q0)                                   update_uVoc(u, vo, io, p0, q0)

#elif defined VSM_CONTROL
#define init_gfm_control(u, w0, v0, delW, delV, p0, q0, tp, tq, fpq, ts, cp)    init_vsmControl(u, w0, v0, delW, delV, p0, q0, tp, tq, fpq, ts, cp)
#define update_gfm_control(u, vo, io, p0, q0)                                   update_vsmControl(u, vo, io, p0, q0)
#endif

#ifdef DROOP_CONTROL
typedef droopCtrl   gfmControl;
#elif defined UVOC
typedef uVoc        gfmControl;
#elif defined VSM_CONTROL
typedef vsmCtrl     gfmControl;
#endif

/* Function Prototypes */
void init_droopControl(droopCtrl *u, float nomW, float nomV, float delWmax, float delVmax, float Prated, float Qrated, float Tp, float Tq, float pqFiltBwHz, float samPeriod, bool calcParam);
void update_droopControl(droopCtrl *u, abVector *vg, abVector*iout, float P0, float Q0);

void init_uVoc(uVoc *u, float nomW, float nomV, float delWmax, float delVmax, float Prated, float Qrated, float Tp, float Tq, float pqFiltBwHz, float samPeriod, bool calcParam);
void update_uVoc(uVoc *u, abVector *vg, abVector *iout, float P0, float Q0);

void init_vsmControl(vsmCtrl *u, float nomW, float nomV, float delWmax, float delVmax, float Prated, float Qrated, float Tp, float Tq, float pqFiltBwHz, float samPeriod, bool calcParam);
void update_vsmControl(vsmCtrl *u, abVector *vg, abVector*iout, float P0, float Q0);

#endif /* INCLUDE_PRIMARYCONTROL_H_ */
