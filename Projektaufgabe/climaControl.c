/*
* File:   displayControl.c
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 14:03
*/

#include "climaControl.h"

/************************************************************************/
/* Initialisiert die Hardware, die ben�tigt wird umd den				*/
/* "Luftfeuchtigkeitssensor" auszulesen                                 */
/************************************************************************/
void initHumidity(void);

/************************************************************************/
/* Funktion zur initialisierung der Klima Komponenten                   */
/************************************************************************/
void initClima(void) {
	initHumidity();
}

/************************************************************************/
/* Dokumentation im Funktionsprototyp in der Header Datei				*/
/* Author : Chris Deter													*/
/************************************************************************/
uint8_t getHum(void) {
	uint8_t result = 0;
	/* Start ADC Conversion */
	ADCSRA  |= (1<<ADSC);
	/* Wait till conversion is complete */
	while(!(ADCSRA & (1<<ADIF)));
	/* Read the ADC Result */
	result = ADCH;
	/* Clear ADC Conversion Interrupt Flag */
	ADCSRA  |= (1 << ADIF);
	result = result / 2;
	return (result > ANALOG_MAX_PERCENT) ? ANALOG_MAX_PERCENT : result;
}

int16_t getTemp(void) {
	return 255; // Simulator Default Value
}

/************************************************************************/
/* Dokumentation im Funktionsprototyp									*/
/* Author : Chris Deter													*/
/************************************************************************/
void initHumidity(void){
	cli();			// vor dem Initialisieren Interrupts aus!
	ANALOG_PORT |= ANALOG_PORT_VALUE; // Port A 2
	ANALOG_DDR |= ANALOG_DDR_VALUE;
	ADCSRA = ADCSRA_VALUE;
	ADMUX = ADMUX_VALUE;
	sei();			// Sicherstellen, dass Interrupts wieder an sind
}
