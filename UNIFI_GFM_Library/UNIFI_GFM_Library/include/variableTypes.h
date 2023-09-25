/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         variableTypes.h
 *      @ Description:  Type definitions for variables & structures
 ********************************************************************************/

#ifndef INCLUDE_VARIABLETYPES_H_
#define INCLUDE_VARIABLETYPES_H_

typedef unsigned char bool;

typedef struct{
    float alpha;
    float beta;
} abVector;

typedef struct{
    float d;
    float q;
} dqVector;

typedef struct{
    float a;
    float b;
    float c;
} threePhaseVector;

typedef struct{
    float r;
    float theta;
} polarVector;

typedef struct{
    float x;
    float y;
} cartesianVector;

#endif /* INCLUDE_VARIABLETYPES_H_ */
