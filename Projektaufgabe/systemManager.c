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
/* Gibt die Zahl die auf 'n' folgt zurueck,                             */
/* wird 'max' erreicht, wird 0 zurueck gegeben.                         */
/* Ist 'reverse' auf true, wird die naechstkleinere Zahl                */
/* zurueckgegeben                                                       */
/************************************************************************/
uint8_t getNextNumber(uint8_t n, uint8_t max, uint8_t reverse);

/************************************************************************/
/* Gibt den darauffolgenden Display Mode zurueck                        */
/* (aus dem enum display_t)                                             */
/* Reverse dreht die Reihenfolge um                                     */
/************************************************************************/
display_t getNextDispMode(display_t last, uint8_t reverse);

/************************************************************************/
/* Gibt die darauffolgende configChoice zurueck                         */
/* (aus dem enum configChoice_t)                                        */
/* Reverse dreht die Reihenfolge um                                     */
/************************************************************************/
display_t getNextConfigChoice(configChoice_t last, uint8_t reverse);

/************************************************************************/
/* Setzt die Geschwindigkeit in der Config Menu Anzeige                 */
/* b = der gedrueckte button (up, down, enter)                          */
/* speed = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben;                  */
/*              gibt Speed zurueck                                      */
/*              es sei denn, die Eingabe war Enter, dann wird der neue  */
/*              display_t Status zurueck gegeben (Config Uebersicht)	*/
/* next = sorgt, dafuer, dass der neue Wert gespeichert wird und die	*/
/*	  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird      */
/************************************************************************/
display_t setSpeed(button_t b, uint8_t *speed, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt die Stunden in der Config Menu Anzeige                         */
/* b = der gedrueckte button (up, down, enter)                          */
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt Hours       */
/*	    zurueck es sei denn, die Eingabe war Enter, dann wird der       */
/*	    neue display_t Status zurueck gegeben (Minutes)                 */
/* next = sorgt, dafuer, dass die                                       */
/*	  Anzeige des naechsten Config Bildschirms durchgefuehrt wird       */
/************************************************************************/
display_t setHours(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt die Minuten in der Config Menu Anzeige                         */
/* b = der gedrueckte button (up, down, enter)                          */
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt Minutes     */
/*	zurueck es sei denn, die Eingabe war Enter, dann wird der           */
/*	neue display_t Status zurueck gegeben (Seconds)                     */
/* next = sorgt, dafuer, dass die                                       */
/*          Anzeige des naechsten Config Bildschirmes durchgefuehrt wird*/
/************************************************************************/
display_t setMinutes(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt die Sekunden in der Config Menu Anzeige                        */
/* b = der gedrueckte button (up, down, enter)                          */
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt Seconds     */
/*          zurueck es sei denn, die Eingabe war Enter, dann wird der   */
/*          neue display_t Status zurueck gegeben (Config uebersicht)	*/
/* next = sorgt, dafuer, dass der neue Wert gespeichert wird und die	*/
/*	  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird      */
/************************************************************************/
display_t setSeconds(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next);

/************************************************************************/
/* Setzt den Display Mode in der Config Menu Anzeige                    */
/* b = der gedrueckte button (up, down, enter)                          */
/* nt = der Zeiger auf den letzten Wert - dieser Wert wird angepasst	*/
/* lastDisp = die letzte Anzeige, wird zurueckgegeben; gibt dispMode	*/
/*          zurueck es sei denn, die Eingabe war Enter, dann wird der   */
/*          neue display_t Status zurueck gegeben (Config uebersicht)	*/
/* next = sorgt, dafuer, dass der neue Wert gespeichert wird	und die	*/
/*	  Anzeige des naechsten Config Bildschirmes durchgefuehrt wird      */
/************************************************************************/
display_t setDisplayMode(button_t b, display_t lastDisp, uint8_t *next, display_t *newDispl);

/************************************************************************/
/* Setzt die Auswahl in der Config Menu Anzeige	                        */
/* b = der gedrueckte button (up, down, enter)                          */
/* newConfChoice = der Zeiger auf den letzten Wert                      */
/*				   - dieser Wert wird angepasst                         */
/* next = sorgt dafuer, dass der neue Wert gespeichert wird und die	    */
/*        Anzeige des naechsten Config Bildschirmes durchgefuehrt wird  */
/************************************************************************/
configChoice_t setConfChoice(button_t b, uint8_t *next, configChoice_t *newConfChoice);

/************************************************************************/
/* Waehlt die passende neue Anzeige fuer den Config Mode und gibt die   */
/* Parameter weiter an die Unterfunktionen                              */
/* LastDisp = Anhand dieses Parameters wird das Auswaehlen des Displays */
/*			  durchgefuehrt                                             */
/************************************************************************/
void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime,
        uint8_t *goToThermoMode, display_t *newDisplayMode, configChoice_t *confChoice, uint8_t *speed);

/************************************************************************/
/* Prueft ob Zeit vergangen ist, gibt true zurueck, sollte Zeit         */
/* vergangen sein. Orientiert sich an der im System hinterlegten        */
/* Messgeschwindigkeit                                                  */
/* start = Sekunden fuer den Start des Vergleichs                       */
/* now = Sekunden zum jetztigen Zeitpunkt                               */
/************************************************************************/
uint8_t checkTime(uint8_t start, uint8_t now);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Auswahldialog im Config Menu an                            */
/* lastDisp = anhand dem Ergebnis von getNextConfigChoice() wird der    */
/*			  Displayzustand ausgewaehlt                                */
/* next = wird auf True gesetzt, wenn naechster Modus betreten werden	*/
/*		  soll							                                */
/* confChoice = initialChoice bei der Anzeige                           */
/************************************************************************/
void showOverview(display_t * lastDisp, button_t b, configChoice_t * confChoice, uint8_t * next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Wechselt den Anzeigemodus                                            */
/* lastDisp = hier wird der neue Zustand hineingeschrieben              */
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand        */
/*		  geaendert wird                                                */
/* confChoice = die Auswahl des Nutzers                                 */
/* newTime = wird benoetigt um bei dem Wechsel auf Hour den ersten Wert	*/
/*			 anzuzeigen                                                 */
/* goToThermoMode = Wenn Exit gewaehlt wird, geht er anschliessend in   */
/*					den normalen Anzeigemodus zurueck                   */
/************************************************************************/
void overviewExit(uint8_t next, configChoice_t * confChoice, display_t * lastDisp, smhTime_t * newTime, uint8_t * goToThermoMode);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Display Modus Auswahldialog im Config Menu an              */
/* lastDisp = gibt den neuen Displaymodus zurueck                       */
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand        */
/*		  geaendert wird und die Auswahl wird gespeichert               */
/************************************************************************/
display_t * showConfigDisp(display_t * lastDisp, button_t b, display_t * newDisplayMode, uint8_t * next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Display Modus Auswahldialog im Config Menu an              */
/* newDispplaymode = der neue Displaymodus                              */
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand        */
/*		  geaendert wird                                                */
/************************************************************************/
void configDispExit(uint8_t next, display_t * newDisplayMode);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Speed Auswahldialog im Config Menu an               */
/* lastDisp = gibt den neuen Displaymodus zurueck                       */
/************************************************************************/
display_t * showConfigSpeed(display_t * lastDisp, button_t b, uint8_t * speed, uint8_t *next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Display Modus Auswahldialog im Config Menu an              */
/* speed = die neue Geschwindigkeit	                                    */
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand        */
/*		  geaendert wird                                                */
/************************************************************************/
void configSpeedExit(uint8_t next, uint8_t * speed);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Hours Auswahldialog im Config Menu an               */
/* lastDisp = gibt den neuen Displaymodus zurueck                       */
/************************************************************************/
display_t * showConfigHours(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t *next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Display Modus Auswahldialog im Config Menu an              */
/* newTime = die neue Zeit                                              */
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand        */
/*		  geaendert wird                                                */
/************************************************************************/
void configHoursExit(uint8_t next, smhTime_t * newTime);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Minutes Auswahldialog im Config Menu an             */
/* lastDisp = gibt den neuen Displaymodus zurueck                       */
/************************************************************************/
display_t * showConfigMinutes(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t *next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Display Modus Auswahldialog im Config Menu an              */
/* newTime = die neue Zeit                                              */
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand        */
/*		  geaendert wird                                                */
/************************************************************************/
void configMinutesExit(uint8_t next, smhTime_t * newTime);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Config Seconds Auswahldialog im Config Menu an             */
/* lastDisp = gibt den neuen Displaymodus zurueck                       */
/************************************************************************/
display_t * showConfigSeconds(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t *next);

/************************************************************************/
/* (Ausgelagert aus 'manageConfigStates()' )                            */
/* Zeigt den Display Modus Auswahldialog im Config Menu an              */
/* newTime = die neue Zeit                                              */
/* next = Anhand von Next, wird erkannt, wenn der Displayzustand        */
/*		  geaendert wird                                                */
/************************************************************************/
void configSecondsExit(uint8_t next, smhTime_t * newTime);

/************************************************************************/
/* prueft ob ein Button gedrueckt wurde, wenn ja dann prueft es ob der	*/
/* gedrueckte Button "Cancel" ist und setzt goToThermoMode auf TRUE	    */
/* Wenn es sich bei dem gedrueckten Button nicht um Cancel handelt, dann*/
/* wird eine Funktion zum uendern der Displayanzeige aufgerufen         */
/************************************************************************/
void changeDisplayIfButtonIsPressed(uint8_t *goToThermoMode, display_t lastDisp,
        smhTime_t newTime, display_t newDisplayMode, configChoice_t confChoice, uint8_t speed);

/************************************************************************/
/* Initialisiert alle benoetigten Komponenten des Systems               */
/************************************************************************/
void init(void);

/************************************************************************/
/* Normaler Betriebszustand, in dem das System seine Daten anzeigt      */
/************************************************************************/
void thermoMode(void);

/************************************************************************/
/* Konfigurationsmodus, in dem die Optionen des Systems vom Nutzer      */
/* geaendert werden koennen                                             */
/************************************************************************/
void configMode(void);

/************************************************************************/
/* Ausf�hren einer Erstkonfiguration des Boards                         */
/************************************************************************/
void firstRunMode(void);

// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header										                    */
/************************************************************************/
void startThermo(void) {
    init();
    firstRunMode();
    while (1) {
        systemState.time = getTime();
        thermoMode();
        configMode();
    }
}

// --------------- Implementation der Hilfsfunktionen ---------------

measuringSet_t update(void) {
    ms.temp = getTemp();
    ms.hum = getHum();
    ms.time = getTime();
    return ms;
}

void setSystemTime() {
    systemState.time = getTime();
}

uint8_t getNextNumber(uint8_t n, uint8_t max, uint8_t reverse) {
    if (reverse == TRUE) {
        n--;
        if (n >= max) {
            n = max - 1;
        }
    } else {
        n++;
        if (n >= max) {
            n = 0;
        }
    }
    return n;
}

display_t getNextDispMode(display_t last, uint8_t reverse) {
    switch (last) {// dispTime, dispTimeTemp, dispTempHum, dispTimeTempHum
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

display_t getNextConfigChoice(configChoice_t last, uint8_t reverse) {
    switch (last) {// dispTime, dispTimeTemp, dispTempHum, dispTimeTempHum
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

display_t setSpeed(button_t b, uint8_t *speed, display_t lastDisp, uint8_t *next) {
    *next = FALSE;
    if (b == enter) {
        lastDisp = confOverview;
        *next = TRUE;
    } else if (b == up) {
        *speed = getNextNumber(*speed, MAX_SPEED, FALSE);
    } else if (b == down) {
        *speed = getNextNumber(*speed, MAX_SPEED, TRUE);
    }
    return lastDisp;
}

display_t setHours(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next) {
    *next = FALSE;
    if (b == enter) {
        lastDisp = confM;
        *next = TRUE;
    } else if (b == up) {
        nt->hour = getNextNumber(nt->hour, MAX_HOURS, FALSE);
    } else if (b == down) {
        nt->hour = getNextNumber(nt->hour, MAX_HOURS, TRUE);
    }
    return lastDisp;
}

display_t setMinutes(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next) {
    *next = FALSE;
    if (b == enter) {
        lastDisp = confS;
        *next = TRUE;
    } else if (b == up) {
        nt->minute = getNextNumber(nt->minute, MAX_MINUTES, FALSE);
    } else if (b == down) {
        nt->minute = getNextNumber(nt->minute, MAX_MINUTES, TRUE);
    }
    return lastDisp;
}

display_t setSeconds(button_t b, smhTime_t *nt, display_t lastDisp, uint8_t *next) {
    *next = FALSE;
    if (b == enter) {
        setTime(nt->hour, nt->minute, nt->second);
        *next = TRUE;
        lastDisp = confOverview;
    } else if (b == up) {
        nt->second = getNextNumber(nt->second, MAX_SECONDS, FALSE);
    } else if (b == down) {
        nt->second = getNextNumber(nt->second, MAX_SECONDS, TRUE);
    }
    return lastDisp;
}

configChoice_t setConfChoice(button_t b, uint8_t *next, configChoice_t *newConfChoice) {
    *next = FALSE;
    if (b == enter) {
        *next = TRUE;
    } else if (b == up) {
        *newConfChoice = getNextConfigChoice(*newConfChoice, FALSE);
    } else if (b == down) {
        *newConfChoice = getNextConfigChoice(*newConfChoice, TRUE);
    }
    return *newConfChoice;
}

display_t setDisplayMode(button_t b, display_t lastDisp, uint8_t *next, display_t *newDispl) {
    *next = FALSE;
    if (b == enter) {
        lastDisp = confOverview;
        *next = TRUE;
    } else if (b == up) {
        *newDispl = getNextDispMode(*newDispl, FALSE);
    } else if (b == down) {
        *newDispl = getNextDispMode(*newDispl, TRUE);
    }
    return lastDisp;
}

void overviewExit(uint8_t next, configChoice_t * confChoice, display_t * lastDisp, smhTime_t * newTime, uint8_t * goToThermoMode) {
    if (next == TRUE) {
        if (*confChoice == confChoiceDisp) {
            setConfStepDisp(confDisplay, systemState.displayMode);
            *lastDisp = confDisplay;
        } else if (*confChoice == confChoiceTime) {
            setConfStepDisp(confH, newTime->hour);
            *lastDisp = confH;
        } else if (*confChoice == confChoiceSpeed) {
            setConfStepDisp(confSpeed, systemState.readIntervall);
            *lastDisp = confSpeed;
        } else {
            *goToThermoMode = TRUE;
        }
    }
}

void showOverview(display_t * lastDisp, button_t b, configChoice_t * confChoice, uint8_t * next) {
    *lastDisp = confOverview;
    setConfChoice(b, next, confChoice);
    setConfStepDisp(confOverview, *confChoice);
}

void configDispExit(uint8_t next, display_t * newDisplayMode) {
    if (next == TRUE) {
        setConfStepDisp(confOverview, confChoiceDisp);
        systemState.displayMode = *newDisplayMode;
    }
}

display_t * showConfigDisp(display_t * lastDisp, button_t b, display_t * newDisplayMode, uint8_t * next) {
    *lastDisp = setDisplayMode(b, *lastDisp, next, newDisplayMode);
    setConfStepDisp(confDisplay, *newDisplayMode);
    return lastDisp;
}

void configSpeedExit(uint8_t next, uint8_t * speed) {
    if (next == TRUE) {
        setConfStepDisp(confOverview, confChoiceSpeed);
        systemState.readIntervall = *speed;
    }
}

display_t * showConfigSpeed(display_t * lastDisp, button_t b, uint8_t * speed, uint8_t *next) {
    *lastDisp = setSpeed(b, speed, *lastDisp, next);
    setConfStepDisp(confSpeed, *speed);
    return lastDisp;
}

void configHoursExit(uint8_t next, smhTime_t * newTime) {
    if (next == TRUE) {
        setConfStepDisp(confM, newTime->minute);
    }
}

display_t * showConfigHours(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t *next) {
    *lastDisp = setHours(b, newTime, *lastDisp, next);
    setConfStepDisp(confH, newTime->hour);
    return lastDisp;
}

void configMinutesExit(uint8_t next, smhTime_t * newTime) {
    if (next == TRUE) {
        setConfStepDisp(confS, newTime->second);
    }
}

display_t * showConfigMinutes(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t *next) {
    *lastDisp = setMinutes(b, newTime, *lastDisp, next);
    setConfStepDisp(confM, newTime->minute);
    return lastDisp;
}

void configSecondsExit(uint8_t next, smhTime_t * newTime) {
    if (next == TRUE) {
        setConfStepDisp(confOverview, confChoiceTime);
        systemState.time = *newTime;
    }
}

display_t * showConfigSeconds(display_t * lastDisp, button_t b, smhTime_t * newTime, uint8_t *next) {
    *lastDisp = setSeconds(b, newTime, *lastDisp, next);
    setConfStepDisp(confS, newTime->second);
    return lastDisp;
}

void manageConfigStates(display_t *lastDisp, button_t b, smhTime_t *newTime, 
        uint8_t *goToThermoMode, display_t *newDisplayMode, configChoice_t *confChoice, uint8_t *speed) {
    uint8_t next = FALSE;
    switch (*lastDisp) {
        case confOverview:
            showOverview(lastDisp, b, confChoice, &next);
            overviewExit(next, confChoice, lastDisp, newTime, goToThermoMode);
            break;
        case confDisplay:
            lastDisp = showConfigDisp(lastDisp, b, newDisplayMode, &next);
            configDispExit(next, newDisplayMode);
            break;
        case confSpeed:
            lastDisp = showConfigSpeed(lastDisp, b, speed, &next);
            configSpeedExit(next, speed);
            break;
        case confH:
            lastDisp = showConfigHours(lastDisp, b, newTime, &next);
            configHoursExit(next, newTime);
            break;
        case confM:
            lastDisp = showConfigMinutes(lastDisp, b, newTime, &next);
            configMinutesExit(next, newTime);
            break;
        case confS:
            lastDisp = showConfigSeconds(lastDisp, b, newTime, &next);
            configSecondsExit(next, newTime);
            break;
        default:
            break;
    }
}

void changeDisplayIfButtonIsPressed(uint8_t *goToThermoMode, display_t lastDisp,
        smhTime_t newTime, display_t newDisplayMode, configChoice_t confChoice, uint8_t speed) {
    button_t b;
    if (isPressed() == TRUE) {
        b = getButton();
        if (b == cancel) {
            *goToThermoMode = TRUE; // Abbruch, zurueck!
        } else {
            manageConfigStates(&lastDisp, b, &newTime, goToThermoMode, &newDisplayMode, &confChoice, &speed);
        }
    }
}

uint8_t checkTime(uint8_t start, uint8_t now) {
    uint8_t comp = start + systemState.readIntervall;
    if (comp > MAX_SECONDS - 1 && now <= systemState.readIntervall) {
        comp = comp - MAX_SECONDS;
    }
    return (comp > now) ? FALSE : TRUE;
}

void init(void) {
    initDebugFeatures();
    initTime();
    initClima();
    initDisplay();
    initButton();
    setSystemTime();
    systemState.displayMode = dispTimeTemp;
    systemState.readIntervall = DEFAULT_SPEED;
}

void thermoMode(void) {
    uint8_t goToConfigMode = FALSE;
    button_t b;
    while (!goToConfigMode) {
        setSystemTime();
        //if (systemState.time.second != ms.time.second){
        if (checkTime(ms.time.second, systemState.time.second) == TRUE) {
            update();
        }
        setDisplay(ms, systemState);
        if (isPressed() == TRUE) {
            b = getButton();
            if (b == enter) {
                goToConfigMode = TRUE;
            } else if (b == up) {
                systemState.displayMode = getNextDispMode(systemState.displayMode, FALSE);
            } else if (b == down) {
                systemState.displayMode = getNextDispMode(systemState.displayMode, TRUE);
            }
        }
    }
}

void configMode(void) {
    uint8_t goToThermoMode = FALSE;
    display_t lastDisp = confOverview;
    smhTime_t newTime = systemState.time;
    display_t newDisplayMode = systemState.displayMode;
    configChoice_t confChoice = confChoiceDisp;
    uint8_t speed = systemState.readIntervall;
    setConfStepDisp(lastDisp, confChoice); // Initialanzeige im Config menu
    while (!goToThermoMode) {
        setSystemTime();
        changeDisplayIfButtonIsPressed(&goToThermoMode, lastDisp, newTime, newDisplayMode, confChoice, speed);
    }
}

void firstRunMode(void) {
    volatile uint8_t goToThermoMode = FALSE;
    button_t b;
    display_t lastDisp = confH;
    smhTime_t newTime = systemState.time;
    uint8_t next = FALSE;
    setConfStepDisp(lastDisp, newTime.hour);
    while (!goToThermoMode) {
        setSystemTime();
        if (isPressed() == TRUE) {
            b = getButton();
            if (b == cancel) {
                goToThermoMode = TRUE; // Abbruch, zurueck!
            } else {
                switch (lastDisp) {
                    case confH:
                        lastDisp = *showConfigHours(&lastDisp, b, &newTime, &next);
                        configHoursExit(next, &newTime);
                        break;
                    case confM:
                        lastDisp = *showConfigMinutes(&lastDisp, b, &newTime, &next);
                        configMinutesExit(next, &newTime);
                        break;
                    case confS:
                        lastDisp = *showConfigSeconds(&lastDisp, b, &newTime, &next);
                        if (next == TRUE) {
                            goToThermoMode = TRUE;
                            systemState.time = newTime;
                        }
                        break;
                    default:
                        goToThermoMode = TRUE;
                        break;
                }
            }
        }
    }
}