/*
 * File:   displayControl.h
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 27. November 2016, 14:01
 */

#ifndef DISPLAYCONTROL_H
#define DISPLAYCONTROL_H

#include "thermoTypes.h"

// Maximale Groesse der Array im Controller des Displays
#define DISPLAY_ARRAY_SIZE_FOR_CONTROLLER 41

// Maximale Groesse der Arrays in Disp Set
#define DISPLAY_ARRAY_SIZE_FOR_SOFTWARE 17 


#define LCD_PORT      PORTD
#define LCD_DDR       DDRD
#define LCD_DATABITS        PD4

//  RS: 1=Data, 0=Command)
#define LCD_REGISTER_SELECT        PD2

#define LCD_ENABLE        PD3

#define LCD_COMMAND_DELAY_US          42
#define LCD_CLEAR_DISPLAY       0x01
#define LCD_CLEAR_DISPLAY_DELAY_MS    2
#define LCD_WRITEDATA_DELAY_US        46

#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00

#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_BLINKING_OFF        0x00

#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00


/************************************************************************/
/* Initialisiert das Display und die dazugehoerigen Ports               */
/************************************************************************/
void initDisp(void);

/************************************************************************/
/* Gibt zwei Char Arrays auf dem Display aus                            */
/* topRow = Die obere Reihe an Zeichen                                  */
/* bottomRow = Die untere Reihe an Zeichen                              */
/************************************************************************/
void dispSet(char topRow[], char bottomRow[]);

#endif /* DISPLAYCONTROL_H */