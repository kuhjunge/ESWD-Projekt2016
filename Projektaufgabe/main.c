/*
 * File:   main.c
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 23. November 2016, 18:07
 */

#include <stdlib.h>
#include "systemManager.h"

/************************************************************************/
/* Main, startet die Funktion,                                          */
/* welche den Kontrollflusses des Programmes ausfuehrt                  */
/************************************************************************/
int main(int argc, char** argv) {
    startThermo();
    return (EXIT_SUCCESS);
}

