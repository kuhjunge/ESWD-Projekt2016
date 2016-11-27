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

time_t getTime(void);

void setTime(time_t t);

#endif /* TIMECONTROL_H */

