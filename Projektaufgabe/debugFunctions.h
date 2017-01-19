 /*
 * File:   debugFunctions.h
 * Author: Alexandra Scheben, Dirk Teschner, Chris Deter
 *
 * Created on 27. November 2016, 13:52
 */

 #ifndef DEBUGFUNCTIONS_H
 #define DEBUGFUNCTIONS_H
 
 #include "thermoTypes.h"

 #define DEBUGLIGHTPORT PORTB
 #define DEBUGLIGHTDDR DDRB

 void initDebugFeatures(void);

 void setDebugLED(uint8_t val);

 void setHumDebugLED(uint8_t val);

 void setDisplayDebugLED(display_t val);

 #endif /* DEBUGFUNCTIONS_H */