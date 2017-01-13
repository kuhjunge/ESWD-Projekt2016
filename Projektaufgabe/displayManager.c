/*
* File:   displayManager.c
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 14:03
*/

#include "displayManager.h"

int toggleDisp = 0;
uint8_t seconds = 0;

char* itoa(int i, char b[]);

char* toDRight(char b[], uint8_t fill);

void setDTemp(char b[], int16_t tmpVal);

char* setDVal(char b[], int16_t tmpVal, uint8_t offset, uint8_t size, uint8_t withZero);

void setDText(char b[], char text[]);

void setDHum(char b[], uint8_t tmpVal);

void setDTime(char b[], smhTime_t* t);

void setConfigTimeH(char b[], uint8_t tmpVal);

void setConfigTimeM(char b[], uint8_t tmpVal);

void setConfigTimeS(char b[], uint8_t tmpVal);

void setConfigChoice(char b[], uint8_t tmpVal);

void setConfigDisp(char b[], uint8_t tmpVal);

void setConfigSpeed(char b[], uint8_t tmpVal);

char* getDispModeText(uint8_t tmpVal);

char* getSelectConfigText(uint8_t tmpVal);

void initDisplay(void) {
	initDisp();
}

void setDisplay(measuringSet_t ms, state_t stateOfSystem) {
	char arr[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] =  "                ";
	char arr2[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "                ";
	setDisplayDebugLED(stateOfSystem.displayMode);
	if (stateOfSystem.displayMode == dispTime) {
		setDTime(arr, &stateOfSystem.time);
	}
	else if (stateOfSystem.displayMode == dispTimeTemp) {
		setDTime(arr, &stateOfSystem.time);
		setDTemp(arr2, ms.temp);
	}
	else if (stateOfSystem.displayMode == dispTempHum) {
		setDTemp(arr, ms.temp);
		setDHum(arr2, ms.hum);
	}
	else {
			if (toggleDisp <= DISPLAY_TOGGLE_TIME_FIRST){
				setDTime(arr, &stateOfSystem.time);
				if (stateOfSystem.time.second != seconds){
					seconds = stateOfSystem.time.second;
					toggleDisp++;
				}
				setDisplayDebugLED(dispTime);
			} else {
				setDTemp(arr, ms.temp);
				setDHum(arr2, ms.hum);
				if (stateOfSystem.time.second != seconds){
					seconds = stateOfSystem.time.second;
					toggleDisp++;
					if (toggleDisp > DISPLAY_TOGGLE_TIME_SECOND) {
						toggleDisp = 0;
					}
				}
				setDisplayDebugLED(dispTempHum);
			}
	}
	dispSet(arr, arr2);
}

void setConfStepDisp(display_t displayMode, uint8_t val) {
	char arr[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] =  "CONFIGURATION   ";
	char arr2[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "                ";
	setDisplayDebugLED(displayMode);
	// confH, confM, confS, confDisplay
	if (displayMode == confOverview) {
		setConfigChoice(arr2, val);
	}
	else if (displayMode == confDisplay) {
		setConfigDisp(arr2, val);
	}
	else if (displayMode == confSpeed) {
		setConfigSpeed(arr2, val);
	}
	else if (displayMode == confH) {
		setConfigTimeH(arr2, val);
	}
	else if (displayMode == confM) {
		setConfigTimeM(arr2, val);
	}
	else { // confS
		setConfigTimeS(arr2, val);
	}
	dispSet(arr, arr2);
}

void setDTime(char b[], smhTime_t* t) {
	char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:     :  :  ";
	setDText(b, text);
	setDVal(b, t->hour, 8, 2, TRUE);
	setDVal(b, t->minute, 11, 2, TRUE);
	setDVal(b, t->second, 14, 2, TRUE);
}

void setDTemp(char b[], int16_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TEMP:         °C";
	//text[14] = 0xDF; // Versuche das Grad Zeichen korrekt einzusetzen für Display
	setDText(b, text);
	setDVal(b, tmpVal, 9, 4, FALSE);
	
}

void setDHum(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "HUMIDITY:      %";
	setDText(b, text);
	setDVal(b, tmpVal, 12, 3, FALSE);
}

void setConfigChoice(char b[], uint8_t tmpVal) {

	setDText(b, getSelectConfigText(tmpVal));
}

void setConfigDisp(char b[], uint8_t tmpVal) {
	setDText(b, getDispModeText(tmpVal));
}

char* getSelectConfigText(uint8_t tmpVal){
	switch(tmpVal){
		case confChoiceTime:
		return "     OF TIME    ";
		break;
		case confChoiceDisp:
		return " OF DISP. MODE  ";
		break;
		case confChoiceSpeed:
		return " OF DATA SPEED  ";
		break;
		default:
		return "       EXIT     ";
		break;
		
	}
}

char* getDispModeText(uint8_t tmpVal){
	switch(tmpVal){
		case dispTime:
		return "MODE: TIME ONLY ";
		break;
		case dispTimeTemp:
		return "MODE: TIME+TEMP ";
		break;
		case dispTempHum:
		return "MODE: TEMP+HUM  ";
		break;
		default:
		return "MODE: EVERYTHING";
		break;
	}
}

void setConfigTimeH(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:        h  ";
	setDText(b, text);
	setDVal(b, tmpVal, 10, 2, TRUE);
}

void setConfigTimeM(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:        Min";
	setDText(b, text);
	setDVal(b, tmpVal, 10, 2, TRUE);
}

void setConfigSpeed(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "SPEED:          ";
	setDText(b, text);
	setDVal(b, tmpVal, 10, 2, TRUE);
}

void setConfigTimeS(char b[], uint8_t tmpVal) {
	char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:        Sec";
	setDText(b, text);
	setDVal(b, tmpVal, 10, 2, TRUE);
}

void setDText(char b[], char text[]) {
	int i;
	for (i = 0; i < DISPLAY_ARRAY_SIZE_FOR_SOFTWARE; i++) {
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
