/*
* File:   timeControl.c
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 23. November 2016, 18:07
*/

#include "thermoTypes.h"

smhTime_t sysTime;

void timer1Init () {
	cli();
	TCCR1A = 0x00;		//CTC ON
	TCCR1B = 0x0D;		//Prescaler 1024; Grundfreq = 1 MHz / 1024 = 977Hz
	// mit Fuse CLKDIV8 gesetzt sind es effektiv 1MHz
	
	// Konfigurieren der "Output Compare Units"
	OCR1A = 977 * 8;		// 1.000.000 / 1024 = 977 fuer eine Sekunde
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	TCNT1 = 0x00;		// Zaehlregister des Timers noch auf Null stellen
	sei();
}

/************************************************************************/
/* Compare Interrupt A													*/
/* schaltet die PortA-LED ein und die Port B - LED aus					*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	TCNT1 = 0x00; // Timer auf Null
	sysTime.second++;
	if (sysTime.second > 59) {
		sysTime.second = 0;
		sysTime.minute++;
		if (sysTime.minute > 59) {
			sysTime.minute = 0;
			sysTime.hour++;
			if (sysTime.hour > 23) {
				sysTime.hour = 0;
			}
		}
	}
}

void initTime(void) {
	sysTime.hour = 0;
	sysTime.minute = 0;
	sysTime.second = 0;
	timer1Init();
}

smhTime_t getTime(smhTime_t* t) {
	return sysTime;
}

void setTime(uint8_t h, uint8_t m, uint8_t s) {
	sysTime.hour = h;
	sysTime.minute = m;
	sysTime.second = s;
}