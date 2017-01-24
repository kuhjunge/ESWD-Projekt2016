/*
 * File:   displayControl.c
 *
 * Created on 27. November 2016, 14:03
 */

#include "climaControl.h"

// ------------------ Definition der Hilfsfunktionen ------------------

void transactionSequence(transaction_t command);

void output(int);

int input(unsigned);

int OneWireTouchReset(void);

void OneWireWriteBit(int);

int OneWireReadBit(void);

void OneWireWriteByte(int);

int OneWireReadByte(void);

/************************************************************************/
/* Initialisiert die Hardware, die benoetigt wird, um den				*/
/* Temperatursensor auszulesen							                */
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
void initTemp();

/************************************************************************/
/* Initialisiert die Hardware, die benoetigt wird, um den				*/
/* "Luftfeuchtigkeitssensor" auszulesen                                 */
/* Author : Chris Deter                                                 */
/************************************************************************/
void initHumidity(void);


// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header                                                         */
/* Author : Chris Deter, Alexandra Scheben, Dirk Teschner               */
/************************************************************************/
void initClima(void) {
    initHumidity();
    initTemp();
}

/************************************************************************/
/* Siehe Header															*/
/* Author : Chris Deter													*/
/************************************************************************/
uint8_t getHum(void) {
    uint8_t result = 0;
    /* Start ADC Conversion */
    ADCSRA |= (1 << ADSC);
    /* Wait till conversion is complete */
    while (!(ADCSRA & (1 << ADIF)));
    /* Read the ADC Result */
    result = ADCH;
    /* Clear ADC Conversion Interrupt Flag */
    ADCSRA |= (1 << ADIF);
    result = result / 2;
    return (result > ANALOG_MAX_PERCENT) ? ANALOG_MAX_PERCENT : result;
}

/************************************************************************/
/* 																		*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
int16_t getTemp(void) {
    // Transaktionssequenz mit dem CONVERT T Kommando
    transactionSequence(convert);

    // Warten auf das Ende der Temperaturkonvertierung	
	while(!OneWireReadBit());

    // Transaktionssequenz mit dem READ SCRATCHPAD Kommando
    transactionSequence(read);

    // --------------- Scratchpad Daten auslesen ---------------
	
	int8_t temperatureArray[9];
	volatile int8_t temperatureRead = 0;
    volatile float temperature = 0;

    temperatureArray[0] = OneWireReadByte(); // Temperature LSB
    temperatureArray[1] = OneWireReadByte(); // Temperature MSB
    temperatureArray[2] = OneWireReadByte(); // User Byte 1
    temperatureArray[3] = OneWireReadByte(); // User Byte 2
    temperatureArray[4] = OneWireReadByte(); // Reserved 1
    temperatureArray[5] = OneWireReadByte(); // Reserved 2
    temperatureArray[6] = OneWireReadByte(); // COUNT REMAIN
    temperatureArray[7] = OneWireReadByte(); // COUNT PER °C
    temperatureArray[8] = OneWireReadByte(); // CRC
	
	// Abschneiden des letzten Bits, weil dies nur die Nachkommastelle angibt
	temperatureRead = temperatureArray[0] >> 1;
	
	// Temperatur mit hoeherer Genauigkeit berechnen
	temperature = temperatureRead - 0.25 + ((float)(temperatureArray[7] - temperatureArray[6]) / (float)temperatureArray[7]);

	// Interne Repraesentation
	temperature *= 10;

	// Wenn die Temperatur negativ ist, den Wert negieren
    if(temperatureArray[1] > 0)
		temperature = -temperature;

    // Bus zuruecksetzen
    initTemp();

    return (int16_t) temperature;
}

// --------------- Implementation der Hilfsfunktionen ---------------

/************************************************************************/
/* Dokumentation im Funktionsprototyp									*/
/* Author : Chris Deter													*/
/************************************************************************/
void initHumidity(void) {
    cli(); // vor dem Initialisieren Interrupts aus!
    ANALOG_PORT |= ANALOG_PORT_VALUE; // Port A 2
    ANALOG_DDR |= ANALOG_DDR_VALUE;
    ADCSRA = ADCSRA_VALUE;
    ADMUX = ADMUX_VALUE;
    sei(); // Sicherstellen, dass Interrupts wieder an sind
}

/************************************************************************/
/* Dokumentation im Funktionsprototyp									*/
/* Author : Alexandra Scheben, Dirk Teschner							*/
/************************************************************************/
void initTemp() {
    cli();
    // DDR und PORT Register des Temperatursensors werden so eingestellt,
	// dass der PIN standartmaessig auf logisch 1 gehalten wird
    TEMPDDR |= (TEMPPIN);
    TEMPPORT &= ~(TEMPPIN);
    sei();
}


/************************************************************************/
/* Transaktionssequenz, die entweder ein CONVERT T oder READ SCRATCHPAD */
/* Kommando an den Temperatursensor sendet							    */
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
void transactionSequence(transaction_t command) {
    // Bus zuruecksetzen
    OneWireTouchReset();

    // SKIP ROM
    OneWireWriteByte(0xCC);

    // CONVERT T Kommando senden
    if (command == convert)
        OneWireWriteByte(0x44);
    // READ SCRATCHPAD Kommando senden
    else if (command == read)
        OneWireWriteByte(0xBE);
}

/************************************************************************/
/* Setzt das DDR Register des Temperatursensors,						*/
/* um entsprechendes Bit zu senden										*/
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
void output(int bit) {
    if (!bit)
        TEMPDDR |= (TEMPPIN);
    else
        TEMPDDR &= ~(TEMPPIN);
}

/************************************************************************/
/* Liest ein Bit des an den Temperatursensor angeschlossenen Pins		*/
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
int input(unsigned port) {
    int ret;
    int temp = TEMPPINREGISTER & TEMPPIN;
    if (temp == 0)
        ret = 0;
    else
        ret = 1;

    return ret;
}


/************************************************************************/
/* OneWire Touch Reset													*/
/* Returnt 1, wenn kein angeschlossenes Gerät erkannt wurde, ansonsten 0*/
/* Implementation entsprechend der Beispielfunktion aus dem				*/
/* "Application Note zum 1-Wire Protokoll.pdf"							*/
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
int OneWireTouchReset(void) {
    volatile int result;

    _delay_us(G);

    output(0); // Drives DQ low
    _delay_us(H);
    output(1); // Releases the bus
    _delay_us(I);
    result = input(TEMPPORT); // ^ 0x04; // Sample for presence pulse from slave
    _delay_us(J); // Complete the reset sequence recovery
    return result;
}

/************************************************************************/
/* Sendet ein Bit ueber den OneWire Kanal								*/
/* Haelt 10 µs Erholungszeit ein										*/
/* Implementation entsprechend der Beispielfunktion aus dem				*/
/* "Application Note zum 1-Wire Protokoll.pdf"							*/
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
void OneWireWriteBit(int bit) {
    if (bit) {
        // Write '1' bit
        output(0); // Drives DQ low
        _delay_us(A);
        output(1); // Releases the bus
        _delay_us(B); // Complete the time slot and 10us recovery
    } else {
        // Write '0' bit
        output(0); // Drives DQ low
        _delay_us(C);
        output(1); // Releases the bus
        _delay_us(D);
    }
}

/************************************************************************/
/* Empfaengt ein Bit ueber den OneWire Kanal und gibt es zurueck		*/
/* Haelt 10 µs Erholungszeit ein										*/
/* Implementation entsprechend der Beispielfunktion aus dem				*/
/* "Application Note zum 1-Wire Protokoll.pdf"							*/
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
int OneWireReadBit(void) {
    int result;
    output(0); // Drives DQ low
    _delay_us(A);
    output(1); // Releases the bus
    _delay_us(E);
    result = input(TEMPPORT); // Sample the bit value from the slave
    _delay_us(F); // Complete the time slot and 10us recovery
    return result;
}

/************************************************************************/
/* Sendet ein Byte ueber den OneWire Kanal								*/
/* Implementation entsprechend der Beispielfunktion aus dem				*/
/* "Application Note zum 1-Wire Protokoll.pdf"							*/
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
void OneWireWriteByte(int data) {
    int loop;
    // Loop to write each bit in the byte, LS-bit first
    for (loop = 0; loop < 8; loop++) {
        OneWireWriteBit(data & 0x01);
        // shift the data byte for the next bit
        data >>= 1;
    }
}

/************************************************************************/
/* Empfaengt ein Byte ueber den OneWire Kanal und gibt es zurueck		*/
/* Implementation entsprechend der Beispielfunktion aus dem				*/
/* "Application Note zum 1-Wire Protokoll.pdf"							*/
/* Author : Alexandra Scheben, Dirk Teschner                            */
/************************************************************************/
int OneWireReadByte(void) {
    int loop, result = 0;
    for (loop = 0; loop < 8; loop++) {
        // shift the result to get it ready for the next bit
        result >>= 1;
        // if result is one, then set MS bit
        if (OneWireReadBit())
            result |= 0x80;
    }
    return result;
}