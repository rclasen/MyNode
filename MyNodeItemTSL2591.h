#ifndef MyNodeItemTSL2591_h
#define MyNodeItemTSL2591_h

#include <Wire.h> // I2C Library
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>

#include "MyNodeItem.h"

class MyNodeItemTSL2591 : public MyNodeItem {
public:
	MyNodeItemTSL2591( uint8_t lux, uint8_t visible, uint8_t ir,
			MyNodeTime sleep );

	// TODO: compile time options to set gain and integration time

	virtual void before(void);

protected:
	virtual void runAction( MyNodeAction action );
	void actionPollPrepare(void);
	void actionPollRun(void);

private:
	Adafruit_TSL2591 _sensor;
	MyNodeTime _sleep;
};

#endif
