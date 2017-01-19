/*
* buttonControl.c
*
* Created: 19.12.2016 17:47:29
*  Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*/

#include "buttonControl.h"

// Definiert ob polling für die Button Erkennung im Code genutzt wird, oder Interrupts
#define POLLING TRUE

button_t lastPressed = none; // Speichert den letzten Knopfdruck

#if POLLING == TRUE

unsigned char stateButtons = 0; // Speichert den Status der aktuellen Knoepfe

// ------------------- Definition der Hilfsfunktionen -------------------

/************************************************************************/
/* Gibt das Bit einer bestimmten Stelle im Char zurück                  */
/************************************************************************/
char getBit(char id, int position);

/************************************************************************/
/* Wechselt die Wertigkeit eines bestimmten Bits im Char                */
/************************************************************************/
char toggleBit(char var, char n, char x);

/************************************************************************/
/* Setzt die Variablen nach einer Eingabe							    */
/************************************************************************/
void doChange(char pin, int number);

/************************************************************************/
/* Prueft, ob sich die Eingabe für einen Button geaendert hat und       */
/* veranlasst doChange()												*/
/************************************************************************/
void checkAndDoChange(char pin, int number);

/************************************************************************/
/* Prüft, ob ein Button gedrueckt wurde für ENTER; CANCEL; UP; DOWN	    */
/************************************************************************/
void lookForPressedButton();

#else

// ------------------- Interrups -------------------
ISR(PCINT0_vect)
{
	if ((BUTTON_PIN & (1<<PINENTER))){
		lastPressed = enter;
	}
}

uint8_t b;
uint8_t c;
uint8_t d;
uint8_t e;
uint8_t f;
ISR(PCINT1_vect)
{
b = BUTTON_PIN;
c = PINCANCEL;
d = !(BUTTON_PIN & (1<<PINCANCEL));
e = 1<<PINCANCEL;
f = (BUTTON_PIN & (1<<PINCANCEL));
	if ((BUTTON_PIN & (1<<PINCANCEL))){
		lastPressed = cancel;
	}
}

ISR(PCINT2_vect)
{
	if ((BUTTON_PIN & (1<<PINUP))){
		lastPressed = up;
	}
}

ISR(PCINT3_vect)
{
	if ((BUTTON_PIN & (1<<PINDOWN))){
		lastPressed = down;
	}
}

#endif

// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header                                                         */
/************************************************************************/
void initButton(void) {
	BUTTON_PORT &= BUTTON_INIT;
	BUTTON_DDR &= BUTTON_INIT;
	lastPressed = none;
	
#if POLLING == FALSE
	cli();
	PCICR |= (1 << PCIE0) | (1 << PCIE1) | (1 << PCIE2) | (1 << PCIE3); 
	PCMSK0  |= (1 << PINENTER);    // set PCINT4 (enter) to trigger an interrupt on state change 
	PCMSK1  |= (1 << PINCANCEL);
	PCMSK2  |= (1 << PINUP);
	PCMSK3  |= (1 << PINDOWN);
	sei();                    // turn on interrupts
#endif
}

/************************************************************************/
/* Siehe Header                                                         */
/************************************************************************/
uint8_t isPressed(void) {
#if POLLING == TRUE
	lookForPressedButton();
#endif
	if (lastPressed == none) {
		return FALSE;
		} else {
		return TRUE;
	}
}

/************************************************************************/
/* Siehe Header                                                         */
/************************************************************************/
button_t getButton(void) {
#if POLLING == TRUE
	if (lastPressed == none){
		lookForPressedButton();
	}
#endif
	button_t temp = lastPressed;
	lastPressed = none;
	return temp;
}

#if POLLING == TRUE

// --------------- Implementation der Helfer Funktionen ---------------

char getBit(char id, int position) {
	return (id >> position) & 1;
}

char toggleBit(char var, char n, char x) {
	return var ^= (-x ^ var) & (1 << n);
}

void doChange(char pin, int number) {
	if (pin != 0) {
		if (number == PINCANCEL) {
			lastPressed = cancel;
			} else if (number == PINENTER) {
			lastPressed = enter;
			} else if (number == PINUP) {
			lastPressed = up;
			} else if (number == PINDOWN) {
			lastPressed = down;
		}
	}
}

void checkAndDoChange(char pin, int number) {
	if (getBit(stateButtons, number) != pin) {
		stateButtons = toggleBit(stateButtons, number, pin);
		doChange(pin, number);
	}
}

void lookForPressedButton()
{
	checkAndDoChange(getBit(BUTTON_PIN, PINUP), PINUP);
	checkAndDoChange(getBit(BUTTON_PIN, PINDOWN), PINDOWN);
	checkAndDoChange(getBit(BUTTON_PIN, PINCANCEL), PINCANCEL);
	checkAndDoChange(getBit(BUTTON_PIN, PINENTER), PINENTER);
}

#endif