/*
* File:   climaControl.h
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 13:52
*/

#ifndef CLIMACONTROL_H
#define CLIMACONTROL_H

#include "thermoTypes.h"

#if SIMULATOR < 1

#define ANALOG_PORT PORTA
#define ANALOG_DDR DDRA
#define ANALOG_PIN_VALUE 0x04
#define ANALOG_PORT_VALUE 0b00000000
#define ANALOG_DDR_VALUE 0b00000000
#define ANALOG_MAX_PERCENT 100

/* ADCSRA [Datasheet S250]
* Bit 7	ADEN: ADC Enable
* Bit 6	ADSC: ADC Start Conversion
* Bit 5	ADATE: ADC Auto Trigger Enable
* Bit 4	ADIF: ADC Interrupt Flag
* Bit 3	ADIE: ADC Interrupt Enable
* Bits 2:0	ADPS2:0: ADC Prescaler Select Bits
*/
#define ADCSRA_VALUE 0b10000111

/* ADMUX [Datasheet S249]
* Bit 7:6	REFS1:0: Reference Selection Bits -> (11) Internal 2.56V Voltage Reference with external capacitor at AREF pin
* Bit 5	ADLAR: ADC Left Adjust Result
* Bits 4:0	MUX4:0: Analog Channel and Gain Selection Bits
*/
#define ADMUX_VALUE 0b01100010
#endif

/************************************************************************/
/* Funktion zur initialisierung der Klima Komponenten                   */
/************************************************************************/
void initClima(void);

/************************************************************************/
/* Gibt die Luftfeuchtigkeit in % zurück. Wertebereich: 0 - 100         */
/************************************************************************/
uint8_t getHum(void);

int16_t getTemp(void);

#endif /* CLIMACONTROL_H */

