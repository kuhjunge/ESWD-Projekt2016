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

void setSystemTime(){
	systemState.time = getTime();
}

int checkForButton(button_t target){
	button_t b;
	if (isPressed() == TRUE) {
		b = getButton();
		if (b == target) {
			return TRUE;
		}
	}
	return FALSE;
}

void thermoMode(void) {
	uint8_t goToConfigMode = FALSE;
	while (!goToConfigMode) {
		setSystemTime();
		if (systemState.time.second != ms.time.second){
			setDisplay(ms, systemState.displayMode);
			update();
		}
		goToConfigMode = checkForButton(enter);
	}
}

uint8_t getNextNumber(uint8_t n, uint8_t max,  uint8_t reverse){
	if (reverse == TRUE) {
		n--;
		if (n >= max){
			n = max - 1;
		}
		} else {
		n++;
		if (n >= max){
			n=0;
		}
	}
	return n;
}

display_t getNextDispMode(display_t last, uint8_t reverse){
	switch(last){// dispTime, dispTimeTemp, dispTempHum, dispTimeTempHum
		case dispTime:
		return (reverse == TRUE) ? dispTimeTempHum : dispTimeTemp;
		break;
		case dispTimeTemp:
		return (reverse == TRUE) ? dispTime : dispTempHum;
		break;
		case dispTempHum:
		return (reverse == TRUE) ? dispTimeTemp : dispTimeTempHum;
		break;
		case dispTimeTempHum:
		return (reverse == TRUE) ? dispTempHum : dispTime;
		break;
		default:
		return dispTime;
		break;
	}
}

display_t setHours(button_t b,smhTime_t *nt, display_t lastDisp)
{
	if (b == enter) {
		lastDisp = confM;
	}
	else if (b == up){
		nt->hour = getNextNumber(nt->hour ,12, FALSE);
		} else if (b == down){
		nt->hour  = getNextNumber(nt->hour ,12, TRUE);
	}
	return lastDisp;
}

display_t setMinutes(button_t b, smhTime_t *nt, display_t lastDisp)
{
	if (b == enter) {
		lastDisp = confS;
	}
	else if (b == up){
		nt->minute = getNextNumber(nt->minute ,60, FALSE);
		} else if (b == down){
		nt->minute = getNextNumber(nt->minute ,60, TRUE);
	}
	return lastDisp;
}

display_t setSeconds(button_t b, smhTime_t *nt, display_t lastDisp)
{
	if (b == enter) {
		setTime(nt->hour, nt->minute, nt->second);
		lastDisp = dispTime;
	}
	else if (b == up){
		nt->second = getNextNumber(nt->second ,60, FALSE);
		} else if (b == down){
		nt->second = getNextNumber(nt->second ,60, TRUE);
	}
	return lastDisp;
}

display_t setDisplayMode(button_t b, display_t lastDisp)
{
	if (b == enter) {
		lastDisp = confH;
	}
	else if (b == up){
		systemState.displayMode = getNextDispMode(systemState.displayMode, FALSE);
		} else if (b == down){
		systemState.displayMode = getNextDispMode(systemState.displayMode, TRUE);
	}
	return lastDisp;
}

void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, uint8_t *goToThermoMode)
{
	switch (*lastDisp)
	{
		case confDisplay:
		*lastDisp = setDisplayMode(b, *lastDisp);
		setConfStepDisp(confDisplay, systemState.displayMode);
		PORTB = 0x10;
		break;

		case confH:
		*lastDisp = setHours(b, newTime, *lastDisp);
		setConfStepDisp(confH, newTime->hour);
		PORTB = 0x20;
		break;

		case confM:
		*lastDisp = setMinutes(b, newTime, *lastDisp);
		setConfStepDisp(confM, newTime->minute);
		PORTB = 0x30;
		break;

		case confS:
		*lastDisp = setSeconds(b, newTime, *lastDisp);
		setConfStepDisp(confS, newTime->second);
		PORTB = 0x40;
		break;
		
		default:
		*goToThermoMode = TRUE;
		break;
	}
}

void configMode(void) {
	uint8_t goToThermoMode = FALSE;
	button_t b;
	display_t lastDisp = confDisplay;
	smhTime_t newTime = systemState.time;
	setDisplay(ms, lastDisp);
	PORTB = 0xF0;
	while (!goToThermoMode) {
		setSystemTime();
		if (isPressed() == TRUE) {
			b = getButton();
			if (b == cancel) {
				goToThermoMode = TRUE; // Abbruch, zurück!
				} else {
				manageConfigStates(&lastDisp, b, &newTime, &goToThermoMode);

			}
		}
	}
}

void startThermo(void) {
	init();
	systemState.displayMode = dispTimeTempHum;
	systemState.readIntervall = 1;
	while (1) {
		systemState.time = getTime();
		thermoMode();
		configMode();
	}
}