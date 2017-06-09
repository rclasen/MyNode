#ifndef MyNodeItemVolt_h
#define MyNodeItemVolt_h

#include "MyNode.h"

class MyNodeItemVolt : public MyNodeItem {
public:
	MyNodeItemVolt( uint8_t id,
			uint8_t analog_pin,
			uint8_t vcc_pin = MYNODE_PIN_NONE );

	virtual void registered(void);

protected:
	inline void nextActionPoll( MyNodeTime delay = 0 )
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

	MyNodeTime _wait;
private:
	uint8_t _analog, _vcc;
};

#endif
