/* 
 * File:   displayControl.c
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 14:03
 */

#include "test.h"

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    //*p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void initDisplay(void){
    
}

void setDisplay(measuringSet_t ms, display_t displayMode){
    char temp[4] ="   ";
    char arr[9] = "        ";
    char arr2[9] = "        ";
    itoa(ms.temp, temp);
    arr2[4] = temp[0];
    arr2[5] = temp[1];
    arr2[6] = temp[2];
    dispSet(arr,arr2);
}

void setConfStepDisp(display_t displayMode, uint8_t val){
    
}

