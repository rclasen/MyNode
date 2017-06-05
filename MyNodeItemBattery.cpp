#include "MyNodeItemBattery.h"

MyNodeItemBattery::MyNodeItemBattery( uint8_t analog_pin, uint8_t vcc_pin,
		uint16_t min, uint16_t max,
		MyNodeTime sleep ) : MyNodeItemAnalog( analog_pin,
		vcc_pin,  50 )
{
	_sleep = sleep;
	_range = max - min;
	_min = min;
	setChild(0, MYNODE_CHILD_BATTERY, S_MULTIMETER );
};

bool MyNodeItemBattery::actionPollRun( void )
{
	nextPoll( _sleep );

	uint16_t mvolt = getMVolt();
	powerOff();

	float volt = .0001 * mvolt;
	uint8_t level = ( mvolt <= _min ) ? 0
		: 100.0 * (mvolt - _min)/_range;

#if #MYNODE_DEBUG
	Serial.print(F("MNI Battery actionPollRun volt="));
	Serial.print(volt);
	Serial.print(F(" level="));
	Serial.println(level);
#endif

	bool result = true;

	result &= send(_msg_set(0, V_VOLTAGE).set(volt,2));
	result &= sendBatteryLevel( level );

	return result;
}

