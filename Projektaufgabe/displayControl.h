/*
 * File:   displayControl.h
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 27. November 2016, 14:01
 */

#ifndef DISPLAYCONTROL_H
#define DISPLAYCONTROL_H

#include "thermoTypes.h"

// Maximale Groesse der Array im Controller des Displays
#define DISPLAY_ARRAY_SIZE_FOR_CONTROLLER 41

// Maximale Groesse der Arrays in Disp Set
#define DISPLAY_ARRAY_SIZE_FOR_SOFTWARE 17 

/************************************************************************/
/* Initialisiert den Display                                            */
/************************************************************************/
void initDisp(void);

/************************************************************************/
/* Gibt zwei Char Arrays auf dem Display aus                            */
/* topRow = Die obere Reihe an Zeichen                                  */
/* bottomRow = Die untere Reihe an Zeichen                              */
/************************************************************************/
void dispSet(char topRow[], char bottomRow[]);

#endif /* DISPLAYCONTROL_H */