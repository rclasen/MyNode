#include "MyBattery.h"
#include "MyAssert.h"

static const char ALOC[] PROGMEM = "MyBattery.cpp";
#define ASSERT(e) myassert(PGMT(ALOC), e );

/************************************************************
 * Alkaline batteries
 */

#define ALKALINE_MIN 800
#define ALKALINE_MAX 1500

uint16_t MyBatteryMinAlkaline( uint16_t circuit_min )
{
	if( circuit_min < ALKALINE_MIN )
		return ALKALINE_MIN;

	return circuit_min;
}

uint8_t MyBatteryLevelAlkaline( uint16_t mvolt )
{
	if( mvolt >= ALKALINE_MAX )
		return 100;

	if( mvolt <= ALKALINE_MIN )
		return 0;

	// alkaline cell voltage drops "mostly" proportional to charging level
	return (float) 100 * (mvolt - ALKALINE_MIN)
		/ (ALKALINE_MAX - ALKALINE_MIN);
}

/************************************************************
 * global
 */

uint16_t MyBatteryMin( uint16_t circuit_min, MyBatteryType type )
{
	switch( type ){
	  case MyBatteryAlkaline:
		return MyBatteryMinAlkaline( circuit_min );
	}

	return 0;
}

uint8_t MyBatteryLevel( uint16_t mvolt, MyBatteryType type )
{
	switch( type ){
	  case MyBatteryAlkaline:
		return MyBatteryLevelAlkaline( mvolt );
	}

	return 255;
}

uint8_t MyBatteryCircuit( uint16_t bat_mvolt, uint16_t circuit_min,
		uint8_t cells, MyBatteryType type )
{
	ASSERT(cells);
	if( ! cells )
		return 0;

	uint16_t min = MyBatteryMin( circuit_min / cells, type );
	uint8_t lmin = MyBatteryLevel(min, type );

	if( lmin >= 100 )
		return 0;

	uint8_t lbat = MyBatteryLevel( bat_mvolt / cells, type );

	return (float)100 * (lbat - min) / (100 - min);
}


