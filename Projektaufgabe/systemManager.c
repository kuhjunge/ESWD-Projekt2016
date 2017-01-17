/*
* File:   main.c
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 23. November 2016, 18:07
*/
#include "systemManager.h"

state_t systemState;
measuringSet_t ms;

// ------------------ Definition der Hilfsunktionen ------------------

/************************************************************************/
/* Gibt ein neues Set von Messwerten mit Zeitstempel zurueck            */
/************************************************************************/
measuringSet_t update(void);

/************************************************************************/
/* Aktualisiert die Zeit im System                                      */
/************************************************************************/
void setSystemTime();

/************************************************************************/
/* Prueft, ob ein bestimmter Knopf gedrueckt wurde						*/
/* Nur genutzt fuer das Enter, welches den Config Modus aufruft			*/
/************************************************************************/
int checkForButton(button_t target);

/************************************************************************/
/* Gibt die Zahl die auf 'n' folgt zurueck,								*/
/* wird 'max' erreicht, wird 0 zurueck gegeben. 						*/
/* Ist 'reverse' auf true, wird die naechstkleinere Zahl				*/
/* zurueckgegeben														*/
/************************************************************************/
uint8_t getNextNumber(uint8_t n, uint8_t max,  uint8_t reverse);

/************************************************************************/
/* Gibt den darauffolgenden Display Mode zurueck						*/
/* (aus dem enum display_t)												*/
/* Reverse dreht die Reihenfolge um										*/
/************************************************************************/
display_t getNextDispMode(display_t last, uint8_t reverse);

/************************************************************************/
/* Gibt die darauffolgende configChoice zurueck							*/
/* (aus dem enum configChoice_t)										*/
/* Reverse dreht die Reihenfolge um  									*/
/************************************************************************/
display_t getNextConfigChoice(configChoice_t last, uint8_t reverse);

/************************************************************************/
/* Setzt die Geschwindigkeit in der Config Menu Anzeige					*/
/* b = der gedrueckte button (up, down, enter)						  	*/
/* speed = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben;					*/
/*			  gibt Speed zurueck										*/
/*			  es sei denn, die Eingabe war Enter, dann wird der neue    */
/*			  display_t Status zurueck gegeben (Config Uebersicht)		*/
/* next = sorgt, dafuer, dass der neue Wert gespeichert wird und die	*/
/*		  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird  */
/************************************************************************/
display_t setSpeed(button_t b,uint8_t *speed, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt die Stunden in der Config Menu Anzeige							*/
/* b = der gedrueckte button (up, down, enter)						  	*/
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt Hours		*/
/*			  zurueck es sei denn, die Eingabe war Enter, dann wird der */
/*			  neue display_t Status zurueck gegeben (Minutes)			*/
/* next = sorgt, dafuer, dass die										*/
/*		  Anzeige des naechsten Config Bildschirms durchgefuehrt wird   */
/************************************************************************/
display_t setHours(button_t b,smhTime_t *nt, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt die Minuten in der Config Menu Anzeige							*/
/* b = der gedrueckte button (up, down, enter)						  	*/
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt Minutes		*/
/*			  zurueck es sei denn, die Eingabe war Enter, dann wird der */
/*			  neue display_t Status zurueck gegeben (Seconds)			*/
/* next = sorgt, dafuer, dass die										*/
/*		  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird  */
/************************************************************************/
display_t setMinutes(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt die Sekunden in der Config Menu Anzeige						*/
/* b = der gedrueckte button (up, down, enter)						  	*/
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt Seconds		*/
/*			  zurueck es sei denn, die Eingabe war Enter, dann wird der */
/*			  neue display_t Status zurueck gegeben (Config uebersicht)	*/
/* next = sorgt, dafuer, dass der neue Wert gespeichert wird und die	*/
/*		  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird  */
/************************************************************************/
display_t setSeconds(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt den Display Mode in der Config Menu Anzeige					*/
/* b = der gedrueckte button (up, down, enter)						  	*/
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt dispMode	*/
/*			  zurueck es sei denn, die Eingabe war Enter, dann wird der */
/*			  neue display_t Status zurueck gegeben (Config uebersicht)	*/
/* next = sorgt, dafuer, dass der neue Wert gespeichert wird	und die	*/
/*		  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird  */
/************************************************************************/
display_t setDisplayMode(button_t b, display_t lastDisp, uint8_t *next, display_t *newDispl);

/************************************************************************/
/* Setzt die Auswahl in der Config Menu Anzeige							*/
/* b = der gedrueckte button (up, down, enter)						  	*/
/* newConfChoice = der Zeiger auf den letzten Wert						*/
/*				   - dieser Wert wird angepasst							*/
/* next = sorgt dafuer, dass der neue Wert gespeichert wird und die		*/
/*		  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird  */
/************************************************************************/
configChoice_t setConfChoice(button_t b, uint8_t *next, configChoice_t *newConfChoice );

/************************************************************************/
/* Waehlt die passende neue Anzeige fuer den Config Mode und gibt die   */
/* Parameter weiter an die Unterfunktionen								*/
/* LastDisp = Anhand dieses Parameters wird das Auswaehlen des Displays */
/*			  durchgefuehrt												*/
/************************************************************************/
void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, uint8_t *goToThermoMode, display_t *newDisplayMode, configChoice_t *confChoice, uint8_t *speed);

/************************************************************************/
/* Prueft ob Zeit vergangen ist, gibt true zurueck, sollte Zeit			*/
/* vergangen sein. Orientiert sich an der im System hinterlegten		*/
/* Messgeschwindigkeit 													*/
/* start = Sekunden fuer den Start des Vergleichs						*/
/* now = Sekunden zum jetztigen Zeitpunkt								*/
/************************************************************************/
uint8_t checkTime(uint8_t start, uint8_t now);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Auswahldialog im Config Menu an							*/
/* lastDisp = anhand dem Ergebnis von getNextConfigChoice() wird der    */
/*			  Displayzustand ausgewaehlt								*/
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand		*/
/*		  geaendert wird												*/
/* confChoice = initialChoice bei der Anzeige							*/
/* newTime = wird benoetigt um bei dem Wechsel auf Hour den ersten Wert	*/
/*			 anzuzeigen													*/
/* goToThermoMode = Wenn Exit gewaehlt wird, geht er anschließend in den */
/*					normalen Anzeigemodus zurueck						*/
/************************************************************************/
void showOverview(display_t * lastDisp, button_t b, uint8_t next, configChoice_t * confChoice, smhTime_t * newTime, uint8_t * goToThermoMode);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Display Modus Auswahldialog im Config Menu an				*/
/* lastDisp = gibt den neuen Displaymodus zurueck						*/
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand		*/
/*		  geaendert wird und die Auswahl wird gespeichert				*/
/*                                                                      */
/************************************************************************/
display_t * showConfigDisp(display_t * lastDisp, button_t b, uint8_t next, display_t * newDisplayMode);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Speed Auswahldialog im Config Menu an				*/
/* lastDisp = gibt den neuen Displaymodus zurueck						*/
/************************************************************************/
display_t * showConfigSpeed(display_t * lastDisp, button_t b, uint8_t * speed, uint8_t next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Hours Auswahldialog im Config Menu an				*/
/* lastDisp = gibt den neuen Displaymodus zurueck						*/
/************************************************************************/
display_t * showConfigHours(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Minutes Auswahldialog im Config Menu an				*/
/* lastDisp = gibt den neuen Displaymodus zurueck						*/
/************************************************************************/
display_t * showConfigMinutes(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Seconds Auswahldialog im Config Menu an				*/
/* lastDisp = gibt den neuen Displaymodus zurueck						*/
/************************************************************************/
display_t * showConfigSeconds(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t next);

/************************************************************************/
/* Initialisiert alle benoetigten Komponenten des Systems				*/
/************************************************************************/
void init(void);

/************************************************************************/
/* Normaler Betriebszustand, in dem das System seine Daten anzeigt      */
/************************************************************************/
void thermoMode(void);

/************************************************************************/
/* Konfigurationsmodus, in dem die Optionen des Systems vom Nutzer		*/
/* geaendert werden koennen												*/
/************************************************************************/
void configMode(void);

// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header										                    */
/************************************************************************/
void startThermo(void) {
	init();
	systemState.displayMode = dispTimeTemp;
	systemState.readIntervall = 1;
	while (1) {
		systemState.time = getTime();
		configMode();
		thermoMode();
	}
}

// --------------- Implementation der Hilfsfunktionen ---------------

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

void showOverview(display_t * lastDisp, button_t b, uint8_t next, configChoice_t * confChoice, smhTime_t * newTime, uint8_t * goToThermoMode)
{
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
}

display_t * showConfigDisp(display_t * lastDisp, button_t b, uint8_t next, display_t * newDisplayMode)
{
	*lastDisp = setDisplayMode(b, *lastDisp, &next, newDisplayMode);
	setConfStepDisp(confDisplay, *newDisplayMode);
	if (next == TRUE) {
		setConfStepDisp(confOverview, confChoiceDisp);
		systemState.displayMode = *newDisplayMode;
	}		
	return lastDisp;
}

display_t * showConfigSpeed(display_t * lastDisp, button_t b, uint8_t * speed, uint8_t next)
{
	 *lastDisp = setSpeed(b, speed, *lastDisp, &next);
	setConfStepDisp(confSpeed, *speed);
	if (next == TRUE) {
		setConfStepDisp(confOverview, confChoiceSpeed);
		systemState.readIntervall = *speed;
	}		
	return lastDisp;
}

display_t * showConfigHours(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t next)
{
	*lastDisp = setHours(b, newTime, *lastDisp, &next);
	setConfStepDisp(confH, newTime->hour);
	if (next == TRUE) {
		setConfStepDisp(confM, newTime->minute);
	}		
	return lastDisp;
}

display_t * showConfigMinutes(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t next)
{
	*lastDisp = setMinutes(b, newTime, *lastDisp, &next);
	setConfStepDisp(confM, newTime->minute);
	if (next == TRUE) {
		setConfStepDisp(confS, newTime->second);
	}		
	return lastDisp;
}

display_t * showConfigSeconds(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t next)
{
	*lastDisp = setSeconds(b, newTime, *lastDisp, &next);
	setConfStepDisp(confS, newTime->second);
	if (next == TRUE) {
		setConfStepDisp(confOverview, confChoiceTime);
		systemState.time = *newTime;
	}		
	return lastDisp;
}

void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, uint8_t *goToThermoMode, display_t *newDisplayMode, configChoice_t *confChoice, uint8_t *speed)
{
	uint8_t next = FALSE;
	switch (*lastDisp)
	{
		case confOverview:
			showOverview(lastDisp, b, next, confChoice, newTime, goToThermoMode);
		break;
		case confDisplay:
			lastDisp = showConfigDisp(lastDisp, b, next, newDisplayMode);
		break;
		case confSpeed:
			lastDisp = showConfigSpeed(lastDisp, b, speed, next);
		break;
		case confH:
			lastDisp = showConfigHours(lastDisp, b, newTime, next);
		break;
		case confM:
			lastDisp = showConfigMinutes(lastDisp, b, newTime, next);
		break;
		case confS:
			lastDisp = showConfigSeconds(lastDisp, b, newTime, next);
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
	display_t lastDisp = confH;
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
				goToThermoMode = TRUE; // Abbruch, zurueck!
			} else {
				manageConfigStates(&lastDisp, b, &newTime, &goToThermoMode, &newDisplayMode, &confChoice, &speed);

			}
		}
	}
}