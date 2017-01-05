/*
* File:   displayControl.c
* Author: Alexandra Scheben, Dirk Teschner
*
* Created on 27. November 2016, 14:03
*/

#include "displayControl.h"

#define LCD_PORT      PORTD
#define LCD_DDR       DDRD
#define LCD_DB        PD4

//  LCD RS      <-->  PORTD Bit PD4     (RS: 1=Data, 0=Command)
#define LCD_RS        PD2

//  LCD EN      <-->  PORTD Bit PD5     (EN: 1-Impuls für Daten)
#define LCD_EN        PD3

#define LCD_COMMAND_US          42
#define LCD_CLEAR_DISPLAY       0x01
#define LCD_CLEAR_DISPLAY_MS    2
#define LCD_WRITEDATA_US        46

#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01
#define LCD_SET_SHIFT           0x10

#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01

#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04
#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00
#define LCD_FUNCTION_5X10       0x04


#define LCD_DDADR_LINE1         0x00
#define LCD_DDADR_LINE2         0x40

static void lcd_enable(void) {
	LCD_PORT |= (1 << LCD_EN); // Enable auf 1 setzen
	_delay_us(20); // kurze Pause in microsec
	LCD_PORT &= ~(1 << LCD_EN); // Enable auf 0 setzen
}

static void lcd_out(uint8_t data) {
	data &= 0xF0; // obere 4 Bit maskieren

	LCD_PORT &= ~(0xF0 >> (4 - LCD_DB)); // Maske löschen
	LCD_PORT |= (data >> (4 - LCD_DB)); // Bits setzen
	lcd_enable();
}

void lcd_command(uint8_t data) {
	LCD_PORT &= ~(1 << LCD_RS); // RS auf 0 setzen

	lcd_out(data); // zuerst die oberen,
	lcd_out(data << 4); // dann die unteren 4 Bit senden

	_delay_us(LCD_COMMAND_US);
}

void lcd_clear(void) {
	lcd_command(LCD_CLEAR_DISPLAY);
	_delay_ms(LCD_CLEAR_DISPLAY_MS);
}

void initDisp(void) {
	// verwendete Pins auf Ausgang schalten
	uint8_t pins = (0x0F << LCD_DB) | // 4 Datenleitungen
	(1 << LCD_RS) | // R/S Leitung
	(1 << LCD_EN); // Enable Leitung
	LCD_DDR |= pins;

	// initial alle Ausgänge auf Null
	LCD_PORT &= ~pins;

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

void lcd_data(uint8_t data) {
	LCD_PORT |= (1 << LCD_RS); // RS auf 1 setzen

	lcd_out(data); // zuerst die oberen,
	lcd_out(data << 4); // dann die unteren 4 Bit senden

	_delay_us(LCD_WRITEDATA_US);
}

void lcd_string(const char *data) {
	while (*data != '\0'){
		lcd_data(*data++);
	}
}

void dispSet(char topRow[], char bottomRow[]) {
	int i = 0;
	char first[DISPLAY_ARRAY_SIZE_FOR_CONTROLLER]  = "                                        ";
	char second[DISPLAY_ARRAY_SIZE_FOR_CONTROLLER] = "                                        ";
	for (i=0;i<DISPLAY_ARRAY_SIZE_FOR_SOFTWARE-1;i++){
		first[i] = topRow[i];
	}
	for (i=0;i<DISPLAY_ARRAY_SIZE_FOR_SOFTWARE-1;i++){
		second[i] = bottomRow[i];
	}
	//lcd_clear();
	lcd_string(first);
	lcd_string(second);
}

