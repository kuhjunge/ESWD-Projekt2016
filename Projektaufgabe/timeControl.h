/*
* File:   timeControl.h
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 15:16
*/

#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include "thermoTypes.h"

#define MAX_HOURS 24
#define MAX_MINUTES 60
#define MAX_SECONDS 60

/************************************************************************/
/* Initialisiert die Zeit Funktionialität								*/
/* (Interrupt und die nötigen Variablen)								*/
/************************************************************************/
void initTime(void);

/************************************************************************/
/* Gibt die aktuelle Zeit zurück (im smhTime_T struct)                  */
/************************************************************************/
smhTime_t getTime(void);

/************************************************************************/
/* Setzt eine neue Zeit mit Stunde, Minute, Sekunde als Parameter		*/
/************************************************************************/
void setTime(uint8_t h, uint8_t m, uint8_t s);

#endif /* TIMECONTROL_H */

