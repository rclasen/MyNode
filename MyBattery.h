#ifndef MyBattery_h
#define MyBattery_h

#include <Arduino.h>

// TODO: support non-alkaline discharge behaviour

typedef enum {
	MyBatteryAlkaline,
} MyBatteryType;

// return minimum voltage
uint16_t MyBatteryMin( uint16_t circuit_min, MyBatteryType type = MyBatteryAlkaline );
uint16_t MyBatteryMinAlkaline( uint16_t circuit_min );

// return charging percentage for a single cell
uint8_t MyBatteryLevel( uint16_t mvolt, MyBatteryType type = MyBatteryAlkaline );
uint8_t MyBatteryLevelAlkaline( uint16_t mvolt );

// return charging percentage taking actual circuit into account
uint8_t MyBatteryCircuit( uint16_t bat_mvolt, uint16_t circuit_min = 0,
		uint8_t cells = 1, MyBatteryType type = MyBatteryAlkaline );

#endif
