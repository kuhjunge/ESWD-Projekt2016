/*
* File:   buttonControl.h
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 15:19
*
* Hier werden die Buttons ENTER; CANCEL; UP; DOWN per polling abgefragt.
* InitButton muss vor der Verwendung von isPressed und getButton aufgerufen werden.
*/

#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include "thermoTypes.h"

// Definieren von Pins
#define PINUP 0
#define PINDOWN 3
#define PINENTER 4
#define PINCANCEL 5

#if SIMULATOR < 1
#define BUTTON_PORT PORTA
#define BUTTON_DDR DDRA
#define BUTTON_PIN PINA
#define BUTTON_INIT 0b11001100;
#endif

/************************************************************************/
/* Initialisiert die Buttonports an der HW und benötigte Variablen      */
/************************************************************************/
void initButton(void);

/************************************************************************/
/* Prüft ob ein Button gepresst wurde. Bei wiederholten Aufruf wird der */
/* zuletzt geprüfte Button Zustand überschrieben, sollte inzwischen ein */
/* neuer Button gedrueckt worden sein                                   */
/************************************************************************/
uint8_t isPressed(void);

/************************************************************************/
/* gibt den zuletzt gepressten Button zurück, gibt 'none' zurück wenn   */
/* keiner gepresst wurde - konsumiert die letzte Buttoneingabe			*/
/************************************************************************/
button_t getButton(void);

#endif /* BUTTONCONTROL_H */

