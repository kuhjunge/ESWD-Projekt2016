/*
* File:   thermoTypes.h
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 13:37
*/

#ifndef THERMOTYPES_H
#define THERMOTYPES_H

// defines to be used as boolean
#define FALSE 0
#define TRUE !FALSE

// F_CPU Notwendig damit delay mit 8Mhz arbeitet
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef unsigned char		uint8_t;
typedef signed char		int8_t;

/************************************************************************/
/* Enum zur Verwaltung der Button eingaben                              */
/************************************************************************/
typedef enum {
	none, enter, cancel, up, down //, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9
} button_t;

/************************************************************************/
/* Enum zum Verwaltung der möglichen Display Anzeigen                   */
/************************************************************************/
typedef enum {
	dispTime, dispTimeTemp, dispTempHum, dispTimeTempHum, confH, confM, confS, confDisplay, confOverview, confSpeed
} display_t;

/************************************************************************/
/* Enum für die Optionen im Configurationsmodus                         */
/************************************************************************/
typedef enum {
	confChoiceDisp, confChoiceTime, confChoiceSpeed, confChoiceExit
} configChoice_t;

// Todo Dirk (oder Alex)
typedef enum
{
	convert, read
} transaction_t;

/************************************************************************/
/* Struct zur Verwaltung einer Uhrzeit                                  */
/************************************************************************/
typedef struct stime {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} smhTime_t;

/************************************************************************/
/* Eine Messung                                                         */
/************************************************************************/
typedef struct measuringSet {
	int16_t temp; // -55 to +125 °C
	uint8_t hum;
	smhTime_t time;
} measuringSet_t;

/************************************************************************/
/* Status des Systems                                                   */
/************************************************************************/
typedef struct state {
	display_t displayMode;
	uint8_t readIntervall;
	smhTime_t time;
} state_t;

#endif /* THERMOTYPES_H */

