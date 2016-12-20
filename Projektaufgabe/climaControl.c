/* 
 * File:   displayControl.c
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 14:03
 */

#include "climaControl.h"

uint8_t result = 0;
uint8_t compare;

void initClima(void) {
#if SIMULATOR < 1
    cli(); // vor dem Initialisieren Interrupts aus!
    ANALOG_PORT |= ANALOG_PIN; // Port A 2
    ANALOG_DDR |= ANALOG_PIN;
    /* [Datasheet S250]
     * Bit 7	ADEN: ADC Enable
     * Bit 6	ADSC: ADC Start Conversion
     * Bit 5	ADATE: ADC Auto Trigger Enable
     * Bit 4	ADIF: ADC Interrupt Flag
     * Bit 3	ADIE: ADC Interrupt Enable
     * Bits 2:0	ADPS2:0: ADC Prescaler Select Bits
     */
    ADCSRA = 0b10000100;
    /* [Datasheet S249]
     * Bit 7:6	REFS1:0: Reference Selection Bits -> (11) Internal 2.56V Voltage Reference with external capacitor at AREF pin 
     * Bit 5	ADLAR: ADC Left Adjust Result
     * Bits 4:0	MUX4:0: Analog Channel and Gain Selection Bits
     */
    ADMUX = 0b11100010;
    compare = (uint8_t) 465;
    sei(); // Sicherstellen, dass Interrupts wieder an sind
#endif
}

uint8_t getHum(void) {
#if SIMULATOR < 1
    /* Start ADC Conversion */
    ADCSRA |= (1 << ADSC);
    /* Wait till conversion is complete */
    while ((ADCSRA & (1 << ADIF)) != 0x10);

    /* Read the ADC Result */
    result = ADC;
    /* Clear ADC Conversion Interrupt Flag */
    ADCSRA |= (1 << ADIF);

    /* Compare the converted result with 0.5 V */
    if (result <= compare)
        /* If less clear PORTB.0 else set PORTB.0*/
        ANALOG_PORT &= ANALOG_PIN ^ 1; //0b11111110;
    else
        ANALOG_PORT |= ANALOG_PIN;
    return (result > ANALOG_MAX_PERCENT) ? ANALOG_MAX_PERCENT : result;
#else
    return 50; // Simulator Default Value
#endif
}

int16_t getTemp(void) {
    return 255; // Simulator Default Value
}