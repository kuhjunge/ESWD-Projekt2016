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

#define ANALOG_PORT PORTA
#define ANALOG_DDR DDRA
#define ANALOG_PIN 0x04
#define ANALOG_MAX_PERCENT 100
#endif

void initClima(void);

uint8_t getHum(void);

int16_t getTemp(void);

#endif /* CLIMACONTROL_H */

