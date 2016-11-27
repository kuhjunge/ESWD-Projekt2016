/* 
 * File:   main.c
 * Author: Kuhjunge
 *
 * Created on 23. November 2016, 18:07
 */

#include <stdlib.h>
#include "thermoTypes.h"
#include "climaControl.h"
#include "test.h"
/*
 * 
 */

int main(int argc, char** argv) {
    measuringSet_t ms;
    display_t disp = temphum;
    ms.temp = getTemp();
    setDisplay(ms, disp);
    return (EXIT_SUCCESS);
}

