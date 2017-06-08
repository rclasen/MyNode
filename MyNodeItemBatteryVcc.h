#ifndef MyNodeItemBatteryVcc_h
#define MyNodeItemBatteryVcc_h

#include "MyNode.h"

// provide battery status if your node is directly powered from the
// battery withoug step-up/down regulator

class MyNodeItemBatteryVcc : public MyNodeItem {
public:
	MyNodeItemBatteryVcc( uint16_t min = 2600, uint16_t max = 3400,
			MyNodeTime interval = (24 * 3600 * 1000) );

protected:
	virtual void before(void);
	virtual void runAction(MyNodeAction action);
	void actionPollRun(void);

private:
	MyNodeTime _interval;
	uint16_t _min, _max;
};

#endif
