/*
 * File:   displayControl.c
 * Author: Chris Deter, Alexandra Scheben, Dirk Teschner
 *
 * Unsere Implementation ist sehr stark an die Beispielimplementierung von den folgenden Seiten angelehnt
 * Quellen: 
 * http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
 * http://www.mikrocontroller.net/articles/HD44780
 * http://www.mikrocontroller.net/attachment/288383/lcd-routines.c
 *
 * Created on 27. November 2016, 14:03
 */

#include "displayControl.h"

// ------------------ Definition der Hilfsfunktionen ------------------

void lcd_enable(void);

void lcd_out(uint8_t);

void lcd_command(uint8_t data);

void lcd_clear(void);

void initDispPorts(void);

void initDispLCD(void);

/************************************************************************/
/* Erzeugt einen Enable-Puls											*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void lcd_enable(void) {
    LCD_PORT |= (1 << LCD_ENABLE); // Enable auf 1 setzen
    _delay_us(20); // kurze Pause in microsec
    LCD_PORT &= ~(1 << LCD_ENABLE); // Enable auf 0 setzen
}

/************************************************************************/
/* Sendet 4 Bit an das LCD												*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void lcd_out(uint8_t data) {
    data &= 0xF0; // obere 4 Bit maskieren

    LCD_PORT &= ~(0xF0 >> (4 - LCD_DATABITS)); // Maske löschen
    LCD_PORT |= (data >> (4 - LCD_DATABITS)); // Bits setzen
    lcd_enable();
}

/************************************************************************/
/* Sendet das übergebene Kommando an das LCD							*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void lcd_command(uint8_t data) {
    LCD_PORT &= ~(1 << LCD_REGISTER_SELECT); // RS auf 0 setzen

    lcd_out(data); // zuerst die oberen,
    lcd_out(data << 4); // dann die unteren 4 Bit senden

    _delay_us(LCD_COMMAND_DELAY_US);
}

/************************************************************************/
/* Sendet den Befehl zum löschen des LCDs								*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void lcd_clear(void) {
    lcd_command(LCD_CLEAR_DISPLAY);
    _delay_ms(LCD_CLEAR_DISPLAY_DELAY_MS);
}

/************************************************************************/
/* Ausgabe eines einzelnen Zeichens an der aktuellen Cursorposition		*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void lcd_data(uint8_t data) {
    LCD_PORT |= (1 << LCD_REGISTER_SELECT); // RS auf 1 setzen

    lcd_out(data); // zuerst die oberen,
    lcd_out(data << 4); // dann die unteren 4 Bit senden

    _delay_us(LCD_WRITEDATA_DELAY_US);
}

/************************************************************************/
/* Ausgabe eines Strings an der aktuellen Cursorposition				*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void lcd_string(const char *data) {
    while (*data != '\0') {
        lcd_data(*data++);
    }
}

/************************************************************************/
/* Dokumentation im Funktionsprototyp									*/
/* Author : Chris Deter											        */
/************************************************************************/
void dispSet(char topRow[], char bottomRow[]) {
    int i = 0;
    char first[DISPLAY_ARRAY_SIZE_FOR_CONTROLLER] = "                                        ";
    char second[DISPLAY_ARRAY_SIZE_FOR_CONTROLLER] = "                                        ";
    for (i = 0; i < DISPLAY_ARRAY_SIZE_FOR_SOFTWARE - 1; i++) {
        first[i] = topRow[i];
    }
    for (i = 0; i < DISPLAY_ARRAY_SIZE_FOR_SOFTWARE - 1; i++) {
        second[i] = bottomRow[i];
    }
    //lcd_clear();
    lcd_string(first);
    lcd_string(second);
}


/************************************************************************/
/* Dokumentation im Funktionsprototyp									*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void initDisp(void) {
	// Ports initialisieren
	initDispPorts();

	// LCD initialisieren
	initDispLCD();
}

/************************************************************************/
/* Initialisiert die fuer das Display noetigen Ports					*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void initDispPorts(void) {
	// verwendete Pins auf Ausgang schalten
	uint8_t pins = (0x0F << LCD_DATABITS) | // 4 Datenleitungen
	(1 << LCD_REGISTER_SELECT) | // R/S Leitung
	(1 << LCD_ENABLE); // Enable Leitung
	LCD_DDR |= pins;

	// initial alle Ausgänge auf Null
	LCD_PORT &= ~pins;
}

/************************************************************************/
/* Initialisiert das LCD												*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void initDispLCD(void) {
	// warten auf die Bereitschaft des LCD
	_delay_ms(15); // Bootzeit in ms

	// Soft-Reset muss 3mal hintereinander gesendet werden zur Initialisierung
	lcd_out(0x30); //LCD Soft Reset Pin
	_delay_ms(5);

	lcd_enable();
	_delay_ms(1);

	lcd_enable();
	_delay_ms(1);

	// 4-bit Modus aktivieren
	lcd_out(0x20 | //SET Function
	0x00); //Set Function 4Bit
	_delay_ms(5); //4 Bit Mode Milliseconds wait

	// 4-bit Modus / 2 Zeilen / 5x7
	lcd_command(LCD_SET_FUNCTION |
	LCD_FUNCTION_4BIT |
	LCD_FUNCTION_2LINE |
	LCD_FUNCTION_5X7);

	// Display ein / Cursor aus / Blinken aus
	lcd_command(LCD_SET_DISPLAY |
	LCD_DISPLAY_ON |
	LCD_CURSOR_OFF |
	LCD_BLINKING_OFF);

	// Cursor inkrement / kein Scrollen
	lcd_command(LCD_SET_ENTRY |
	LCD_ENTRY_INCREASE |
	LCD_ENTRY_NOSHIFT);

	lcd_clear();
}