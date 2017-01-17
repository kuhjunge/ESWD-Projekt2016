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

// Definieren von Pins
#define PINUP 0
#define PINDOWN 3
#define PINENTER 4
#define PINCANCEL 5

// Definieren von Buttons
#define BUTTON_PORT PORTA
#define BUTTON_DDR DDRA
#define BUTTON_PIN PINA
#define BUTTON_INIT 0b11001100;

/************************************************************************/
/* Initialisiert die Buttonports an der HW und die benötigten Variablen */
/************************************************************************/
void initButton(void);

/************************************************************************/
/* Prüft, ob ein Button gepresst wurde. Bei wiederholtem Aufruf wird    */
/* der zuletzt geprüfte Button Zustand überschrieben, falls inzwischen  */
/* ein neuer Button gedrueckt wurde.                                    */
/************************************************************************/
uint8_t isPressed(void);

/************************************************************************/
/* Gibt den zuletzt gedrueckten Button zurück,							*/
/* gibt 'none' zurück wenn keiner gedrueckt wurde.						*/
/* Konsumiert die letzte Buttoneingabe									*/
/************************************************************************/
button_t getButton(void);

#endif /* BUTTONCONTROL_H */

