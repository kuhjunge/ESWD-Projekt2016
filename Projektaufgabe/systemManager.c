/*
* File:   main.c
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 23. November 2016, 18:07
*/
#include "systemManager.h"

state_t systemState;
measuringSet_t ms;

measuringSet_t update(void);

void setSystemTime();

int checkForButton(button_t target);

uint8_t getNextNumber(uint8_t n, uint8_t max,  uint8_t reverse);

display_t getNextDispMode(display_t last, uint8_t reverse);

display_t setHours(button_t b,smhTime_t *nt, display_t lastDisp, uint8_t *next);

display_t setMinutes(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

display_t setSeconds(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

display_t setDisplayMode(button_t b, display_t lastDisp, uint8_t *next, display_t *newDispl);

void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, uint8_t *goToThermoMode, display_t *newDisplayMode);

uint8_t isTimeChanged(smhTime_t a, smhTime_t b, uint8_t dif);

void init(void);

void thermoMode(void);

void configMode(void);

void startThermo(void) {
	init();
	systemState.displayMode = dispTimeTemp;
	systemState.readIntervall = 1;
	while (1) {
		systemState.time = getTime();
		thermoMode();
		configMode();
	}
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

display_t setHours(button_t b,smhTime_t *nt, display_t lastDisp, uint8_t *next)
{
	*next = FALSE;
	if (b == enter) {
		lastDisp = confM;
		*next = TRUE;
	}
	else if (b == up){
		nt->hour = getNextNumber(nt->hour ,MAX_HOURS, FALSE);
		} else if (b == down){
		nt->hour  = getNextNumber(nt->hour ,MAX_HOURS, TRUE);
	}
	return lastDisp;
}

display_t setMinutes(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next)
{
	*next = FALSE;
	if (b == enter) {
		lastDisp = confS;
		*next = TRUE;
	}
	else if (b == up){
		nt->minute = getNextNumber(nt->minute, MAX_MINUTES, FALSE);
		} else if (b == down){
		nt->minute = getNextNumber(nt->minute, MAX_MINUTES, TRUE);
	}
	return lastDisp;
}

display_t setSeconds(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next)
{
	*next = FALSE;
	if (b == enter) {
		setTime(nt->hour, nt->minute, nt->second);
		*next = TRUE;
	}
	else if (b == up){
		nt->second = getNextNumber(nt->second ,MAX_SECONDS, FALSE);
		} else if (b == down){
		nt->second = getNextNumber(nt->second ,MAX_SECONDS, TRUE);
	}
	return lastDisp;
}

display_t setDisplayMode(button_t b, display_t lastDisp, uint8_t *next, display_t *newDispl )
{
	*next = FALSE;
	if (b == enter) {
		lastDisp = confH;
		*next = TRUE;
	}
	else if (b == up){
		*newDispl = getNextDispMode(*newDispl, FALSE);
		} else if (b == down){
		*newDispl = getNextDispMode(*newDispl, TRUE);
	}
	return lastDisp;
}

void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, uint8_t *goToThermoMode, display_t *newDisplayMode)
{
	uint8_t next = FALSE;
	switch (*lastDisp)
	{
		case confDisplay:
		*lastDisp = setDisplayMode(b, *lastDisp, &next, newDisplayMode);
		setConfStepDisp(confDisplay, *newDisplayMode);
		if (next == TRUE) {
			setConfStepDisp(confH, newTime->hour);
			systemState.displayMode = *newDisplayMode;
		}
		break;

		case confH:
		*lastDisp = setHours(b, newTime, *lastDisp, &next);
		setConfStepDisp(confH, newTime->hour);
		if (next == TRUE) {
			setConfStepDisp(confM, newTime->minute);
		}
		break;

		case confM:
		*lastDisp = setMinutes(b, newTime, *lastDisp, &next);
		setConfStepDisp(confM, newTime->minute);
		if (next == TRUE) {
			setConfStepDisp(confS, newTime->second);
		}
		break;

		case confS:
		*lastDisp = setSeconds(b, newTime, *lastDisp, &next);
		setConfStepDisp(confS, newTime->second);
		if (next == TRUE) {
			*goToThermoMode = TRUE;
		}
		break;

		default:
		break;
	}
}

void init(void) {
	initDebugFeatures();
	initTime();
	initClima();
	initDisplay();
	initButton();
}

// NICHT BENUTZT BIS JETZT / EVENTUELL FEHLERHAFT
uint8_t isTimeChanged(smhTime_t a, smhTime_t b, uint8_t dif){
	uint16_t compa = a.second + (a.minute * 60) + (a.hour * 60 * 60);
	uint16_t compb = a.second + (a.minute * 60) + (a.hour * 60 * 60);
	if (compb - compa > dif){
		return TRUE;
	}
	return FALSE;
}

void thermoMode(void) {
	uint8_t goToConfigMode = FALSE;
	while (!goToConfigMode) {
		setSystemTime();
		if (systemState.time.second != ms.time.second){
			//if (isTimeChanged(ms.time, systemState.time, 5) == TRUE){
			setDisplay(ms, systemState.displayMode);
			update();
		}
		goToConfigMode = checkForButton(enter);
	}
}

void configMode(void) {
	uint8_t goToThermoMode = FALSE;
	button_t b;
	display_t lastDisp = confDisplay;
	smhTime_t newTime = systemState.time;
	display_t newDisplayMode = systemState.displayMode;
	setConfStepDisp(confDisplay, systemState.displayMode);
	while (!goToThermoMode) {
		setSystemTime();
		if (isPressed() == TRUE) {
			b = getButton();
			if (b == cancel) {
				goToThermoMode = TRUE; // Abbruch, zurück!
				} else {
				manageConfigStates(&lastDisp, b, &newTime, &goToThermoMode, &newDisplayMode);

			}
		}
	}
}