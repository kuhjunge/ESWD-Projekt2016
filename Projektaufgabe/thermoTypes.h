/* 
 * File:   thermoTypes.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 13:37
 */

#ifndef THERMOTYPES_H
#define THERMOTYPES_H

#include "types.h"
#if SIMULATOR < 1
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

typedef enum {
    none, enter, cancel, up, down, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9
} button_t;

typedef enum {
    dispTime, dispTimeTemp, dispTempHum, dispTimetemphum, confH, confM, confS, confDisplay
} display_t;

typedef struct stime {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} smhTime_t;

typedef struct measuringSet {
    int16_t temp; // -55 to +125 °C
    uint8_t hum;
    smhTime_t time;
} measuringSet_t;

typedef struct state {
    display_t displayMode;
    uint8_t readIntervall;
    smhTime_t time;
} state_t;


#endif /* THERMOTYPES_H */

