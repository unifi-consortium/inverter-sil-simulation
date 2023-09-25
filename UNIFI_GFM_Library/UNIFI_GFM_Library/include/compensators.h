/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         interface.h
 *      @ Description:  type & function definitions for compensators 
 ********************************************************************************/

#ifndef INCLUDE_COMPENSATORS_H_
#define INCLUDE_COMPENSATORS_H_

#include "variableTypes.h"

/* Macro defines */
#define min(x,y) ((x) < (y)) ? (x) : (y);
#define max(x,y) ((x) > (y)) ? (x) : (y);

#define SATURATE(x, MIN, MAX)   \
    x = (x > MAX) ? MAX : x;    \
    x = (x < MIN) ? MIN : x;


/* compensator object structures */

/***************************************************************************
 *              PI Compensator with Anti-Windup and Feedforward
 * ************************************************************************/
typedef struct {
    float e;
    float x;
    float xPreSat;
    float xIntegral;
    float xMin;
    float xMax;
    signed char sat;
    float Kp;
    float KiT;
}piCompensator;


/***************************************************************************
 *                              PR Compensator
 * ************************************************************************/
typedef struct{
    float x[3];
    float y[3];
    float b0;
    float b1;
    float b2;
    float a1;
    float a2;
}resCompensator;

/***************************************************************************
 *                        First-Order Low-Pass Filter
 * ************************************************************************/
typedef struct{
    float coeff;
    float out;
} lowPassFilter;

/***************************************************************************
 *                             First-Order Filter
 * ************************************************************************/
typedef struct{
    float x1;
    float y;
    float b0;
    float b1;
    float a1;
}firstOrderFilter;

/* Function Prototypes */
void init_piCompensator(piCompensator *v, float Kp, float Ki, float min, float max, float Ts);
void update_piCompensator(piCompensator *v, float r, float y, float FF);

void init_resonantCompensator(resCompensator *v, float Wb, float Wn, float Ts, bool calcParam);
void update_resonantCompensator(resCompensator *v, float in);

void init_lowPassFilter(lowPassFilter *u, float initCondition, float bandwidth, float Ts);
void update_lowPassFilter(lowPassFilter *u, float in);

void init_firstOrderFilter(firstOrderFilter *v, float B0, float B1, float A0, float A1, float Tsamp, bool calcParam);
void update_firstOrderFilter(firstOrderFilter *v, float in);

float min3(float x, float y, float z);
float max3(float x, float y, float z);

#endif /* INCLUDE_COMPENSATORS_H_ */
