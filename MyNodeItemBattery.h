
#ifndef MyNodeItemBattery_h
#define MyNodeItemBattery_h

#include "MyNodeItemAnalog.h"

class MyNodeItemBattery : public MyNodeItemAnalog {
public:
	MyNodeItemBattery( uint8_t analog_pin,
			uint8_t vcc_pin = MYNODE_PIN_NONE,
			uint16_t min = 2600,  uint16_t max = 3400,
			MyNodeTime sleep = (24 * 3600 * 1000) );

	virtual mysensor_sensor getChildSensor(uint8_t child);
	virtual mysensor_data getChildType(uint8_t child);
	virtual bool actionPollRun(void);

private:
	MyNodeTime _sleep;
	uint16_t _min, _range;
};

#endif
