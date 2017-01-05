/*
* File:   displayManager.c
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 14:03
*/

#include "displayManager.h"

int toggleDisp = 0;

char* itoa(int i, char b[]);

char* toDRight(char b[], uint8_t fill);

void setDTemp(char b[], int16_t tmpVal);

char* setDVal(char b[], int16_t tmpVal, uint8_t offset, uint8_t size, uint8_t withZero);

void setDText(char b[], char text[]);

void setDHum(char b[], uint8_t tmpVal);

void setDTime(char b[], smhTime_t* t);

void setConfigTimeS(char b[], uint8_t tmpVal);

void setConfigDisp(char b[], uint8_t tmpVal);

void initDisplay(void) {
	initDisp();
}

void setDisplay(measuringSet_t ms, display_t displayMode) {
	char arr[DISPLAY_ARRAY_SIZE] =  "                ";
	char arr2[DISPLAY_ARRAY_SIZE] = "                ";
	setDisplayDebugLED(displayMode);
	if (displayMode == dispTime) {
		setDTime(arr, &ms.time);
	}
	else if (displayMode == dispTimeTemp) {
		setDTime(arr, &ms.time);
		setDTemp(arr2, ms.temp);
	}
	else if (displayMode == dispTempHum) {
		setDTemp(arr, ms.temp);
		setDHum(arr2, ms.hum);
	}
	else {
		if (toggleDisp < 6){
			setDTime(arr, &ms.time);
			toggleDisp++;
			setDisplayDebugLED(dispTime);
		}
		else {
			setDTemp(arr, ms.temp);
			setDHum(arr2, ms.hum);
			toggleDisp++;
			if (toggleDisp > 10) {
				toggleDisp = 0;
			}
			setDisplayDebugLED(dispTempHum);
		}
	}
	dispSet(arr, arr2);
}

void setConfStepDisp(display_t displayMode, uint8_t val) {
	char arr[DISPLAY_ARRAY_SIZE] = "CONFIGURATION:  ";
	char arr2[DISPLAY_ARRAY_SIZE] = "               ";
	setDisplayDebugLED(displayMode);
	// confH, confM, confS, confDisplay
	if (displayMode == confDisplay) {
		setConfigDisp(arr2, val);
	}
	else if (displayMode == confH) {
		setConfigTimeS(arr2, val);
	}
	else if (displayMode == confM) {
		setConfigTimeS(arr2, val);
	}
	else { // confS
		setConfigTimeS(arr2, val);
	}
	dispSet(arr, arr2);
}

void setDTime(char b[], smhTime_t* t) {
	char text[DISPLAY_ARRAY_SIZE] = "TIME:     :  :  ";
	setDText(b, text);
	setDVal(b, t->hour, 8, 2, TRUE);
	setDVal(b, t->minute, 11, 2, TRUE);
	setDVal(b, t->second, 14, 2, TRUE);
}

void setDTemp(char b[], int16_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE] = "TEMP:         *C";
	setDText(b, text);
	setDVal(b, tmpVal, 9, 4, FALSE);
}

void setDHum(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE] = "FEUCHTE:       %";
	setDText(b, text);
	setDVal(b, tmpVal, 12, 3, FALSE);
}

void setConfigDisp(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE] = "MODE:           ";
	setDText(b, text);
	setDVal(b, tmpVal, 10, 2, TRUE);
}

void setConfigTimeS(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE] = "TIME:        Sec";
	setDText(b, text);
	setDVal(b, tmpVal, 10, 2, TRUE);
}

void setDText(char b[], char text[]) {
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
