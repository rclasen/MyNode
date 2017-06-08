#ifndef MyNodeItemBatteryVcc_h
#define MyNodeItemBatteryVcc_h

#include "MyNode.h"

// provide battery status if your node is directly powered from the
// battery withoug step-up/down regulator

class MyNodeItemBatteryVcc : public MyNodeItem {
public:
	MyNodeItemBatteryVcc();

	void setVoltageMin( uint16_t min );
	void setVoltageMax( uint16_t max );

protected:
	virtual void before(void);
	virtual void runAction(MyNodeAction action);
	void actionPollRun(void);

private:
	uint16_t _min, _max;
};

#endif
