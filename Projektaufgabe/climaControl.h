/* 
 * File:   climaControl.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 13:52
 */

#ifndef CLIMACONTROL_H
#define CLIMACONTROL_H

#include "thermoTypes.h"

void initClima(void);

uint8_t getHum(void);

int16_t getTemp(void);

#endif /* CLIMACONTROL_H */

