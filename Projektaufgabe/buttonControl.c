/*
 * buttonControl.c
 *
 * Created: 19.12.2016 17:47:29
 *  Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 */

#include "buttonControl.h"

button_t lastPressed = none; // Speichert den letzten Knopfdruck

#if POLLING == TRUE

unsigned char stateButtons = 0; // Speichert den Status der aktuellen Knoepfe

// ------------------- Definition der Hilfsfunktionen -------------------

char getBit(char id, int position);

char toggleBit(char var, char n, char x);

void doChange(char pin, int number);

void checkAndDoChange(char pin, int number);

void lookForPressedButton();

#else

// ------------------- Interrupts -------------------

ISR(PCINT0_vect) {
    if ((BUTTON_PIN & (1 << PINENTER))) { // Lese Enter ein, wenn gedrueckt
        lastPressed = enter;
    }
    if ((BUTTON_PIN & (1 << PINCANCEL))) { // Lese Cancel ein, wenn gedrueckt
        lastPressed = cancel;
    }
    if ((BUTTON_PIN & (1 << PINUP))) { // Lese Up ein, wenn gedrueckt
        lastPressed = up;
    }
    if ((BUTTON_PIN & (1 << PINDOWN))) { // Lese Down ein, wenn gedrueckt
        lastPressed = down;
    }
}

#endif

// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Initialisiert die Buttonports an der HW und die benoetigten Variablen*/
/************************************************************************/
void initButton(void) {
    BUTTON_PORT &= BUTTON_INIT;
    BUTTON_DDR &= BUTTON_INIT;
    lastPressed = none;

#if POLLING == FALSE
    cli();
    PCICR |= (1 << PCIE0);
    // set PCINT4 (enter) to trigger an interrupt on state change
    PCMSK0 = (1 << PINENTER) | (1 << PINCANCEL) | (1 << PINUP) | (1 << PINDOWN); 
    sei(); // turn on interrupts
#endif
}

/************************************************************************/
/* Prueft, ob ein Button gedrueckt wurde. Bei wiederholtem Aufruf wird  */
/* der zuletzt gepruefte Button-Zustand ueberschrieben, falls inzwischen*/
/* ein neuer Button gedrueckt wurde.                                    */
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
/* Gibt den zuletzt gedrueckten Button zurueck,                         */
/* gibt 'none' zurueck wenn keiner gedrueckt wurde.						*/
/* Konsumiert die letzte Buttoneingabe									*/
/************************************************************************/
button_t getButton(void) {
#if POLLING == TRUE
    if (lastPressed == none) {
        lookForPressedButton();
    }
#endif
    button_t temp = lastPressed;
    lastPressed = none;
    return temp;
}

#if POLLING == TRUE

// --------------- Implementation der Hilfsfunktionen ---------------


/************************************************************************/
/* Gibt das Bit einer bestimmten Stelle im Char zurueck                 */
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
/* Setzt die Variablen nach einer Eingabe								*/
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
/* Prueft, ob sich die Eingabe fuer einen Button geaendert hat und      */
/* veranlasst doChange()												*/
/************************************************************************/
void checkAndDoChange(char pin, int number) {
    if (getBit(stateButtons, number) != pin) {
        stateButtons = toggleBit(stateButtons, number, pin);
        doChange(pin, number);
    }
}

/************************************************************************/
/* Prueft, ob ein Button gedrueckt wurde fuer ENTER; CANCEL; UP; DOWN	*/
/************************************************************************/
void lookForPressedButton() {
    checkAndDoChange(getBit(BUTTON_PIN, PINUP), PINUP);
    checkAndDoChange(getBit(BUTTON_PIN, PINDOWN), PINDOWN);
    checkAndDoChange(getBit(BUTTON_PIN, PINCANCEL), PINCANCEL);
    checkAndDoChange(getBit(BUTTON_PIN, PINENTER), PINENTER);
}

#endif