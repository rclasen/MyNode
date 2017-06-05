#include "MyNodeItemBattery.h"

MyNodeItemBattery::MyNodeItemBattery( uint8_t analog_pin, uint8_t vcc_pin,
		uint16_t min, uint16_t max,
		MyNodeTime sleep ) : MyNodeItemAnalog( analog_pin,
		vcc_pin,  50 )
{
	_sleep = sleep;
	_range = max - min;
	_min = min;
	setChildId(0, MYNODE_CHILD_BATTERY );
};

mysensor_sensor MyNodeItemBattery::getChildSensor(uint8_t child)
{
	return S_MULTIMETER;
};

mysensor_data MyNodeItemBattery::getChildType(uint8_t child)
{
	return V_VOLTAGE;
};


bool MyNodeItemBattery::actionPollRun( void )
{
	_nextTime = MyNodeNext( _sleep );

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

	result &= send(_msg_set(0).set(volt,2));
	result &= sendBatteryLevel( level );

	return result;
}

