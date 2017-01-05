/*
* buttonControl.c
*
* Created: 19.12.2016 17:47:29
*  Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*/

#include "buttonControl.h"

button_t lastPressed = none;
unsigned char stateButtons = 0; // Speichert den Status der aktuellen Kn�pfe
/************************************************************************/
/* Gibt das Bit einer bestimmten Stelle im Char zur�ck                  */

/************************************************************************/
char getBit(char id, int position) {
	return (id >> position) & 1;
}

/************************************************************************/
/* Wechselt die Wertigkeit eines bestimmten Bits im Char                */

/************************************************************************/
char toggleBit(char var, char n, char x) {
	return var ^= (-x ^ var) & (1 << n);
}

/************************************************************************/
/* Setzt die Variablen nach einer Eingabe							    */

/************************************************************************/
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

/************************************************************************/
/* Pr�ft, ob sich die Eingabe ge�ndert hat und veranlasst doChange()    */
/* Via Polling															*/

/************************************************************************/
void checkAndDoChange(char pin, int number) {
	if (getBit(stateButtons, number) != pin) {
		stateButtons = toggleBit(stateButtons, number, pin);
		doChange(pin, number);
	}
}

#if SIMULATOR < 1
void initButton(void) {
	BUTTON_PORT &= BUTTON_INIT;
	BUTTON_DDR &= BUTTON_INIT;
	lastPressed = none;
}

uint8_t isPressed(void) {
	checkAndDoChange(getBit(BUTTON_PIN, PINUP), PINUP);
	checkAndDoChange(getBit(BUTTON_PIN, PINDOWN), PINDOWN);
	checkAndDoChange(getBit(BUTTON_PIN, PINCANCEL), PINCANCEL);
	checkAndDoChange(getBit(BUTTON_PIN, PINENTER), PINENTER);
	if (lastPressed == none) {
		return FALSE;
		} else {
		return TRUE;
	}
}


#else

void initButton(void){
	
}

uint8_t isPressed(void){
	lastPressed = cancel;//enter;
	return TRUE;
}

#endif

button_t getButton(void) {
	button_t temp = lastPressed;
	lastPressed = none;
	return temp;
}