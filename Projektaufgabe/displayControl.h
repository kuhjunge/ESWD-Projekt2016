/*
* File:   displayControl.h
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 14:01
*/

#ifndef DISPLAYCONTROL_H
#define DISPLAYCONTROL_H

#include "thermoTypes.h"

#define DISPLAY_ARRAY_SIZE_FOR_CONTROLLER 41
#define DISPLAY_ARRAY_SIZE_FOR_SOFTWARE 17

void initDisp(void);

void dispSet(char topRow[], char bottomRow[]);

#endif /* DISPLAYCONTROL_H */