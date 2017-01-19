/*
 * File:   displayControl.c
 * Author: Dirk Teschner, Chris Deter
 *
 * Created on 27. November 2016, 14:03
 */

#include "climaControl.h"

// ------------------ Definition der Helfer Funktionen ------------------

void transactionSequence(transaction_t command);

void output(int);

int input(unsigned);

int OneWireTouchReset(void);

void OneWireWriteBit(int);

int OneWireReadBit(void);

void OneWireWriteByte(int);

int OneWireReadByte(void);

int OneWireTouchByte(int);

void OneWireBlock(unsigned char *, int);

void initTemp();

/************************************************************************/
/* Initialisiert die Hardware, die benoetigt wird umd den				*/
/* "Luftfeuchtigkeitssensor" auszulesen                                 */
/* Author : Chris Deter                                                 */
/************************************************************************/
void initHumidity(void);


// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header                                                         */
/* Author : Chris Deter, Dirk Teschner                                  */
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
/* Siehe Header															*/
/* Author : Dirk Teschner												*/
/************************************************************************/
int16_t getTemp(void) {
    // transaction sequence with CONVERT T command
    transactionSequence(convert);

    // wait for temperature conversion
    _delay_ms(750);

    // transaction sequence with READ SCRATCHPAD command
    transactionSequence(read);

    // set bus to read-modus and read scratchpad-status
    volatile uint8_t temperatureLSB = 0;
    volatile uint8_t temperatureMSB = 0;
    volatile uint8_t userByte1 = 0;
    volatile uint8_t userByte2 = 0;
    volatile uint8_t reserved1 = 0;
    volatile uint8_t reserved2 = 0;
    volatile uint8_t countRemain = 0;
    volatile uint8_t countPerC = 0;
    volatile uint8_t crc = 0;
    volatile uint16_t temperature = 0;


    // get both temperature bytes
    temperatureLSB = OneWireReadByte();
    temperatureMSB = OneWireReadByte();
    userByte1 = OneWireReadByte();
    userByte2 = OneWireReadByte();
    reserved1 = OneWireReadByte();
    reserved2 = OneWireReadByte();
    countRemain = OneWireReadByte();
    countPerC = OneWireReadByte();
    crc = OneWireReadByte();

    temperature = temperatureLSB >> 1;


    //if(temperatureMSB > 0)
    //temperature = -temperature;

    // set bus to high impedance state
    initTemp();

    return (int16_t) temperature;
}

// --------------- Implementation der Helfer Funktionen ---------------

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

void initTemp() {
    cli();
    // Set TEMPPIN to high impedance state
    DDRB |= (TEMPPIN);
    PORTB &= ~(TEMPPIN);
    sei();
}

void transactionSequence(transaction_t command) {

    // initialize bus
    OneWireTouchReset();

    // skip ROM
    OneWireWriteByte(0xCC);

    // send convert command
    if (command == convert)
        OneWireWriteByte(0x44);
        // send read command
    else if (command == read)
        OneWireWriteByte(0xBE);
}

// send 'databyte' to 'port'

void output(int bit) {
    if (bit)
        DDRB |= (TEMPPIN);
    else
        DDRB &= ~(TEMPPIN);
}

// read byte from 'port'

int input(unsigned port) {
    int ret;
    int temp = PINB & 0x04;
    if (temp == 0)
        ret = 0;
    else
        ret = 1;

    return ret;
}


// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.

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

// Send a 1-Wire write bit. Provide 10us recovery time.

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

// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.

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

// Write 1-Wire data byte

void OneWireWriteByte(int data) {
    int loop;
    // Loop to write each bit in the byte, LS-bit first
    for (loop = 0; loop < 8; loop++) {
        OneWireWriteBit(data & 0x04);
        // shift the data byte for the next bit
        data >>= 1;
    }
}

// Read 1-Wire data byte and return it

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

// Write a 1-Wire data byte and return the sampled result.

int OneWireTouchByte(int data) {
    int loop, result = 0;
    for (loop = 0; loop < 8; loop++) {
        // shift the result to get it ready for the next bit
        result >>= 1;
        // If sending a '1' then read a bit else write a '0'
        if (data & 0x04) {
            if (OneWireReadBit())
                result |= 0x80;
        } else
            OneWireWriteBit(0);

        // shift the data byte for the next bit
        data >>= 1;
    }
    return result;
}

// Write a block 1-Wire data bytes and return the sampled result in the same buffer.

void OneWireBlock(unsigned char *data, int data_len) {
    int loop;
    for (loop = 0; loop < data_len; loop++) {
        data[loop] = OneWireTouchByte(data[loop]);
    }
}