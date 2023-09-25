/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         vectorControl.c
 *      @ Description:  vector control functions
 ********************************************************************************/

#include <controlParameters.h>
#include "systemRatings.h"
#include "variableTypes.h"
#include "compensators.h"
#include "transformations.h"
#include "primaryControl.h"
#include "vectorControl.h"
#include "math.h"

/* Internal Data */
threePhaseVector    vg_abc;                 /* Three phase PCC voltage (L-N) */
threePhaseVector    io_abc;                 /* Three phase output current */
abVector            vg_xy;                  /* PCC voltage in stationary alpha-beta frame */
abVector            io_xy;                  /* Output current in stationary alpha-beta frame */
float               vDcBus;                 /* DC bus voltage */
int                 fwStatusWord;           /* Firmware-status word */
int                 swStatusWord = 0;       /* Software status word */
int                 faultId = 0;            /* Fault Id */

firstOrderFilter    YpsA, YpsB;             /* Virtual admittance used during pre-synchronization state*/
firstOrderFilter    YrsA, YrsB;             /* Virtual admittance used under run-state */

fsmState            psState;                /* power-stage state */

piCompensator       psPI_P;                 /* P Axis: PI compensator for pre-synchronization */
piCompensator       psPI_Q;                 /* Q Axis: PI compensator for pre-synchronization */

float               Pref, Qref;             /* Real and reactive power references */

gfmControl          sGfmCtrl;               /* Grid-forming control structure */

resCompensator      ialphaResComp1;         /* Resonant current compensators */
resCompensator      ialphaResComp3;
resCompensator      ialphaResComp5;
resCompensator      ialphaResComp7;

resCompensator      ibetaResComp1;
resCompensator      ibetaResComp3;
resCompensator      ibetaResComp5;
resCompensator      ibetaResComp7;


float               Kpcc = (float) Kpcc0;   /* Current control gains */
float               Krcc1 = (float) Krcc10;
float               Krcc3 = (float) Krcc30;
float               Krcc5 = (float) Krcc50;
float               Krcc7 = (float) Krcc70;

/* Initialization task for vector control */
void stop_vectorControl(bool initialize){

    /* Virtual admittance for pre-sync operation */
    init_firstOrderFilter(&YpsA, 0.0, 1.0, (float) psLv, (float) psRv, (float) ISR_PERIOD, initialize); /* alpha-axis */
    init_firstOrderFilter(&YpsB, 0.0, 1.0, (float) psLv, (float) psRv, (float) ISR_PERIOD, initialize); /* beta-axis */

    /* PI compensators for power reference adjustment under pre-synchronization operation */
    init_piCompensator(&psPI_P, (float) Kppsp, (float) Kipsp, - (float) NOM_POWER, (float) NOM_POWER, (float) ISR_PERIOD);  /* Real power */
    init_piCompensator(&psPI_Q, (float) Kppsq, (float) Kipsq, - (float) NOM_POWER, (float) NOM_POWER, (float) ISR_PERIOD);  /* Reactive power */

    /* Primary/synchronizing controller */
    init_gfm_control(&sGfmCtrl, NOM_FREQ, NOM_VOLTAGE_LN, MAX_FREQ_DELTA, MAX_VOLTAGE_DELTA, (float) NOM_POWER, (float) NOM_VAR, 
                        (float) H, (float) Texc, pqCalcFilterBw_Hz, (float) ISR_PERIOD, initialize);

    /* Virtual admittance for pre-sync operation */
    init_firstOrderFilter(&YrsA, 0.0, 1.0, (float) rsLv, (float) rsRv, (float) ISR_PERIOD, initialize); /* alpha-axis */
    init_firstOrderFilter(&YrsB, 0.0, 1.0, (float) rsLv, (float) rsRv, (float) ISR_PERIOD, TRUE); /* beta-axis */

    /* Resonant current compensators */
    init_resonantCompensator(&ialphaResComp1, (float) wbIr1, (float) w0, (float) ISR_PERIOD, initialize);
    init_resonantCompensator(&ialphaResComp3, (float) wbIr3, (float) w3, (float) ISR_PERIOD, initialize);
    init_resonantCompensator(&ialphaResComp5, (float) wbIr5, (float) w5, (float) ISR_PERIOD, initialize);
    init_resonantCompensator(&ialphaResComp7, (float) wbIr7, (float) w7, (float) ISR_PERIOD, initialize);

    init_resonantCompensator(&ibetaResComp1, (float) wbIr1, (float) w0, (float) ISR_PERIOD, initialize);
    init_resonantCompensator(&ibetaResComp3, (float) wbIr3, (float) w3, (float) ISR_PERIOD, initialize);
    init_resonantCompensator(&ibetaResComp3, (float) wbIr5, (float) w5, (float) ISR_PERIOD, initialize);
    init_resonantCompensator(&ibetaResComp3, (float) wbIr7, (float) w7, (float) ISR_PERIOD, initialize);

}


void update_vectorControl(feedbacks *fb, controlOutputs *ctrlOut, float *dbIn, float *dbOut){

    /* Local variables */
    float               halfDcBus;
    float               psP, psQ;
    abVector            virAdmDrop;
    abVector            iRef = {0.0, 0.0};
    abVector            iErr = {0.0, 0.0};
    polarVector         iRefPolar;
    abVector            vMod_xy;
    threePhaseVector    mMod;
    float               mMin, mMax, mCm;

    /* Copy feedback variables to internal variables */
    vg_abc.a        = (2.0/3.0)*fb->vg_ab + (1.0/3.0)*fb->vg_bc;
    vg_abc.b        = (1.0/3.0)*(- fb->vg_ab + fb->vg_bc);
    vg_abc.c        = - (1.0/3.0)*fb->vg_ab - (2.0/3.0)*fb->vg_bc;

    io_abc.a        = fb->ig_a;
    io_abc.b        = fb->ig_b;
    io_abc.c        = fb->ig_c;

    vDcBus          = fb->vdc;

    fwStatusWord    = fb->fwStatus;

    /* Update debug inputs */
    /* x = *(dbIn + 0);
     * 
     * y = *(dbIn + 1);
     * .......
     * .......
     */

    /* Transformation to stationary alpha-beta frame */
    abcToAlphaBeta(&vg_abc, &vg_xy);
    abcToAlphaBeta(&io_abc, &io_xy);

    if(fwStatusWord & FW_STATE_READY){

        if((psState == runState) || (psState == preSynchronizationState)){

            /* Update GFM controller */
            update_gfm_control(&sGfmCtrl, &vg_xy, &io_xy, Pref, Qref);
        }

        /* Voltage drop across virtual admittance */
        virAdmDrop.alpha    = sGfmCtrl.v.alpha - vg_xy.alpha;
        virAdmDrop.beta     = sGfmCtrl.v.beta - vg_xy.beta;

        if(psState == preSynchronizationState){

            /* Update virtual admittance */
            update_firstOrderFilter(&YpsA, virAdmDrop.alpha);
            update_firstOrderFilter(&YpsB, virAdmDrop.beta);

            /* Override output current feedback */
            io_xy.alpha = YpsA.y;
            io_xy.beta  = YpsB.y;

            /* Calculate virtual power-flow if power stage were run at this time instant */
            psP     = 1.5*(sGfmCtrl.v.alpha*io_xy.alpha + sGfmCtrl.v.beta*io_xy.beta);
            psQ     = 1.5*(sGfmCtrl.v.beta*io_xy.alpha - sGfmCtrl.v.alpha*io_xy.beta);

            /* Update power reference compensation */
            update_piCompensator(&psPI_P, 0.0, psP, 0.0);
            update_piCompensator(&psPI_Q, 0.0, psQ, 0.0);

            /* Update power references */
            Pref    = psPI_P.x;
            Qref    = psPI_Q.x;
        }

        if(psState == runState){

            /* Update virtual admittance - for run state (PWM enabled) */
            update_firstOrderFilter(&YrsA, virAdmDrop.alpha);
            update_firstOrderFilter(&YrsB, virAdmDrop.beta);

            /* Update current reference */
            iRef.alpha  = YrsA.y;
            iRef.beta   = YrsB.y;

            /* Limit current reference magnitude */
            cartesianToPolar((cartesianVector *)&iRef, (polarVector *)&iRefPolar);
            iRefPolar.r = min(iRefPolar.r, (float) MAX_CURRENT_LIMIT );
            polarToCartesian((polarVector *)&iRefPolar, (cartesianVector *)&iRef);

            /* Current reference tracking error */
            iErr.alpha  = iRef.alpha - io_xy.alpha;
            iErr.beta   = iRef.beta - io_xy.beta;

            /* Update resonant compensators */
            update_resonantCompensator(&ialphaResComp1, iErr.alpha);
            update_resonantCompensator(&ialphaResComp3, iErr.alpha);
            update_resonantCompensator(&ialphaResComp5, iErr.alpha);
            update_resonantCompensator(&ialphaResComp7, iErr.alpha);
            update_resonantCompensator(&ibetaResComp1, iErr.beta);
            update_resonantCompensator(&ibetaResComp3, iErr.beta);
            update_resonantCompensator(&ibetaResComp5, iErr.beta);
            update_resonantCompensator(&ibetaResComp7, iErr.beta);

            /* Calculate current control output voltage */
            vMod_xy.alpha   = Kpcc*iErr.alpha + Krcc1*ialphaResComp1.y[0] + Krcc3*ialphaResComp3.y[0] + Krcc5*ialphaResComp5.y[0] + Krcc7*ialphaResComp7.y[0];
            vMod_xy.beta    = Kpcc*iErr.beta +  Krcc1*ibetaResComp1.y[0] + Krcc3*ibetaResComp3.y[0] + Krcc5*ibetaResComp5.y[0] + Krcc7*ibetaResComp7.y[0];

            /* Update modulation signal */
            halfDcBus       = (vDcBus*0.5);
            vMod_xy.alpha   = vMod_xy.alpha/halfDcBus;
            vMod_xy.beta    = vMod_xy.beta/halfDcBus;

            /* Transform into abc frame */
            alphaBetaToAbc(&vMod_xy, &mMod);

            /* Calculate CM for SVPWM */
            mMin            = min3( mMod.a, mMod.b, mMod.c );
            mMax            = max3( mMod.a, mMod.b, mMod.c );
            mCm             = - 0.5*( mMin + mMax );

            /* Update control output - modulating duty ratio */
            ctrlOut->m_u    = mMod.a + mCm;
            ctrlOut->m_v    = mMod.b + mCm;
            ctrlOut->m_w    = mMod.c + mCm;

            /* Enable PWM */
            if(ctrlOut->pwmEnable == 0){
                ctrlOut->pwmEnable = 1;
            }
        }

    }
    else{
        psState     = stopState;
    }

    /* Update finite state machine */
    psFSM(ctrlOut);

    /* Update debug outputs */
    *(dbOut)        = (float) psState;
    *(dbOut + 1)    = (float) iErr.alpha;//iErr.alpha;//*ialphaResComp1.y[0];
    *(dbOut + 2)    = (float) iErr.beta;
    *(dbOut + 3)    = (float) 0.0;
    *(dbOut + 4)    = (float) 0.0;
    *(dbOut + 5)    = (float) ialphaResComp1.b2;
    *(dbOut + 6)    = 0.0;

}


/* Update dispatch parameters and status monitors */
void update_dispatchMonitor(disParam *dispatch, statusMon *status){

    static float Pflt = 0.0;
    static float Qflt = 0.0;
    float P, Q;

    /* Update status monitors */
#ifndef UVOC
    status->freq        = sGfmCtrl.W;
    status->vol         = sGfmCtrl.V;
#else
    status->freq        = sGfmCtrl.W0;
    status->vol         = (float) NOM_VOLTAGE_LN;
#endif

    P                   = 1.5*(vg_xy.alpha * io_xy.alpha + vg_xy.beta * io_xy.beta);
    Q                   = 1.5*(- vg_xy.beta * io_xy.alpha + vg_xy.alpha * io_xy.beta);

    Pflt                = Pflt + (1.0/20.0)*(P - Pflt);                             /* exponential moving average over 20 samples */
    Qflt                = Qflt + (1.0/20.0)*(Q - Qflt);
    status->outP        = Pflt;
    status->outQ        = Qflt;
    status->statusWord  = swStatusWord;

    /* Update dispatch parameters */
    Pref                = dispatch->nomP;
    Qref                = dispatch->nomQ;
    sGfmCtrl.W0         = dispatch->nomFreq;
    sGfmCtrl.V0         = dispatch->nomVol;

#ifdef DROOP_CONTROL
    sGfmCtrl.mp         = dispatch->pwDroopCoeff;
    sGfmCtrl.mq         = dispatch->qvDroopCoeff;
#endif

}

void psFSM(controlOutputs *ctrlOut){
    static double   fsmTimer = 0;
    float           gridVoltageMag;

    gridVoltageMag  = vg_xy.alpha*vg_xy.alpha + vg_xy.beta*vg_xy.beta;
    gridVoltageMag  = sqrt(gridVoltageMag);

    switch(psState) {
        case stopState :
            stop_vectorControl(FALSE);

            if(fwStatusWord & FW_STATE_READY){

                if(gridVoltageMag > 0.9*NOM_VOLTAGE_LN){
                    fsmTimer++;
                }

                if(fsmTimer >= TIME_STANDBY_FOR_STABLE_GRID){
                    psState     = preSynchronizationState;
                    fsmTimer    = 0;
                }

            }
            swStatusWord |= (SW_STATE_PS_STATUS_BITS & 0x01);
            break;

        case preSynchronizationState :

            if(fsmTimer >= TIME_STANDBY_FOR_PRE_SYNCHRONIZATION){
                psState         = runState;
                fsmTimer        = 0;
                Pref = 0.0;
                Qref = 0.0;
            }
            else{
                if( ( fabs(psPI_P.e) < (0.01*NOM_POWER) ) && ( fabs(psPI_Q.e) < (0.01*NOM_VAR) ) ){
                    fsmTimer++;
                }
            }
            swStatusWord        |= (SW_STATE_PS_STATUS_BITS & 0x02);
            break;

        case runState :
            /*if(( fabs(io_abc.a) > OC_SW_LIMIT ) || ( fabs(io_abc.b) > OC_SW_LIMIT ) || ( fabs(io_abc.c) > OC_SW_LIMIT )) */
            swStatusWord        |= (SW_STATE_PS_STATUS_BITS & 0x03);
            break;

        case faultState :
            ctrlOut->pwmEnable  = 0;
            psState             = stopState;
            swStatusWord        |= (SW_STATE_PS_STATUS_BITS & 0x04);
            break;

    }

}
