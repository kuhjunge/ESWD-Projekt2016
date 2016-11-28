/* 
 * File:   displayControl.c
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 14:03
 */

#include "displayManager.h"

char* itoa(int i, char b[]);

char* toDRight(char b[], uint8_t fill);

char* setDTemp(char b[], int16_t tmpVal);

char* setDVal(char b[], int16_t tmpVal, uint8_t offset, uint8_t size, uint8_t withZero);

char* setDText(char b[], char text[]);

char* setDHum(char b[], uint8_t tmpVal);

char* setDTime(char b[], smhTime_t* t);

void initDisplay(void) {
    initDisp();
}

void setDisplay(measuringSet_t ms, display_t displayMode) {
    char arr[DISPLAY_ARRAY_SIZE] = "0123456789ABCDEF";
    char arr2[DISPLAY_ARRAY_SIZE] = "                ";
    if (displayMode == dispTime) {
        setDTime(arr, &ms.time);
    } else if (displayMode == dispTimeTemp) {
        setDTime(arr, &ms.time);
        setDTemp(arr2, ms.temp);
    } else if (displayMode == dispTempHum) {
        setDTemp(arr, ms.temp);
        setDHum(arr2, ms.hum);
    } else {
        setDTemp(arr2, ms.temp); // TODO wechselnde Anzeige
        setDTime(arr, &ms.time);
    }
    dispSet(arr, arr2);
}

void setConfStepDisp(display_t displayMode, uint8_t val) {

}

char* setDTime(char b[], smhTime_t* t) {
    char text[DISPLAY_ARRAY_SIZE] = "TIME:     :  :  ";
    setDText(b, text);
    setDVal(b, t->hour, 8, 2, TRUE);
    setDVal(b, t->minute, 11, 2, TRUE);
    setDVal(b, t->second, 14, 2, TRUE);
}

char* setDTemp(char b[], int16_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE] = "TEMP:         *C";
    setDText(b, text);
    setDVal(b, tmpVal, 9, 4, FALSE);
}

char* setDHum(char b[], uint8_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE] = "FEUCHTE:       %";
    setDText(b, text);
    setDVal(b, tmpVal, 12, 3, FALSE);
}

char* setDText(char b[], char text[]) {
    int i;
    for (i = 0; i < DISPLAY_ARRAY_SIZE; i++) {
        b[i] = text[i];
    }
}

char* setDVal(char b[], int16_t tmpVal, uint8_t offset, uint8_t size, uint8_t withZero) {
    char temp[5] = "    ";
    itoa(tmpVal, temp);
    toDRight(temp, size);
    b[offset] = temp[0];
    if (size > 1) {
        b[offset + 1] = temp[1];
    }
    if (size > 2) {
        b[offset + 2] = temp[2];
        if (temp[2] == ' '){
          b[offset + 2] = '0';  
        }
    }
    if (size > 3) {
        b[offset + 3] = ',';
        b[offset + 4] = temp[3];
    }
    if (withZero) {
        if (b[offset] == ' ') {
            b[offset] = '0';
        }
        if (b[offset + 1] == ' ') {
            b[offset + 1] = '0';
        }
    }
    return b;
}

char* toDRight(char b[], uint8_t fill) {
    char empty = ' ';
    int i;
    while (b[fill - 1] == empty) {
        for (i = fill - 2; i >= 0; i--) {
            b[i + 1] = b[i];
        }
        b[0] = empty;
    }
    return b;
}

char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;
    if (i < 0) {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do {
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    //*p = '\0';
    do {
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}
