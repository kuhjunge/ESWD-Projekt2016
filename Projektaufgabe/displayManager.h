/*
 * File:   display.h
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 27. November 2016, 13:57
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "displayControl.h"
#include "timeControl.h"
#include "debugFunctions.h"

#define DISPLAY_TOGGLE_TIME_FIRST 5
#define DISPLAY_TOGGLE_TIME_SECOND 10

/************************************************************************/
/* Initialisiert die Display Komponenten                                */
/************************************************************************/
void initDisplay(void);

/************************************************************************/
/* Setzt die Anzeige im normalen Anzeige Modus                          */
/************************************************************************/
void setDisplay(measuringSet_t ms, state_t stateOfSystem);

/************************************************************************/
/* Setzt die Anzeige im Konfigurationsmodus                             */
/************************************************************************/
void setConfStepDisp(display_t displayMode, uint8_t val);

#endif /* DISPLAY_H */

