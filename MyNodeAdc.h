#ifndef MyNodeAdc_h
#define MyNodeAdc_h

#include "MyTime.h"

// calibration: set/get actual voltage of internal ADC reference
void MyNodeAdcIntrefSet( uint16_t actual_mvolts );
uint16_t MyNodeAdcIntrefGet();

// return VCC in millivolts
uint16_t MyNodeAdcVcc( MyTime maxage = 60000 );

// return voltage at pin in mvolts (0..VCC)
uint16_t MyNodeAdcRead( uint8_t pin, MyTime vccage = 60000 );

#endif
