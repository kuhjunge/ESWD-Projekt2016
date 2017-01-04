/* 
 * File:   timeControl.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 15:16
 */

#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include "thermoTypes.h"

void initTime(void);

smhTime_t getTime(void);

void setTime(uint8_t h, uint8_t m, uint8_t s);

#endif /* TIMECONTROL_H */

