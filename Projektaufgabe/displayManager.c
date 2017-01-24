/*
 * File:   displayManager.c
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 27. November 2016, 14:03
 */

#include "displayManager.h"

int toggleDisp = 0; // lokale Variable zur Steuerung des Toggle Mode der Anzeige
uint8_t seconds = 0; // lokale Variable zur Steuerung des Toggle Mode der Anzeige

// ------------------ Definition der Hilfsfunktionen ------------------

char* itoa(int i, char b[]);

char* toDRight(char b[], uint8_t fill);

void setDText(char b[], char text[]);

char* setDVal(char b[], int16_t tmpVal, uint8_t offset, uint8_t size, uint8_t withZero);

void setDTemp(char b[], int16_t tmpVal);

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

void doToggleDisp(char * arr, state_t *stateOfSystem, measuringSet_t *ms, char * arr2);

// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Initialisiert die Display Komponenten                                */
/************************************************************************/
void initDisplay(void) {
    initDisp();
}

/************************************************************************/
/* Setzt die Anzeige im normalen Anzeige Modus                          */
/************************************************************************/
void setDisplay(measuringSet_t ms, state_t stateOfSystem) {
    char arr[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "                ";
    char arr2[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "                ";
    setDisplayDebugLED(stateOfSystem.displayMode);
    if (stateOfSystem.displayMode == dispTime) {
        setDTime(arr, &stateOfSystem.time);
    } else if (stateOfSystem.displayMode == dispTimeTemp) {
        setDTime(arr, &stateOfSystem.time);
        setDTemp(arr2, ms.temp);
    } else if (stateOfSystem.displayMode == dispTempHum) {
        setDTemp(arr, ms.temp);
        setDHum(arr2, ms.hum);
    } else {
        doToggleDisp(arr, &stateOfSystem, &ms, arr2);
    }
    dispSet(arr, arr2);
}

/************************************************************************/
/* Setzt die Anzeige im Konfigurationsmodus                             */
/************************************************************************/
void setConfStepDisp(display_t displayMode, uint8_t val) {
    char arr[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "CONFIGURATION   ";
    char arr2[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "                ";
    setDisplayDebugLED(displayMode);
    // confH, confM, confS, confDisplay
    if (displayMode == confOverview) {
        setConfigChoice(arr2, val);
    } else if (displayMode == confDisplay) {
        setConfigDisp(arr2, val);
    } else if (displayMode == confSpeed) {
        setConfigSpeed(arr2, val);
    } else if (displayMode == confH) {
        setConfigTimeH(arr2, val);
    } else if (displayMode == confM) {
        setConfigTimeM(arr2, val);
    } else { // confS
        setConfigTimeS(arr2, val);
    }
    dispSet(arr, arr2);
}

// --------------- Implementation der Hilfsfunktionen ---------------


/************************************************************************/
/* (ausgelagert aus : setDisplay)                                        */
/* managt das togglen der Anzeige im "Everything" Anzeige Modus         */
/************************************************************************/
void doToggleDisp(char * arr, state_t *stateOfSystem, measuringSet_t *ms, char * arr2) {
    if (toggleDisp <= DISPLAY_TOGGLE_TIME_FIRST) {
        setDTime(arr, &stateOfSystem->time);
        if (stateOfSystem->time.second != seconds) {
            seconds = stateOfSystem->time.second;
            toggleDisp++;
        }
        setDisplayDebugLED(dispTime);
    } else {
        setDTemp(arr, ms->temp);
        setDHum(arr2, ms->hum);
        if (stateOfSystem->time.second != seconds) {
            seconds = stateOfSystem->time.second;
            toggleDisp++;
            if (toggleDisp > DISPLAY_TOGGLE_TIME_SECOND) {
                toggleDisp = 0;
            }
        }
        setDisplayDebugLED(dispTempHum);
    }
}


/************************************************************************/
/* Erzeugt die Ausgabe fuer die Zeit Anzeige Zeile						*/
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der      */
/* zweite Parameter ist der Wert, der als Zeit angezeigt werden	 soll   */
/************************************************************************/
void setDTime(char b[], smhTime_t* t) {
    char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:     :  :  ";
    setDText(b, text);
    setDVal(b, t->hour, 8, 2, TRUE);
    setDVal(b, t->minute, 11, 2, TRUE);
    setDVal(b, t->second, 14, 2, TRUE);
}

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Temperatur Anzeige Zeile                */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der		*/
/* zweite Parameter ist der Wert, der als Temperatur angezeigt werden	*/
/* soll																	*/
/************************************************************************/
void setDTemp(char b[], int16_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TEMP:         �C";
    //text[14] = 0xDF; // Versuche das Grad Zeichen korrekt einzusetzen fuer Display
    setDText(b, text);
    setDVal(b, tmpVal, 9, 4, FALSE);
}

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Luftfeuchtigkeit Anzeige Zeile          */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der		*/
/* zweite Parameter ist der Wert, der als Feuchtigkeit angezeigt werden	*/
/* soll									*/
/************************************************************************/
void setDHum(char b[], uint8_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "HUMIDITY:      %";
    setDText(b, text);
    setDVal(b, tmpVal, 12, 3, FALSE);
}

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Anzeige des Auswahlmenus der Optionen   */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der      */
/* zweite Parameter ist der Wert, der angezeigt werden soll		*/
/************************************************************************/
void setConfigChoice(char b[], uint8_t tmpVal) {
    setDText(b, getSelectConfigText(tmpVal));
}

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Konfiguration des Anzeigemodus          */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der      */
/* zweite Parameter ist der Wert, der angezeigt werden soll				*/
/************************************************************************/
void setConfigDisp(char b[], uint8_t tmpVal) {
    setDText(b, getDispModeText(tmpVal));
}

/************************************************************************/
/* Wandelt einen Wert in eine Textausgabe fuer das Auswahlmenu  um      */
/************************************************************************/
char* getSelectConfigText(uint8_t tmpVal) {
    switch (tmpVal) {
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

/************************************************************************/
/* Wandelt einen Wert in eine Textausgabe fuer den Displaymodus um      */
/************************************************************************/
char* getDispModeText(uint8_t tmpVal) {
    switch (tmpVal) {
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

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Configuration der Stunden               */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der		*/
/* zweite Parameter ist der Wert, der angezeigt werden soll				*/
/************************************************************************/
void setConfigTimeH(char b[], uint8_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:        h  ";
    setDText(b, text);
    setDVal(b, tmpVal, 10, 2, TRUE);
}

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Configuration der Minuten               */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der		*/
/* zweite Parameter ist der Wert, der angezeigt werden soll				*/
/************************************************************************/
void setConfigTimeM(char b[], uint8_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:        Min";
    setDText(b, text);
    setDVal(b, tmpVal, 10, 2, TRUE);
}

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Konfiguration des Abfrageintervalls     */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der      */
/* zweite Parameter ist der Wert, der angezeigt werden soll				*/
/************************************************************************/
void setConfigSpeed(char b[], uint8_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "SPEED:          ";
    setDText(b, text);
    setDVal(b, tmpVal, 10, 2, TRUE);
}

/************************************************************************/
/* Erzeugt die Ausgabe fuer die Configuration der Sekunden              */
/* Der erste Parameter ist die Anzeige Zeile des Displays, und der		*/
/* zweite Parameter ist der Wert, der angezeigt werden soll				*/
/************************************************************************/
void setConfigTimeS(char b[], uint8_t tmpVal) {
    char text[DISPLAY_ARRAY_SIZE_FOR_SOFTWARE] = "TIME:        Sec";
    setDText(b, text);
    setDVal(b, tmpVal, 10, 2, TRUE);
}


/************************************************************************/
/* Kopiert die Chars aus 'text' in 'b'									*/
/************************************************************************/
void setDText(char b[], char text[]) {
    int i;
    for (i = 0; i < DISPLAY_ARRAY_SIZE_FOR_SOFTWARE; i++) {
        b[i] = text[i];
    }
}

/************************************************************************/
/* Setzt einen Integer Wert in einem Char Array                         */
/* b[] = Char Array in dem der Wert eingefuegt werden soll				*/
/* tmpVal =  Der Wert der ins Array eingefuegt werden soll				*/
/* offset = Position an dem der Wert eingefuegt werden soll				*/
/* size = Groesse der einzufuegenen Zahl								*/
/* withZero = boolischer Wert, welcher angibt ob der leere Bereich mit  */
/* Null aufgefuellt werden soll											*/
/************************************************************************/
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
        if (temp[2] == ' ') {
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

/************************************************************************/
/* Funktion zum Verschieben eines Chars nach rechts                     */
/* Sorgt dafuer, dass die Zahlen korrekt rechtsbuendig ins Display      */
/* geschrieben werden.													*/
/* Der erste Parameter ist das zu fuellende Array und der zweite		*/
/* Parameter gibt die Position im Array an.								*/
/************************************************************************/
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

/************************************************************************/
/* Wandelt eine Integer Zahl in eine Char Represenation der Zahl um     */
/* der Erste Parameter ist die Eingabe Zahl								*/
/* der Zweite Parameter ist das Ausgabe Array, welches von der Funktion	*/
/* auch zurueck gegeben wird.											*/
/************************************************************************/
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
    do {
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}
