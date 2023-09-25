/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         compensators.c
 *      @ Description:  compensator functions 
 ********************************************************************************/

#include "compensators.h"
#include "constants.h"
/* External Data */

/* Internal Data */

/* Function Definitions */
void init_piCompensator(piCompensator *v, float Kp, float Ki, float min, float max, float Ts)
{
    v->e = 0.0;
    v->x = 0.0;
    v->xIntegral = 0.0;
    v->Kp = Kp;
    v->KiT = Ki*Ts;
    v->xMin = min;
    v->xMax = max;
    v->sat = 0;
    v->xPreSat = 0.0;
}

void update_piCompensator(piCompensator *v, float r, float y, float FF)
{
    v->e = r - y;
    if((v->sat < 0 && v->e < 0) || (v->sat > 0 && v->e > 0)){
        // do nothing
    }
    else{
        v->xIntegral = v->xIntegral + v->KiT*v->e;
    }

    v->xPreSat = v->Kp*v->e + v->xIntegral + FF;
    v->x = (v->xPreSat > v->xMax) ? v->xMax : v->xPreSat;
    v->x = (v->x < v->xMin) ? v->xMin : v->x;

    if(v->xPreSat >= v->xMax){
        v->sat = 1;
    }
    else if(v->xPreSat <= v->xMin){
        v->sat = -1;
    }
    else{
        v->sat = 0;
    }
}

void init_resonantCompensator(resCompensator *v, float Wb, float Wn, float Ts, bool calcParam)
{
    float x, y, den;

    if(calcParam == TRUE){
        x = 2.0*Wb*Ts;
        y = Wn*Ts;
        y = y*y;        // y = (Wn*Ts)^2

        den = x + y + 4.0;

        v->b0 = x/den;

        v->b1 = 0.0;

        v->b2 = - v->b0;

        v->a1 = 2.0*(4.0 - y);
        v->a1 = v->a1/den;

        v->a2 = (x - y - 4.0);
        v->a2 = v->a2/den;
    }

    v->x[0] = 0.0;
    v->x[1] = 0.0;
    v->x[2] = 0.0;

    v->y[0] = 0.0;
    v->y[1] = 0.0;
    v->y[2] = 0.0;
}

void update_resonantCompensator(resCompensator *v, float in)
{
    v->x[0] = in;
    v->y[0] = v->b0*v->x[0] + v->b1*v->x[1] + v->b2*v->x[2] + v->a1*v->y[1] + v->a2*v->y[2]; // a1 and a2 are defined as negative
    v->y[2] = v->y[1];
    v->y[1] = v->y[0];
    v->x[2] = v->x[1];
    v->x[1] = v->x[0];
}

void init_lowPassFilter(lowPassFilter *u, float initCondition, float bandwidth, float Ts){
    u->coeff    = 2.0*PI*bandwidth*Ts; 
    u->out      = initCondition;
}

void update_lowPassFilter(lowPassFilter *u, float in){
    u->out = u->out + u->coeff*(in - u->out);
}

void init_firstOrderFilter(firstOrderFilter *v, float B0, float B1, float A0, float A1, float Tsamp, bool calcParam)
{
    float x;

    //if(calcParam == TRUE){
        x = Tsamp*A1 + 2.0*A0;
        v->a1 = (Tsamp*A1 - 2.0*A0)/x;
        v->b0 = (Tsamp*B1 + 2.0*B0)/x;
        v->b1 = (Tsamp*B1 - 2.0*B0)/x;
    //}

    v->x1   = 0.0;
    v->y    = 0.0;
}

void update_firstOrderFilter(firstOrderFilter *v, float in){
    v->y = - v->a1*v->y + v->b0*in + v->b1*v->x1;
    v->x1 = in;
}

float min3(float x, float y, float z){
    float u;

    u = min(x, y);
    u = min(u, z);

    return u;
}

float max3(float x, float y, float z){
    float u;

    u = max(x, y);
    u = max(u, z);

    return u;
}
