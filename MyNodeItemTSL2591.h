#ifndef MyNodeItemTSL2591_h
#define MyNodeItemTSL2591_h

#include <Wire.h> // I2C Library
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>

#include "MyNode.h"
#include "MyAvg.h"

class MyNodeItemTSL2591 : public MyNodeItem {
public:
	MyNodeItemTSL2591( uint8_t id_lux, uint8_t id_vis, uint8_t id_ir,
			uint8_t avg_size = 5 );

	void setPolls( uint8_t polls ); // polls per sendInterval
	void setAvg( uint8_t num ); // how many values to use for averaging
	// TODO: bool setGain();
	// TODO: bool setDuration();

	virtual void registered(void);

protected:
	virtual void runAction( MyNodeAction action );
	void actionPollPrepare(void);
	void actionPollRun(void);

private:
	Sensor_TSL2591 _sensor;
	uint8_t _run, _polls, _avg;
	MyAvg<uint16_t> avis, air;
	MyAvg<float> alux;
};

#endif
