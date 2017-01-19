/*
 * File:   timeControl.c
 * Author: Chris Deter
 *
 * Created on 23. November 2016, 18:07
 */

#include "timeControl.h"

smhTime_t sysTime; // Die Variable fuer die Systemzeit

 // Variable um minimal im Interrupt die Zeit weiterlaufen zu lassen
volatile uint8_t tick;

// ------------------ Definition der Helfer Funktionen ------------------

/************************************************************************/
/* Initialisiert den Timer Interrupt                                    */
/************************************************************************/
void timer1Init(void);

/************************************************************************/
/* Compare Interrupt A                                                  */
/* setzt die Zeit eine Sekunde weiter                                   */
/************************************************************************/
ISR(TIMER1_COMPA_vect);

// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header	                                                        */
/************************************************************************/
void initTime(void) {
    sysTime.hour = 0;
    sysTime.minute = 0;
    sysTime.second = 0;
    tick = 0;
    timer1Init();
}

/************************************************************************/
/* Siehe Header                                                         */
/************************************************************************/
smhTime_t getTime(void) {
    while (tick > 0) {
        tick--;
        sysTime.second++;
        if (sysTime.second > MAX_SECONDS - 1) {
            sysTime.second = 0;
            sysTime.minute++;
            if (sysTime.minute > MAX_MINUTES - 1) {
                sysTime.minute = 0;
                sysTime.hour++;
                if (sysTime.hour > MAX_HOURS - 1) {
                    sysTime.hour = 0;
                }
            }
        }
    }
    return sysTime;
}

/************************************************************************/
/* Siehe Header                                                         */
/************************************************************************/
void setTime(uint8_t h, uint8_t m, uint8_t s) {
    sysTime.hour = h;
    sysTime.minute = m;
    sysTime.second = s;
    tick = 0;
}

// --------------- Implementation der Hilfsfunktionen ---------------

void timer1Init(void) {
    cli();
    TCCR1A = 0x00; //CTC ON
    TCCR1B = 0x0D; //Prescaler 1024; Grundfreqenz = 1 MHz / 1024 = 977Hz
    // mit Fuse CLKDIV8 gesetzt sind es effektiv 1MHz
    // Konfigurieren der "Output Compare Units"
    OCR1A = 977 * 8; // 1.000.000 / 1024 = 977 fuer eine Sekunde
    // Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
    TIMSK1 = (1 << 1); // Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
    TCNT1 = 0x00; // Zaehlregister des Timers noch auf 0 stellen
    sei();
}

ISR(TIMER1_COMPA_vect) {
    TCNT1 = 0x00; // Timer auf 0
    tick++;
}