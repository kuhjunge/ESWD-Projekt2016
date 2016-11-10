/*
 * TimerDemo.c
 *
 * Created: 31.10.2013 13:20:09
 *  Author: Ole Blaurock
 *
 * Demo how to use the Timer1 Interrupts
 * One Compare Interrupt is used to let an LED blink with 1Hz
 * The second compare Interrupt switches the LED off after 0.1s
 * The overflow-Interrupt Toggles another LED
 */ 

 // #defines zum Konfigurieren der IO-Pins

#define BLINKPORT PORTB
#define BLINKDDR DDRB

#define BLINKPORT2 PORTA
#define BLINKDDR2 DDRA

#define BLINKBIT 0


// propotypes
void timer1Init (void);

// --- System Bibliotheken ----------------------------------------------------
#include <avr/io.h>				// allg. IO
#include <avr/interrupt.h>		// interrupts
#include <inttypes.h>			// Datentypen mit Bitbreiten

int main(void)
{
    BLINKDDR=0xFF;	// Blinkport als Ausgang schalten
	BLINKDDR2=0xFF;	// Blinkport als Ausgang schalten
	cli();			// vor dem Initialisieren Interrupts aus!
	timer1Init();
	sei();			// Sicherstellen, dass Interrupts wieder an sind
	
	while(!PINC)
    {
        // hier passiert nichts, alles in den Interrupts
    }
	// alles aus
	BLINKPORT=0; 
	BLINKPORT2=0;
}

/************************************************************************/
/* Initialisieren von Timer1.                                           */
/************************************************************************/
void timer1Init (void) {
	// Taktquelle ist per Default der Systemtakt, Asynchrones Verhalten 
	// wird in separatem ASSR ? Asynchronous Status Register konfiguriert 

	// Timer-Modus: Clear Timer on Compare match (CTC) 
	// d.h Z?hlen bis Wert in OCR1A, dann Reset
	// Konfigbits daf?r in TCCR1A und TCCR1B verteilt
	// Prescaler steht in Bit 2..0 von TCCR1B
	TCCR1A = 0x00;		//CTC ON
	TCCR1B = 0x0D;		//Prescaler 1024; Grundfreq = 1 MHz / 1024 = 977Hz
						// mit Fuse CLKDIV8 gesetzt sind es effektiv 1MHz
	
	// Konfigurieren der "Output Compare Units" 
	// (Vergleich des Z?hlerwertes auf Schwellwert --> Interrupt)
	OCR1BH = 0x00;		// 0,2*1.000.000 / 1024 = 195 f?r 0,2s
	OCR1BL = 0xC3;		// 194 -> 0x00C3
	OCR1A = 977;		// 1.000.000 / 1024 = 977 f?r eine Sekunde 
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	TIMSK1 |= (1<<2);	// Bit 2 ? OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	
	TCNT1 = 0x00;		// Z?hlregister des Timers noch auf Null stellen
}

/************************************************************************/
/* Interrupt Service Routinen f?r Timer1								*/
/************************************************************************/

/* Compare Interrupt A													*/
/* schaltet die LED ein													*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	BLINKPORT |= (1<<BLINKBIT);
//	BLINKPORT2 &= ~(1<<BLINKBIT);
}

/* Compare Interrupt b													*/
/* schaltet die LED (wieder) aus											*/
/************************************************************************/
ISR (TIMER1_COMPB_vect) {
	BLINKPORT &= ~(1<<BLINKBIT);
//	BLINKPORT2 |= (1<<BLINKBIT);
}