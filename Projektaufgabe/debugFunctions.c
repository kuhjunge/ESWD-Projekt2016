 /*
 * File:   debugFunctions.c
 * Author: Chris Deter
 *
 * Created on 27. November 2016, 13:52
 */
 #include "debugFunctions.h"

 void initDebugFeatures(void){
	 DEBUGLIGHTDDR |= 0xFB;
	 DEBUGLIGHTPORT = 0x00;
 }

 void setDebugLED(uint8_t val){
	 DEBUGLIGHTPORT = val;
 }

 void setHumDebugLED(uint8_t val){
	setDebugLED(val);
 }
 
 void setDisplayDebugLED(display_t val){
 //dispTime, dispTimeTemp, dispTempHum, dispTimeTempHum, confH, confM, confS, confDisplay
	 uint8_t newVal = 0;
	 switch(val){
		 case dispTime:
		 newVal = 0x10;
		 break;
		 case dispTimeTemp:
		 newVal = 0x20;
		 break;
		 case dispTempHum:
		 newVal = 0x30;
		 break;
		 case dispTimeTempHum:
		 newVal = 0x40;
		 break;
		 case confDisplay:
		 newVal = 0x10;
		 break;
		 case confH:
		 newVal = 0x20;
		 break;
		case confM:
		newVal = 0x30;
		break;
		case confS:
		newVal = 0x40;
		break;

	 }
	 setDebugLED(newVal);
 }