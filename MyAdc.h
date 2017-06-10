#ifndef MyAdc_h
#define MyAdc_h

#include "MyTime.h"

// calibration: set/get actual voltage of internal ADC reference
void MyAdcIntrefSet( uint16_t actual_mvolts );
uint16_t MyAdcIntrefGet();

// return VCC in millivolts
uint16_t MyAdcVcc( MyTime maxage = 60000 );

// return voltage at pin in mvolts (0..VCC)
uint16_t MyAdcRead( uint8_t pin, MyTime vccage = 60000 );

#endif
