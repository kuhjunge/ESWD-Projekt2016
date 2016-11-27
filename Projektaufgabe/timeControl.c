/* 
 * File:   main.c
 * Author: Kuhjunge
 *
 * Created on 23. November 2016, 18:07
 */

#include "thermoTypes.h"

smhTime_t sysTime;

void tick(void){
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
}

void initTime(void){
    sysTime.hour = 0;
    sysTime.minute = 0;
    sysTime.second = 0;
}

smhTime_t getTime(smhTime_t* t){
    return sysTime;
}

void setTime(uint8_t h,uint8_t m,uint8_t s){
    sysTime.hour = h;
    sysTime.minute = m;
    sysTime.second = s;
}