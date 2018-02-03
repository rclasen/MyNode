#include "MyNodeItemBatteryAdc.h"
#include "MyAdc.h"

static const char name[] PROGMEM = "BatteryAdc";

MyNodeItemBatteryAdc::MyNodeItemBatteryAdc(
		uint8_t analog_pin,
		uint8_t vcc_pin = MYNODE_PIN_NONE ) :
		MyNodeItemVolt( MYNODE_SENSORID_BATTERY, analog_pin, vcc_pin )
{
	_type = MyBatteryAlkaline;
	_cells = 2;

	setSendInterval( MYNODE_DAY );

	// factor = MYNODE_VOLT * ( R1 + R2 ) / R2
	// default for common setup: R1 = 1M, R2 = 4k7
	setFactor( MYNODE_VOLT * ( 1000000 + 4700 ) / 4700 );
};


const __FlashStringHelper *MyNodeItemBatteryAdc::getName( void )
{
	return PGMT(name);
}

void MyNodeItemBatteryAdc::setBatteryType( MyBatteryType type )
{
	_type = type;
}

void MyNodeItemBatteryAdc::setBatteryCells( uint8_t cells )
{
	_cells = cells;
}

void MyNodeItemBatteryAdc::actionPollRun( void )
{
	MyNodeItemVolt::actionPollRun();

	uint8_t level = MyBatteryCircuit( _mvolt, 0, _cells, _type );

#if MYNODE_DEBUG
	Serial.print(F("MNI BatAdc mvolt="));
	Serial.print(_mvolt);
	Serial.print(F(" level="));
	Serial.println(level);
#endif

	sendBatteryLevel( level );
	// TODO: handle failed send
}

