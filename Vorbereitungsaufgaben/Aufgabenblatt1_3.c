/*
 * Aufgabenblatt1_3.c
 *
 * Created: 25.10.2016 17:12:34
 *  Author: Chris Deter, Alexandra Scheben, Dirk Teschner
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>		// interrupts

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

unsigned char state = 0; // Speichert den Status der aktuellen Kn�pfe
unsigned char bit = 0;

/************************************************************************/
/* Variablen f�r den Timer	                                            */
/************************************************************************/
volatile unsigned char timerIsRinging = FALSE;

/************************************************************************/
/* Wechselt die Wertigkeit eines bestimmten Bits im Char                */
/************************************************************************/
char toggleBit(char var, char n, char x)
{
	return var ^= (-x ^ var) & (1 << n);
}

/************************************************************************/
/* Gibt das Bit einer bestimmten Stelle im Char zur�ck                  */
/************************************************************************/
char getBit(char id, int position)
{
	// return id & (1 << position); // NULL or NON ZERO
   return (id >> position) & 1;
}

/************************************************************************/
/* Initialisieren von Timer1.                                           */
/************************************************************************/
void timer1Init () {
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
	OCR1A = 977 * 0.5;		// 1.000.000 / 1024 = 977 fuer eine Sekunde
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	//	TIMSK1 |= (1<<2);	// Bit 2 ? OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	
	TCNT1 = 0x00;		// Zaehlregister des Timers noch auf Null stellen
}

/************************************************************************/
/* Stopt den Timer                                                      */
/************************************************************************/
void timer1Stop(){
	TCCR1B = 0x08;
}

void wait(){
	timerIsRinging	= FALSE;
	
	cli();			// vor dem Initialisieren Interrupts aus!
	timer1Init();
	sei();			// Sicherstellen, dass Interrupts wieder an sind
	
	while (!timerIsRinging);
	timer1Stop();
}

/************************************************************************/
/* Compare Interrupt A													*/
/* schaltet die PortA-LED ein und die Port B - LED aus					*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	timerIsRinging = TRUE;
}

void changeButton(char id, int position, int changeVal){
	bit = getBit(id, position);
	if (bit == 1){
		state += changeVal;
	} 
}

void showVal(void){
	PORTA = state;
}

int main(void)
{
	DDRA  = 0xFF;		// PORTA to output
	PORTA = 0x00;		// all LEDs off
	DDRC  = 0xF0;			// PORTC to input /output
	PORTC = 0x00;		// all LEDs off
	
	while(1)
	{
		// PC4 = OUT
		// && PC0 = 1  0x01| PC1 = 2 0x02 | PC2 = 3 0x03  | PC3 = A 0x0A 
		state = 0; 
		PORTC = N4;
			changeButton(PORTC, N0, 0x01);
			changeButton(PORTC, N1, 0x02);
			changeButton(PORTC, N2, 0x03);
			changeButton(PORTC, N3, 0x0A);
		// PC5 = OUT
		// && PC0 = 4 0x04 | PC1 = 5 0x05 | PC2 = 6 0x06 | PC3 = B 0x0B 
		PORTC = N5;
			changeButton(PORTC, N0, 0x04);
			changeButton(PORTC, N1, 0x05);
			changeButton(PORTC, N2, 0x06);
			changeButton(PORTC, N3, 0x0B);
		// PC6 = OUT
		// && PC0 = 7 0x07 | PC1 = 8 0x08 | PC2 = 9 0x09 | PC3 = C 0x0C 
		PORTC = N6;
			changeButton(PORTC, N0, 0x07);
			changeButton(PORTC, N1, 0x08);
			changeButton(PORTC, N2, 0x09);
			changeButton(PORTC, N3, 0x0C);
		// PC7 = OUT
		// && PC0 = E 0x0E | PC1 = 10 (A) 0x0A  | PC2 = F 0x0F  | PC3 = D 0x0D 
		PORTC = N7;
			changeButton(PORTC, N0, 0x0E);
			changeButton(PORTC, N1, 0x0A);
			changeButton(PORTC, N2, 0x0F);
			changeButton(PORTC, N3, 0x0D);
		showVal();
	}
}