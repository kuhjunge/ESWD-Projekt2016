/* 
 * File:   displayControl.c
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 14:03
 */

#include "climaControl.h"

void initClima(void) {
	HUMREADPORT  = 0x00;
	HUMREADDDR = 0x00;
}

uint8_t getHum(void) {
	#if SIMULATOR < 1
	uint8_t value = (ANALOG_INPUT_READ_BITMASK & PINA) * ANALOG_INPUT_SCALE;
		return (value > ANALOG_MAX_PERCENT) ? ANALOG_MAX_PERCENT : value;
	#else
		return 50; // Simulator Default Value
	#endif
}

int16_t getTemp(void) {
    return 255; // TODO
}