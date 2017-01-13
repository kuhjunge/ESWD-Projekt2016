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

display_t getNextConfigChoice(configChoice_t last, uint8_t reverse);

display_t setSpeed(button_t b,uint8_t *speed, display_t lastDisp, uint8_t *next);

display_t setHours(button_t b,smhTime_t *nt, display_t lastDisp, uint8_t *next);

display_t setMinutes(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

display_t setSeconds(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

display_t setDisplayMode(button_t b, display_t lastDisp, uint8_t *next, display_t *newDispl);

configChoice_t setConfChoice(button_t b, uint8_t *next, configChoice_t *newDispl );

void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, uint8_t *goToThermoMode, display_t *newDisplayMode, configChoice_t *confChoice, uint8_t *speed);

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

display_t getNextConfigChoice(configChoice_t last, uint8_t reverse){
	switch(last){// dispTime, dispTimeTemp, dispTempHum, dispTimeTempHum
		case confChoiceDisp:
		return (reverse == TRUE) ? confChoiceExit : confChoiceTime;
		break;
		case confChoiceTime:
		return (reverse == TRUE) ? confChoiceDisp : confChoiceSpeed;
		break;
		case confChoiceSpeed:
		return (reverse == TRUE) ? confChoiceTime : confChoiceExit;
		break;
		default: // confChoiceExit
		return (reverse == TRUE) ? confChoiceSpeed : confChoiceDisp;
		break;
	}
}

display_t setSpeed(button_t b,uint8_t *speed, display_t lastDisp, uint8_t *next){
	*next = FALSE;
	if (b == enter) {
		lastDisp = confOverview;
		*next = TRUE;
	}
	else if (b == up){
		*speed = getNextNumber(*speed ,MAX_SPEED, FALSE);
		} else if (b == down){
		*speed  = getNextNumber(*speed ,MAX_SPEED, TRUE);
	}
	return lastDisp;
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
		lastDisp = confOverview;
	}
	else if (b == up){
		nt->second = getNextNumber(nt->second ,MAX_SECONDS, FALSE);
		} else if (b == down){
		nt->second = getNextNumber(nt->second ,MAX_SECONDS, TRUE);
	}
	return lastDisp;
}

configChoice_t setConfChoice(button_t b, uint8_t *next, configChoice_t *newConfChoice ){
	*next = FALSE;
	if (b == enter) {
		*next = TRUE;
	}
	else if (b == up){
		*newConfChoice = getNextConfigChoice(*newConfChoice, FALSE);
		} else if (b == down){
		*newConfChoice = getNextConfigChoice(*newConfChoice, TRUE);
	}
	return *newConfChoice;
}

display_t setDisplayMode(button_t b, display_t lastDisp, uint8_t *next, display_t *newDispl )
{
	*next = FALSE;
	if (b == enter) {
		lastDisp = confOverview;
		*next = TRUE;
	}
	else if (b == up){
		*newDispl = getNextDispMode(*newDispl, FALSE);
		} else if (b == down){
		*newDispl = getNextDispMode(*newDispl, TRUE);
	}
	return lastDisp;
}

void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, uint8_t *goToThermoMode, display_t *newDisplayMode, configChoice_t *confChoice, uint8_t *speed)
{
	uint8_t next = FALSE;
	switch (*lastDisp)
	{
		case confOverview:
			*lastDisp = confOverview;
			 setConfChoice(b, &next, confChoice);
			 setConfStepDisp(confOverview, *confChoice);
			if (next == TRUE) {
				if(*confChoice == confChoiceDisp){
					setConfStepDisp(confDisplay, systemState.displayMode);
					*lastDisp = confDisplay;
				} else if (*confChoice == confChoiceTime) {
					setConfStepDisp(confH, newTime->hour);
					*lastDisp = confH;
				} else if(*confChoice == confChoiceSpeed) {
					setConfStepDisp(confSpeed, systemState.readIntervall);
					*lastDisp = confSpeed;
				} else {
					*goToThermoMode = TRUE;
				}
			}
		break;

		case confDisplay:
		*lastDisp = setDisplayMode(b, *lastDisp, &next, newDisplayMode);
		setConfStepDisp(confDisplay, *newDisplayMode);
		if (next == TRUE) {
			setConfStepDisp(confOverview, confChoiceDisp);
			systemState.displayMode = *newDisplayMode;
		}
		break;

		case confSpeed:
		 *lastDisp = setSpeed(b, speed, *lastDisp, &next);
		setConfStepDisp(confSpeed, *speed);
		if (next == TRUE) {
			setConfStepDisp(confOverview, confChoiceSpeed);
			systemState.readIntervall = *speed;
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
			setConfStepDisp(confOverview, confChoiceTime);
			systemState.time = *newTime;
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

uint8_t checkTime(uint8_t start, uint8_t now){
	uint8_t comp = start + systemState.readIntervall;
	if (comp > MAX_SECONDS - 1 && now <= systemState.readIntervall){
		comp = comp - MAX_SECONDS;
	}
	return (comp > now) ? FALSE:TRUE;
}

void thermoMode(void) {
	uint8_t goToConfigMode = FALSE;
	while (!goToConfigMode) {
		setSystemTime();
		//if (systemState.time.second != ms.time.second){
		if (checkTime(ms.time.second, systemState.time.second) == TRUE){
			update();
		}
		setDisplay(ms, systemState);
		goToConfigMode = checkForButton(enter);
	}
}

void configMode(void) {
	uint8_t goToThermoMode = FALSE;
	button_t b;
	display_t lastDisp = confOverview;
	smhTime_t newTime = systemState.time;
	display_t newDisplayMode = systemState.displayMode;
	configChoice_t confChoice = confChoiceDisp;
	uint8_t speed = systemState.readIntervall;
	setConfStepDisp(lastDisp, confChoice);
	while (!goToThermoMode) {
		setSystemTime();
		if (isPressed() == TRUE) {
			b = getButton();
			if (b == cancel) {
				goToThermoMode = TRUE; // Abbruch, zurück!
			} else {
				manageConfigStates(&lastDisp, b, &newTime, &goToThermoMode, &newDisplayMode, &confChoice, &speed);

			}
		}
	}
}