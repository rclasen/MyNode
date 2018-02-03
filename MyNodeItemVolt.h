#ifndef MyNodeItemVolt_h
#define MyNodeItemVolt_h

#include "MyNodeItem.h"

// talk about using a mosfet to enable something just for measurement
// https://bitbucket.org/talk2/whisper-node-avr#markdown-header-voltage-monitor

// https://forum.mysensors.org/topic/581/mysensors-power-consumption/17
//  I'm using P channel MOSFET SI2333DDS-T1-GE3. It has low internal
//  resistance G is conencted to Arduino DO, S to 3.3 V and D to switch
//  regulator.

class MyNodeItemVolt : public MyNodeItem {
public:
	MyNodeItemVolt( uint8_t id,
			uint8_t analog_pin,
			uint8_t vcc_pin = MYNODE_PIN_NONE );

	virtual const __FlashStringHelper *getName( void );

	void setFactor( float factor );

	virtual void setup(void);

protected:
	inline void nextActionPoll( MyTime delay = 0 )
	{
		nextAction( ( _vcc == MYNODE_PIN_NONE)
			? MYNODE_ACTION_POLLRUN
			: MYNODE_ACTION_POLLPREPARE,
			delay );
	};

	virtual void runAction(MyNodeAction action);
	void actionPollPrepare(void);
	virtual void actionPollRun(void);

	// enable/disable digital out pin to supply voltage to sensor
	void powerOn( void );
	void powerOff( void );

	MyTime _wait;
	float _factor;
private:
	uint8_t _analog, _vcc;
};

#endif
