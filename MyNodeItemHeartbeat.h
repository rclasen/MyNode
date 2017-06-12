#ifndef MyNodeItemHeartbeat
#define MyNodeItemHeartbeat_h

#include "MyNode.h"

class MyNodeItemHeartbeat : public MyNodeItem {
public:
	MyNodeItemHeartbeat();

	void activity( void );

protected:
	virtual void registered(void);
	virtual void runAction(MyNodeAction action);
	void actionPollRun(void);
};

#endif
