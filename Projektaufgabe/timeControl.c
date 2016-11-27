/* 
 * File:   main.c
 * Author: Kuhjunge
 *
 * Created on 23. November 2016, 18:07
 */

#include "thermoTypes.h"
#if SIMULATOR > 0
#include <time.h>
#endif
smhTime_t sysTime;

#if SIMULATOR > 0
time_t mastertime;
#endif

void tick(void){
    #if SIMULATOR > 0
    time_t curtime;

    /* Get the current time. */
    curtime = time (NULL);

    if (curtime != mastertime){
        mastertime = curtime;
    #endif
    sysTime.second++;
    if (sysTime.second > 59){
        sysTime.second = 0;
        sysTime.minute++;
        if (sysTime.minute > 59){
         sysTime.minute = 0;
         sysTime.hour++;
          if (sysTime.hour > 23){
          sysTime.hour = 0;
          }
        }
    }
    #if SIMULATOR > 0
    }
    #endif
}

void initTime(void){
    sysTime.hour = 0;
    sysTime.minute = 0;
    sysTime.second = 0;
    #if SIMULATOR > 0
    mastertime = time (NULL);
    #endif
}

smhTime_t getTime(smhTime_t* t){
    return sysTime;
}

void setTime(uint8_t h,uint8_t m,uint8_t s){
    sysTime.hour = h;
    sysTime.minute = m;
    sysTime.second = s;
}