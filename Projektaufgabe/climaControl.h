/* 
 * File:   climaControl.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 13:52
 */

#ifndef CLIMACONTROL_H
#define CLIMACONTROL_H

#include "thermoTypes.h"

#if SIMULATOR < 1

#define HUMREADPORT PORTA
#define HUMREADDDR DDRA
#define ANALOG_INPUT_READ_BITMASK 0x1F
#define ANALOG_INPUT_SCALE 3
#define ANALOG_MAX_PERCENT 100
#endif

void initClima(void);

uint8_t getHum(void);

int16_t getTemp(void);

#endif /* CLIMACONTROL_H */

