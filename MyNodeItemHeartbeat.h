#ifndef MyNodeItemHeartbeat
#define MyNodeItemHeartbeat_h

#include "MyNodeItem.h"

class MyNodeItemHeartbeat : public MyNodeItem {
public:
	MyNodeItemHeartbeat();

	virtual const __FlashStringHelper *getName( void );
	void activity( void );

protected:
	virtual void setup(void);
	virtual void runAction(MyNodeAction action);
	void actionPollRun(void);
};

extern MyNodeItemHeartbeat _MyNodeHeartbeat;

#endif
