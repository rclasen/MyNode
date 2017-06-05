#ifndef MyNodeItemAnalog_h
#define MyNodeItemAnalog_h

#include "MyNodeItem.h"
#include "MyNodeVcc.h"

class MyNodeItemAnalog : public MyNodeItem {
public:
	MyNodeItemAnalog( uint8_t analog_pin,
			uint8_t vcc_pin = MYNODE_PIN_NONE,
			MyNodeTime wait = 0);

	virtual void before(void);

protected:
	inline void nextPoll( MyNodeTime delay = 0 )
	{
		nextAction( ( _vcc == MYNODE_PIN_NONE)
			? MYNODE_ACTION_POLLRUN
			: MYNODE_ACTION_POLLPREPARE,
			delay );
	};

	virtual void runAction(MyNodeAction action);
	void actionPollPrepare(void);
	virtual void actionPollRun(void) = 0;

	// enable/disable digital out pin to supply voltage to sensor
	void powerOn( void );
	void powerOff( void );

	// get Millivolts
	uint16_t getMVolt( void );

private:
	uint8_t _analog, _vcc;
	MyNodeTime _wait;
};

#endif
