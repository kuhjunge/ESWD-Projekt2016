/*
 * Aufgabenblatt1_3.c
 *
 * Created: 25.10.2016 17:12:34
 *  Author: Chris Deter, Alexandra Scheben, Dirk Teschner
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>		// interrupts
#include <util/delay.h>

// defines to be used as boolean
#define FALSE 0
#define TRUE !FALSE

#define N0  0x01 
#define N1  0x02 
#define N2  0x04 
#define N3  0x08 

#define N4  0x10 
#define N5  0x20 
#define N6  0x40 
#define N7  0x80 

unsigned char state = 0; // Speichert den Status der aktuellen Knöpfe
unsigned char bit = 0;


/************************************************************************/
/* Variablen für den Timer	                                            */
/************************************************************************/
volatile unsigned char timerIsRinging = FALSE;


/************************************************************************/
/* Gibt das Bit einer bestimmten Stelle im Char zurück                  */
/************************************************************************/
char getBit(char id, int position)
{
   return (id & position) == position;
}


/************************************************************************/
/* Initialisieren von Timer1.                                           */
/************************************************************************/
void timer1Init (float sec) {
	// Taktquelle ist per Default der Systemtakt, Asynchrones Verhalten
	// wird in separatem ASSR ? Asynchronous Status Register konfiguriert

	// Timer-Modus: Clear Timer on Compare match (CTC)
	// d.h Zaehlen bis Wert in OCR1A, dann Reset
	// Konfigbits dafuer in TCCR1A und TCCR1B verteilt
	// Prescaler steht in Bit 2..0 von TCCR1B
	TCCR1A = 0x00;		//CTC ON
	TCCR1B = 0x0D;		//Prescaler 1024; Grundfreq = 1 MHz / 1024 = 977Hz
	// mit Fuse CLKDIV8 gesetzt sind es effektiv 1MHz
	
	// Konfigurieren der "Output Compare Units"
	// (Vergleich des Zaehlerwertes auf Schwellwert --> Interrupt)
	//	OCR1BH = 0x00;		// 0,2*1.000.000 / 1024 = 195 fuer 0,2s (overflow compare)
	//	OCR1BL = 0xC3;		// 194 -> 0x00C3
	OCR1A = 977 * sec;		// 1.000.000 / 1024 = 977 fuer eine Sekunde
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	//	TIMSK1 |= (1<<2);	// Bit 2 ? OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	
	TCNT1 = 0x00;		// Zaehlregister des Timers noch auf Null stellen
}


/************************************************************************/
/* Compare Interrupt A													*/
/* schaltet die PortA-LED ein und die Port B - LED aus					*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	timerIsRinging = TRUE;
}


/************************************************************************/
/* Setzt den Timercounter zurück                                        */
/************************************************************************/
void resetWait(){
	TCNT1 = 0x00;		// Zaehlregister des Timers auf Null setzen
}


/************************************************************************/
/* Stopt den Timer                                                      */
/************************************************************************/
void cleanWait(){
	TCCR1B = 0x08;
}


/************************************************************************/
/* Wartet eine festgelegte Anzahl von Sekunden,							*/
/* zweites Argument bestimmt, ob der Aufruf blockierend ist oder nicht	*/
/************************************************************************/
void wait(float sec, unsigned char block){
	timerIsRinging	= FALSE;
	
	cli();			// vor dem Initialisieren Interrupts aus!
	timer1Init(sec);
	sei();			// Sicherstellen, dass Interrupts wieder an sind
	
	while (!timerIsRinging && block);
	if (block){
		cleanWait();
	}
}
void changeButton(char id, int position, int changeVal){
	// Pin einlesen
	bit = getBit(id, position);
	// state erhöhen, wenn entsprechende Taste gedrückt wurde
	if (bit == 1){
		state += changeVal;
	} 
}

void showVal(void){
	if (state > 0){
		PORTA = state;
		resetWait();
	} else {
		if (timerIsRinging == TRUE){
			PORTA = state;		
			timerIsRinging = FALSE;		
		}
	}
	
}

int main(void)
{
	DDRA  = 0xFF;		// PORTA = Output
	PORTA = 0x00;		// all LEDs off
	DDRC  = 0xF0;		// PC4 - PC7 = Output,     PC0 - PC3 = Input
	PORTC = 0x00;		// PC0 - PC7 = TRISTATE
	
	// Timer auf 1s setzen.
	// nötig für nachleuchten der LED's nachdem Tasten losgelassen wurde
	wait(1, FALSE);
	
	while(1)
	{
		state = 0x00;
		
		// PC4 = Input
		DDRC = N4;
		PORTC = N4;
		_delay_ms(1);
		
		// einzelne Pins einlesen und ggf. state erhöhen
		changeButton(PINC, N0, 0x01);
		changeButton(PINC, N1, 0x02);
		changeButton(PINC, N2, 0x03);
		changeButton(PINC, N3, 0x0A);
		
		// PC5 = Input
		DDRC = N5;
		PORTC = N5;
		_delay_ms(1);
		
		// einzelne Pins einlesen und ggf. state erhöhen
		changeButton(PINC, N0, 0x04);
		changeButton(PINC, N1, 0x05);
		changeButton(PINC, N2, 0x06);
		changeButton(PINC, N3, 0x0B);

		// PC6 = Input
		DDRC = N6;
		PORTC = N6;
		_delay_ms(1);
		
		// einzelne Pins einlesen und ggf. state erhöhen
		changeButton(PINC, N0, 0x07);
		changeButton(PINC, N1, 0x08);
		changeButton(PINC, N2, 0x09);
		changeButton(PINC, N3, 0x0C);

		// PC7 = Input
		DDRC = N7;
		PORTC = N7;
		_delay_ms(1);
		
		// einzelne Pins einlesen und ggf. state erhöhen
		changeButton(PINC, N0, 0x0E);
		changeButton(PINC, N1, 0x10);
		changeButton(PINC, N2, 0x0F);
		changeButton(PINC, N3, 0x0D);
			
		// Gedrückten Wert (state) anzeigen	
		showVal();
		_delay_ms(10);
	}
}