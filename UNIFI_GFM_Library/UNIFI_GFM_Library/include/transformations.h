/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         transformations.h
 *      @ Description:  type and function definitions for trasnformations
 ********************************************************************************/

#ifndef INCLUDE_TRANSFORMATIONS_H_
#define INCLUDE_TRANSFORMATIONS_H_

#include "variableTypes.h"

/* Function Prototypes */
void polarToCartesian(polarVector *uPolar, cartesianVector *uXY);
void cartesianToPolar(cartesianVector *uXY, polarVector *uPolar);

void abcToAlphaBeta(threePhaseVector *abc, abVector *alphaBeta);
void alphaBetaToAbc(abVector *alphaBeta, threePhaseVector *abc);

void alphaBetaToDq(abVector *alphaBeta, dqVector* dq, float theta);
void dqToAlphaBeta(dqVector* dq, abVector *alphaBeta, float theta);

void rotateVector(abVector *u, abVector *y, float rotAngle);

#endif /* INCLUDE_TRANSFORMATIONS_H_ */
