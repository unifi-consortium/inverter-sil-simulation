/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         controlParameters.h
 *      @ Description:  control parameters
 ********************************************************************************/

#ifndef INCLUDE_CONTROLPARAMETERS_H_
#define INCLUDE_CONTROLPARAMETERS_H_

#include "constants.h"
#include "systemRatings.h"

            /* Sampling rate */
#define ISR_FREQ                                25000.0                                 /* [Hz] */
#define ISR_PERIOD                              (1.0/ISR_FREQ)                          /* [s]  */

            /* Current Control */
#define invSideInductor                         0.0006                                  /* [H] */
#define gridSideInductor                        0.001                                   /* [H] */
#define totalInductor                           (invSideInductor + gridSideInductor)    /* [H] */

#define ccBandwidth                             (ISR_FREQ*0.04)                        /* [Hz] */
#define Kpcc0                                   (2.0*PI*ccBandwidth*totalInductor)      /* [V] */
#define Krcc10                                  (Kpcc0/0.004)                           /* [V] */
#define Krcc30                                  (Kpcc0/0.125)                           /* [V] */
#define Krcc50                                  (Kpcc0/0.33)                            /* [V] */
#define Krcc70                                  (Kpcc0/0.5)                             /* [V] */
#define wbIr1                                   (2.0*PI*0.5)                            /* [V] */
#define wbIr3                                   (2.0*PI*1.0)                            /* [V] */
#define wbIr5                                   (2.0*PI*1.0)                            /* [V] */
#define wbIr7                                   (2.0*PI*1.0)                            /* [V] */
#define w0                                      (NOM_FREQ)                              /* [rad/s] */
#define w3                                      (w0*3.0)                                /* [rad/s] */
#define w5                                      (w0*5.0)                                /* [rad/s] */
#define w7                                      (w0*7.0)                                /* [rad/s] */

        /* Primary/Synchronizing control */
#define MAX_FREQ_DELTA                          (NOM_FREQ*0.01)                         /* [rad/s] */
#define MAX_VOLTAGE_DELTA                       (NOM_VOLTAGE_LN*0.05)                   /* [V] */
#define pqCalcFilterBw_Hz                       10.0                                    /* [Hz] */

        /* Virtual admittance : run-state */
#define rsLv                                    (L_BASE*0.25)                           /* [H] */
#define rsRv                                    (Z_BASE*0.075)                          /* [Ohm] */

        /* Pre-synchronization control */
#define psLv                                    (L_BASE*0.05)                           /* [H] */
#define psRv                                    (Z_BASE*0.035)                          /* [Ohm] */
#define Kppsp                                   (1.0)                                   /* [W/W] */
#define Kipsp                                   (Kppsp/0.02)                            /* [/s] */
#define Kppsq                                   (1.0)                                   /* [W/W] */
#define Kipsq                                   (Kppsq/0.02)                            /* [/s] */

        /* Timing definitions */
#define TIME_STANDBY_FOR_STABLE_GRID            (ISR_FREQ*0.005)                         /* [# of control cycles] */
#define TIME_STANDBY_FOR_PRE_SYNCHRONIZATION    (ISR_FREQ*0.005)                         /* [# of control cycles] */

        /* VSM Control Parameters */
#define H                                       (0.5)                                    /* inertial time-constant [s] */
#define Texc                                    (0.2)                                    /* time-constant for excitation control [s] */ 

#endif /* INCLUDE_CONTROLPARAMETERS_H_ */
