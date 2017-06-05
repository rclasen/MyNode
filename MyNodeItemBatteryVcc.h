#ifndef MyNodeItemBatteryVcc_h
#define MyNodeItemBatteryVcc_h

#include "MyNodeItem.h"
#include "MyNodeVcc.h"

class MyNodeItemBatteryVcc : public MyNodeItem {
public:
	MyNodeItemBatteryVcc( uint16_t min = 2600, uint16_t max = 3400,
			MyNodeTime sleep = (24 * 3600 * 1000) );

protected:
	virtual void before(void);
	virtual void runAction(MyNodeAction action);
	void actionPollRun(void);

private:
	MyNodeTime _sleep;
	uint16_t _min, _max;
};

#endif
