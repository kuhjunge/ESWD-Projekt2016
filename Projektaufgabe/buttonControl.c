/*
* buttonControl.c
*
* Created: 19.12.2016 17:47:29
*  Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*/

#include "buttonControl.h"

button_t lastPressed = none; // Speichert den letzten Knopfdruck
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

// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header                                                         */
/************************************************************************/
void initButton(void) {
	BUTTON_PORT &= BUTTON_INIT;
	BUTTON_DDR &= BUTTON_INIT;
	lastPressed = none;
}

/************************************************************************/
/* Siehe Header                                                         */
/************************************************************************/
uint8_t isPressed(void) {
	lookForPressedButton();

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
	if (lastPressed == none){
		lookForPressedButton();
	}
	button_t temp = lastPressed;
	lastPressed = none;
	return temp;
}

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