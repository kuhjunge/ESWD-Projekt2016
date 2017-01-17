/*
* File:   displayControl.c
* Author: Alexandra Scheben, Dirk Teschner, Chris Deter
*
* Created on 27. November 2016, 14:03
*/

#include "climaControl.h"

// ------------------ Definition der Helfer Funktionen ------------------

void transactionSequence(transaction_t command);

void output(unsigned, int);

int input(unsigned);

void tickDelay(int);

int OneWireTouchReset(void);

void OneWireWriteBit(int);

int OneWireReadBit(void);

void OneWireWriteByte(int);

int OneWireReadByte(void);

int OneWireTouchByte(int);

void OneWireBlock(unsigned char *, int);

void initTemp();

/************************************************************************/
/* Initialisiert die Hardware, die benötigt wird umd den				*/
/* "Luftfeuchtigkeitssensor" auszulesen                                 */
/************************************************************************/
void initHumidity(void);


// --------- Implementation der im Header definierten Funktionen ---------

/************************************************************************/
/* Siehe Header										                    */
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
	ADCSRA  |= (1<<ADSC);
	/* Wait till conversion is complete */
	while(!(ADCSRA & (1<<ADIF)));
	/* Read the ADC Result */
	result = ADCH;
	/* Clear ADC Conversion Interrupt Flag */
	ADCSRA  |= (1 << ADIF);
	result = result / 2;
	return (result > ANALOG_MAX_PERCENT) ? ANALOG_MAX_PERCENT : result;
}

int16_t getTemp(void) {
	// transaction sequence with CONVERT T command
	transactionSequence(convert);
	
	// wait for temperature conversion
	_delay_ms(750);
	
	// transaction sequence with READ SCRATCHPAD command
	transactionSequence(read);
	
	// set bus to read-modus and read scratchpad-status and crc
	volatile int8_t temperatureLSB = 0;
	volatile int8_t temperatureMSB = 0;
	//int8_t crc;
	
	// get both temperature bytes
	temperatureLSB = OneWireReadByte();
	temperatureMSB = OneWireReadByte();
	
	volatile int16_t temperature = 0;
	
	temperature = temperatureLSB;
	//if(temperatureMSB > 0)
	//temperature = -temperature;
	
	// set bus to high impedance state
	initTemp();
	
	return temperature;
}

// --------------- Implementation der Helfer Funktionen ---------------

/************************************************************************/
/* Dokumentation im Funktionsprototyp									*/
/* Author : Chris Deter													*/
/************************************************************************/
void initHumidity(void){
	cli();			// vor dem Initialisieren Interrupts aus!
	ANALOG_PORT |= ANALOG_PORT_VALUE; // Port A 2
	ANALOG_DDR |= ANALOG_DDR_VALUE;
	ADCSRA = ADCSRA_VALUE;
	ADMUX = ADMUX_VALUE;
	sei();			// Sicherstellen, dass Interrupts wieder an sind
}

void initTemp()
{
	// Set TEMPPIN to high impedance state
	DDRB &= ~(TEMPPIN);
	PORTB |= (TEMPPIN);
}

void transactionSequence(transaction_t command) {
	
	// initialize bus
	OneWireTouchReset();
	
	// skip ROM
	OneWireWriteByte(0xCC);
	
	// send convert command
	if(command == convert)
	OneWireWriteByte(0x44);
	// send read command
	else if(command == read)
	OneWireWriteByte(0xBE);
}

// send 'databyte' to 'port'
void output(unsigned port, int databyte) {
	
	PORTB = databyte;
}

// read byte from 'port'
int input(unsigned port) {
	return PINB;
}


// Pause for exactly 'tick' number of ticks = 0.25us
void tickDelay(int tick) {
	for(int i = 0; i < tick; i++)
	_delay_us(1);
}

// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.
int OneWireTouchReset(void) {
	volatile int result;
	
	tickDelay(G);
	
	output(TEMPPORT, 0x00); // Drives DQ low
	tickDelay(H);
	output(TEMPPORT, 0x04); // Releases the bus
	tickDelay(I);
	result = input(TEMPPORT) ^ 0x04; // Sample for presence pulse from slave
	tickDelay(J); // Complete the reset sequence recovery
	PORTA = result;
	return result;
}

// Send a 1-Wire write bit. Provide 10us recovery time.
void OneWireWriteBit(int bit) {
	if(bit) {
		// Write '1' bit
		output(TEMPPORT,0x00); // Drives DQ low
		tickDelay(A);
		output(TEMPPORT,0x04); // Releases the bus
		tickDelay(B); // Complete the time slot and 10us recovery
	}
	else
	{
		// Write '0' bit
		output(TEMPPORT,0x00); // Drives DQ low
		tickDelay(C);
		output(TEMPPORT,0x04); // Releases the bus
		tickDelay(D);
	}
}

// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
int OneWireReadBit(void) {
	int result;
	output(TEMPPORT,0x00); // Drives DQ low
	tickDelay(A);
	output(TEMPPORT,0x04); // Releases the bus
	tickDelay(E);
	result = input(TEMPPORT) & 0x04; // Sample the bit value from the slave
	tickDelay(F); // Complete the time slot and 10us recovery
	return result;
}

// Write 1-Wire data byte
void OneWireWriteByte(int data) {
	int loop;
	// Loop to write each bit in the byte, LS-bit first
	for (loop = 0; loop < 8; loop++)
	{
		OneWireWriteBit(data & 0x04);
		// shift the data byte for the next bit
		data >>= 1;
	}
}

// Read 1-Wire data byte and return it
int OneWireReadByte(void) {
	int loop, result=0;
	for (loop = 0; loop < 8; loop++)
	{
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
	int loop, result=0;
	for (loop = 0; loop < 8; loop++)
	{
		// shift the result to get it ready for the next bit
		result >>= 1;
		// If sending a '1' then read a bit else write a '0'
		if (data & 0x04)
		{
			if (OneWireReadBit())
			result |= 0x80;
		}
		else
		OneWireWriteBit(0);
		
		// shift the data byte for the next bit
		data >>= 1;
	}
	return result;
}

// Write a block 1-Wire data bytes and return the sampled result in the same buffer.
void OneWireBlock(unsigned char *data, int data_len) {
	int loop;
	for (loop = 0; loop < data_len; loop++)
	{
		data[loop] = OneWireTouchByte(data[loop]);
	}
}