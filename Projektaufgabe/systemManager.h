/*
 * File:   systemManager.h
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 6. Dezember 2016, 13:08
 */

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "climaControl.h"
#include "displayManager.h"
#include "buttonControl.h"
#include "debugFunctions.h"

#define DEFAULT_SPEED 1 // Abfragegeschwindigkeit der Sensoren in Sekunden
#define MAX_SPEED 11 // Langsamste Abfragegeschwindigkeit (bei 11, maximal 10)

/************************************************************************/
/* Startet die Ausfuehrung des Programms (mit Endlosschleife)           */
/************************************************************************/
void startThermo(void);

#endif /* SYSTEMMANAGER_H */

