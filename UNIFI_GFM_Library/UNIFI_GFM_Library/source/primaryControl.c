/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         primaryControl.c
 *      @ Description:  primary control functions 
 ********************************************************************************/

#include "compensators.h"
#include "constants.h"
#include "primaryControl.h"
#include "transformations.h"
#include "variableTypes.h"

void init_droopControl(droopCtrl *u, float nomW, float nomV, float delWmax, float delVmax, 
                        float Prated, float Qrated, float Tp, float Tq, float pqFiltBwHz, float samPeriod, bool calcParam){

    if(calcParam == TRUE){
        init_lowPassFilter((lowPassFilter *)&(u->Pfilt), 0.0, pqFiltBwHz, samPeriod);
        init_lowPassFilter((lowPassFilter *)&(u->Qfilt), 0.0, pqFiltBwHz, samPeriod);

        u->V0       = nomV;
        u->W0       = nomW;
        u->mp       = delWmax/Prated;
        u->mq       = delVmax/Qrated;

        u->Ts       = samPeriod;
    }

	u->P0		= 0.0;
	u->Q0 		= 0.0;

	u->V		= nomV;
	u->W		= nomW;
	u->theta	= 0.0;
}

void update_droopControl(droopCtrl *u, abVector *vg, abVector *iout, float P0, float Q0){

    float P, Q;
	polarVector vPol;

	u->P0           = P0;
	u->Q0           = Q0;

	P = 1.5*(vg->alpha*iout->alpha + vg->beta*iout->beta);
	Q = 1.5*(vg->beta*iout->alpha  - vg->alpha*iout->beta);

    update_lowPassFilter((lowPassFilter *)&(u->Pfilt), P);
    update_lowPassFilter((lowPassFilter *)&(u->Qfilt), Q);

    u->W = u->W0 + u->mp*(u->P0 - u->Pfilt.out);
    u->V = u->V0 + u->mq*(u->Q0 - u->Qfilt.out);
	
	u->theta += u->W*u->Ts;

	if(u->theta > (float) 3.141592653589793){
		u->theta -= (float) 6.283185307179586;
	}

	vPol.r		= u->V;
	vPol.theta 	= u->theta;

	polarToCartesian((polarVector *)&vPol, (cartesianVector *)&(u->v));
}

void init_uVoc(uVoc *u, float nomW, float nomV, float delWmax, float delVmax, 
                float Prated, float Qrated, float Tp, float Tq, float pqFiltBwHz, float samPeriod, bool calcParam){
    float vMax, vMax2, v02;

    if(calcParam == TRUE){
        vMax        = nomV + delVmax;
        vMax2       = vMax*vMax;
        v02         = nomV*nomV;

        u->eta      = (3.0*delWmax*vMax2)/Prated;
        u->mu       = ((2.0/3.0)*u->eta*Qrated)/((2.0*vMax2 - v02)*(2.0*vMax2 - v02) - v02*v02);
        u->W0       = nomW;
        u->V0       = nomV;
        u->Tsam     = samPeriod;
        u->Thsam    = 0.5*samPeriod;
    }

    u->P0       = 0.0;
    u->Q0       = 0.0;

    u->v.alpha  = nomV;
    u->v.beta   = 0.0;
}

void update_uVoc(uVoc *u, abVector *vg, abVector *iout, float P0, float Q0){

    float Vp02, Vp2, ev, iAlphaErr, iBetaErr, muEvk;

    u->P0           = P0;
    u->Q0           = Q0;
    Vp02            = u->V0*u->V0;

    Vp2             = u->v.alpha*u->v.alpha + u->v.beta*u->v.beta;
    ev              = Vp02 - Vp2;

    iAlphaErr       = (2.0/3.0)*((u->v.alpha*u->P0 + u->v.beta*u->Q0)/Vp2) - iout->alpha;
    iBetaErr        = (2.0/3.0)*((u->v.beta*u->P0 - u->v.alpha*u->Q0)/Vp2) - iout->beta;

    muEvk           = u->mu*ev;

    u->delta1.alpha = - u->W0*u->v.beta + muEvk*u->v.alpha - u->eta*iBetaErr;
    u->delta1.beta  = u->W0*u->v.alpha + muEvk*u->v.beta + u->eta*iAlphaErr;

    u->v1.alpha     = u->v.alpha + u->Tsam*u->delta1.alpha;
    u->v1.beta      = u->v.beta + u->Tsam*u->delta1.beta;

    Vp2             = u->v1.alpha*u->v1.alpha + u->v1.beta*u->v1.beta;
    ev              = Vp02 - Vp2;

    iAlphaErr       = (2.0/3.0)*((u->v1.alpha*u->P0 + u->v1.beta*u->Q0)/Vp2) - iout->alpha;
    iBetaErr        = (2.0/3.0)*((u->v1.beta*u->P0 - u->v1.alpha*u->Q0)/Vp2) - iout->beta;

    muEvk           = u->mu*ev;

    u->delta2.alpha = - u->W0*u->v1.beta + muEvk*u->v1.alpha - u->eta*iBetaErr;
    u->delta2.beta  = u->W0*u->v1.alpha + muEvk*u->v1.beta + u->eta*iAlphaErr;

    u->v.alpha      = u->v.alpha + u->Thsam*(u->delta1.alpha + u->delta2.alpha);
    u->v.beta       = u->v.beta + u->Thsam*(u->delta1.beta + u->delta2.beta);
}

void init_vsmControl(vsmCtrl *u, float nomW, float nomV, float delWmax, float delVmax, 
                float Prated, float Qrated, float Tp, float Tq, float pqFiltBwHz, float samPeriod, bool calcParam){

    float eqPfiltBwHz; /* virtual-inertia equivalent filter bandwidth for P*/

    if(calcParam == TRUE){
        u->W0       = nomW;
        u->mp       = delWmax/Prated;
        eqPfiltBwHz = (1.0/(4.0*PI))*nomW/(delWmax*Tp); /* J = 2*Tp*Prated/W0^2, J*W0 = 1/(2*pi*eqPfiltBwHz*mp) */ 
        init_lowPassFilter((lowPassFilter *)&(u->Pfilt), 0.0, eqPfiltBwHz, samPeriod);
        
        u->V0       = nomV;
        u->Dq       = Qrated/delVmax;
        u->Kq       = nomV*samPeriod/(Qrated*Tq);
        init_lowPassFilter((lowPassFilter *)&(u->Qfilt), 0.0, pqFiltBwHz, samPeriod);

        u->Ts       = samPeriod;
    }

	u->P0		= 0.0;
	u->Q0 		= 0.0;

	u->V		= nomV;
	u->W		= nomW;
	u->theta	= 0.0;
}



void update_vsmControl(vsmCtrl *u, abVector *vg, abVector*iout, float P0, float Q0)
{
    float       P, Q;
	polarVector vPol;
	polarVector VpccPol;

	cartesianToPolar((cartesianVector *)vg, &VpccPol);

	u->P0           = P0;
	u->Q0           = Q0;

	P = 1.5*(vg->alpha*iout->alpha + vg->beta*iout->beta);
	Q = 1.5*(vg->beta*iout->alpha  - vg->alpha*iout->beta);

    update_lowPassFilter((lowPassFilter *)&(u->Pfilt), P);
    update_lowPassFilter((lowPassFilter *)&(u->Qfilt), Q);

    u->W = u->W0 + u->mp*(u->P0 - u->Pfilt.out);
    u->V += u->Kq*((u->V0 - VpccPol.r)*u->Dq + (u->Q0 - u->Qfilt.out));
	
	u->theta += u->W*u->Ts;

	if(u->theta > (float) 3.141592653589793){
		u->theta -= (float) 6.283185307179586;
	}

	vPol.r		= u->V;
	vPol.theta 	= u->theta;

	polarToCartesian((polarVector *)&vPol, (cartesianVector *)&(u->v));
}