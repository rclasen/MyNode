#ifndef MyNodeItemTSL2591_h
#define MyNodeItemTSL2591_h

#include <Wire.h> // I2C Library
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>

#include "MyNodeItem.h"
#include "MyAvg.h"

class MyNodeItemTSL2591 : public MyNodeItem {
public:
	MyNodeItemTSL2591( uint8_t id_lux, uint8_t id_vis, uint8_t id_ir,
			uint8_t avg_size = 5 );

	virtual const __FlashStringHelper *getName( void );
	void setPolls( uint8_t polls ); // polls per sendInterval
	void setAvg( uint8_t num ); // how many values to use for averaging
	void setGain( tsl2591Gain_t gain );
	void setIntegration( tsl2591IntegrationTime_t integration );

protected:
	virtual void runAction( MyNodeAction action );
	void actionInit(void);
	void actionPollPrepare(void);
	void actionPollRun(void);
	void nextActionReinit();

private:
	Sensor_TSL2591 _sensor;
	uint8_t _run, _polls, _avg;
	MyAvg<uint16_t> avis, air;
	MyAvg<float> alux;
};

#endif
