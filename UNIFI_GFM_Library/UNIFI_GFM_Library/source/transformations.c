/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         transformations.c
 *      @ Description:  reference frame transformations 
 ********************************************************************************/

#include "math.h"
#include "transformations.h"
#include "constants.h"

/* External Data */

/* Internal Data */

/* Function Definitions */
void polarToCartesian(polarVector *uPolar, cartesianVector *uXY)
{
	uXY->x	= uPolar->r*cos(uPolar->theta);
	uXY->y	= uPolar->r*sin(uPolar->theta);
}

void cartesianToPolar(cartesianVector *uXY, polarVector *uPolar){
	uPolar->r 	    = uXY->x*uXY->x + uXY->y*uXY->y;
	uPolar->r 	    = sqrt(uPolar->r);
	uPolar->theta	= atan2(uXY->y, uXY->x);
}

void abcToAlphaBeta(threePhaseVector *abc, abVector *alphaBeta)
{
	alphaBeta->alpha = (float) (2.0/3.0)*abc->a - (float) (1.0/3.0)*(abc->b + abc->c); 
	alphaBeta->beta  = (float) (1.0/sqrt3)*(abc->b - abc->c);
}

void alphaBetaToAbc(abVector *alphaBeta, threePhaseVector *abc)
{
	abc->a = alphaBeta->alpha;
	abc->b = -(float) (0.5)*alphaBeta->alpha + (float) (0.5*sqrt3)*alphaBeta->beta;
	abc->c = -(float) (0.5)*alphaBeta->alpha - (float) (0.5*sqrt3)*alphaBeta->beta;
}

void alphaBetaToDq(abVector *alphaBeta, dqVector* dq, float theta)
{
    dq->d = alphaBeta->alpha*cos(theta) + alphaBeta->beta*sin(theta);
    dq->q = - alphaBeta->alpha*sin(theta) + alphaBeta->beta*cos(theta);
}

void dqToAlphaBeta(dqVector* dq, abVector *alphaBeta, float theta){
    alphaBeta->alpha    = dq->d*cos(theta) - dq->q*sin(theta);
    alphaBeta->beta     = dq->d*sin(theta) + dq->q*cos(theta);
}

void rotateVector(abVector *u, abVector *y, float rotAngle){
    y->alpha    = u->alpha*cos(rotAngle) + u->beta*sin(rotAngle);
    y->beta     = - u->alpha*sin(rotAngle) + u->beta*cos(rotAngle);
}

