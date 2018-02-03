#ifndef MyNodeItemDS18B20_h
#define MyNodeItemDS18B20_h

#include <OneWire.h>
#include <DallasTemperature.h>

#include "MyNodeItem.h"
#include "MyAvg.h"

class MyNodeItemDS18B20 : public MyNodeItem {
public:
	MyNodeItemDS18B20( uint8_t id, OneWire *bus,
		uint8_t avg_size = 5);
	virtual const __FlashStringHelper *getName( void );
	void setPolls( uint8_t polls ); // polls per sendInterval
	void setAvg( uint8_t num ); // how many values to use for averaging

protected:
	virtual void runAction( MyNodeAction action );
	void actionInit(void);
	void actionPollPrepare(void);
	void actionPollRun(void);
	void nextActionReinit();

	bool _sensorSetup(void);

private:
	DallasTemperature _sensors;
	DeviceAddress _address;
	uint8_t _resolution;
	uint8_t _run, _polls, _avg;
	MyAvg<uint16_t,uint32_t> atemp;
};

#endif
