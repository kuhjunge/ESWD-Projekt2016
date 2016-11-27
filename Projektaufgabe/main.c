/* 
 * File:   main.c
 * Author: Kuhjunge
 *
 * Created on 23. November 2016, 18:07
 */

#include <stdlib.h>
#include "thermoTypes.h"
#include "climaControl.h"
#include "displayManager.h"
#include "timeControl.h"

/*
 * 
 */

int main(int argc, char** argv) {
    measuringSet_t ms;
    display_t disp = dispTimeTemp;
    initTime();
    while(1){
        tick();
        ms.temp = getTemp();
        ms.hum = getHum(); 
        ms.time = getTime();
       setDisplay(ms, disp);
    }
    return (EXIT_SUCCESS);
}

