/* 
 * File:   display.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 13:57
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "thermoTypes.h"
#include "displayControl.h"

void initDisplay(void);

void setDisplay(measuringSet_t ms, display_t displayMode);

void setConfStepDisp(display_t displayMode, uint8_t val);

#endif /* DISPLAY_H */

