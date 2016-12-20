/* 
 * File:   main.c
 * Author: Kuhjunge
 *
 * Created on 23. November 2016, 18:07
 */
#include "climaControl.h"
#include "displayManager.h"
#include "buttonControl.h"

state_t systemState;
measuringSet_t ms;

void init(void) {
    initTime();
    initClima();
    initDisplay();
    initButton();
}

measuringSet_t update(void) {
    ms.temp = getTemp();
    ms.hum = getHum();
    ms.time = getTime();
    return ms;
}

void thermoMode(void) {
    uint8_t goToConfigMode = FALSE;
    button_t b;
    while (!goToConfigMode) {
        tick();
        update();
        setDisplay(ms, systemState.displayMode);
        if (isPressed() == TRUE) {
            b = getButton();
            if (b == enter) {
                goToConfigMode = TRUE;
            }
        }
    }
}

void configMode(void) {
    uint8_t goToThermoMode = FALSE;
    button_t b;
    while (!goToThermoMode) {
        setDisplay(ms, systemState.displayMode);
        if (isPressed() == TRUE) {
            b = getButton();
            if (b == cancel) {
                goToThermoMode = TRUE;
            }
        }
    }
}

void startThermo(void) {
    init();
    systemState.displayMode = dispTimeTemp;
    systemState.readIntervall = 1;
    while (1) {
        tick();
        systemState.time = getTime();
        // Check for Buttons
        thermoMode();
        configMode();
    }
}