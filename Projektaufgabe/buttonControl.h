/*
 * File:   buttonControl.h
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 27. November 2016, 15:19
 *
 * Hier werden die Buttons ENTER; CANCEL; UP; DOWN per Polling abgefragt.
 * InitButton muss vor der Verwendung von isPressed und getButton aufgerufen werden.
 */

#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include "thermoTypes.h"

// Definiert ob polling fuer die Button Erkennung im Code genutzt wird, oder Interrupts
// Wenn FALSE = werden Interrupts zur Verarbeitung der Eingabe verwendet
#define POLLING FALSE

// Definieren von Pins
#define PINUP PCINT0
#define PINDOWN PCINT3
#define PINENTER PCINT4
#define PINCANCEL PCINT5

// Definieren von Buttons
#define BUTTON_PORT PORTA
#define BUTTON_DDR DDRA
#define BUTTON_PIN PINA
#define BUTTON_INIT 0b11000110;

/************************************************************************/
/* Initialisiert die Buttonports an der HW und die benoetigten Variablen*/
/************************************************************************/
void initButton(void);

/************************************************************************/
/* Prueft, ob ein Button gepresst wurde. Bei wiederholtem Aufruf wird   */
/* der zuletzt gepruefte Button Zustand ueberschrieben, falls inzwischen*/
/* ein neuer Button gedrueckt wurde.                                    */
/************************************************************************/
uint8_t isPressed(void);

/************************************************************************/
/* Gibt den zuletzt gedrueckten Button zurueck,                         */
/* gibt 'none' zurueck wenn keiner gedrueckt wurde.						*/
/* Konsumiert die letzte Buttoneingabe									*/
/************************************************************************/
button_t getButton(void);

#endif /* BUTTONCONTROL_H */

