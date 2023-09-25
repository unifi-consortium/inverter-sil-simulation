/*********************************************************************************
 *      Grid-Forming Control Library created for UNIFI Consortium
 *      Author: M A Awal
 *      FREEDM Systems Center, North Carolina State University
 *  
 *      @ File:         systemRatings.h
 *      @ Description:  Inverter Ratings
 ********************************************************************************/

#ifndef INCLUDE_SYSTEMRATINGS_H_
#define INCLUDE_SYSTEMRATINGS_H_

#include "constants.h"

#define NOM_POWER               3000.0                                  /* [W] */
#define NOM_VAR                 2000.0                                  /* [VAR] */
#define NOM_VA                  3600.0                                  /* [VA] */
#define NOM_VOLTAGE_LN_RMS      120.0                                   /* [V] */
#define NOM_FREQ_HZ             60.0                                    /* [Hz] */
#define NOM_FREQ                (2.0*PI*NOM_FREQ_HZ)                    /* [rad/s] */
#define NOM_CURRENT_RMS         (NOM_POWER/(NOM_VOLTAGE_LN_RMS*3.0))    /* [A] */
#define NOM_VOLTAGE_LN          (NOM_VOLTAGE_LN_RMS*sqrt2)              /* [V] */
#define NOM_CURRENT             (NOM_CURRENT_RMS*sqrt2)                 /* [A] */
#define Z_BASE                  (NOM_VOLTAGE_LN_RMS/NOM_CURRENT_RMS)    /* [Ohm] */
#define L_BASE                  (Z_BASE/NOM_FREQ)                       /* [H] */

#define NOM_DC_BUS_VOLTAGE      380.0                                   /* [V] */
#define MAX_CURRENT_LIMIT       (NOM_CURRENT*1.2)                       /* [A] */

#endif /* INCLUDE_SYSTEMRATINGS_H_ */
