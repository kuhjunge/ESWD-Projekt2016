/* 
 * File:   displayControl.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 14:01
 */

#ifndef DISPLAYCONTROL_H
#define DISPLAYCONTROL_H

#include "thermoTypes.h"

#define DISPLAY_ARRAY_SIZE 17

void initDisp(void);

void dispSet(char topRow[], char bottomRow[]);

#endif /* DISPLAYCONTROL_H */