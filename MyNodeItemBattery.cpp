#include "MyNodeItemBattery.h"

MyNodeItemBattery::MyNodeItemBattery( uint8_t analog_pin, uint8_t vcc_pin,
		uint16_t min, uint16_t max,
		MyNodeTime sleep ) : MyNodeItemAnalog( analog_pin,
		vcc_pin,  50 )
{
	_sleep = sleep;
	_max = max;
	_min = min;
	setChild(0, MYNODE_CHILD_BATTERY, S_MULTIMETER );
};

void MyNodeItemBattery::actionPollRun( void )
{
	nextActionPoll( _sleep );

	uint16_t mvolt = getMVolt();
	powerOff();

	float volt = .001 * mvolt;

	uint8_t level;
	if( mvolt <= _min ){
		level = 0;

	} else if( mvolt > _max ){
		level = 100;

	} else {
		level = 100.0 * (mvolt - _min) / (_max - _min);;
	}

#if MYNODE_DEBUG
	Serial.print(F("MNI Battery volt="));
	Serial.print(volt);
	Serial.print(F(" level="));
	Serial.println(level);
#endif

	send(_msg_set(0, V_VOLTAGE).set(volt,2));
	sendBatteryLevel( level );
	// TODO: handle failed send
}

