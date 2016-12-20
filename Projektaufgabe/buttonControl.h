/* 
 * File:   buttonControl.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 15:19
 */

#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include "thermoTypes.h"

// Definieren von Pins
#define PINUP 0
#define PINDOWN 3
#define PINENTER 4
#define PINCANCEL 5

#if SIMULATOR < 1
#define BUTTON_PORT PORTA
#define BUTTON_DDR DDRA
#define BUTTON_PIN PINA
#define BUTTON_INIT 0b11001100;
#endif

void initButton(void);

uint8_t isPressed(void);

button_t getButton(void);

#endif /* BUTTONCONTROL_H */

