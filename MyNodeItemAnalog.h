#ifndef MyNodeItemAnalog_h
#define MyNodeItemAnalog_h

#include "MyNodeItem.h"
#include "MyNodeVcc.h"

class MyNodeItemAnalog : public MyNodeItem {
public:
	MyNodeItemAnalog( uint8_t analog_pin,
			uint8_t vcc_pin = MYNODE_PIN_NONE,
			MyNodeTime wait = 0);

	virtual bool before(void);

protected:
	inline void nextPoll( MyNodeTime delay = 0 )
	{
		nextAction( ( _vcc == MYNODE_PIN_NONE)
			? MYNODE_ACTION_POLLRUN
			: MYNODE_ACTION_POLLPREPARE,
			delay );
	};

	virtual bool runAction(MyNodeAction action);
	bool actionPollPrepare(void);
	virtual bool actionPollRun(void) = 0;

	// enable/disable digital out pin to supply voltage to sensor
	bool powerOn( void );
	bool powerOff( void );

	// get Millivolts
	uint16_t getMVolt( void );

private:
	uint8_t _analog, _vcc;
	MyNodeTime _wait;
};

#endif
