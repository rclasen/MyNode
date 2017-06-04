#ifndef MyNodeItem_h
#define MyNodeItem_h

#include <Arduino.h>
#include <core/MySensorsCore.h>

#define MYNODE_DEBUG 1

typedef uint32_t MyNodeTime;
#define MYNODE_TIME_NONE UINT32_MAX
MyNodeTime MyNodeNow( void );
void MyNodeDelta( MyNodeTime delta );

enum {
	MYNODE_CHILD_CONFIG = 0,
	MYNODE_CHILD_BATTERY,
	MYNODE_CHILD_LAST,
	MYNODE_CHILD_NONE = 255,
};

typedef enum {
	MYNODE_ACTION_INIT,
	MYNODE_ACTION_POLLPREPARE,
	MYNODE_ACTION_POLLRUN,
} MyNodeAction;

// interfacing with actual sensor/actor (=item)
// init, poll, ...
// get/set values
// power on/off as needed
class MyNodeItem {
public:
	MyNodeItem( uint8_t childc );
	virtual ~MyNodeItem();

	// for MyNode init:
	bool MyNodeItem::setChildId(uint8_t child, uint8_t id);
	uint8_t getChildCount( void );
	uint8_t getChildId(uint8_t child);
	uint8_t getChildMax( void );

	virtual mysensor_sensor getChildSensor(uint8_t child);
	virtual mysensor_data getChildType(uint8_t child);

	// dispatched by MyNode:
	virtual bool before(void);
	bool presentation(void);

	MyNodeAction getNextAction( void );
	MyNodeTime getNextTime( void );
	virtual bool runAction( void );

	virtual bool actionInit(void);
	virtual bool actionPollPrepare(void);
	virtual bool actionPollRun(void);

	// TODO& more

protected:
	MyMessage& _msg_set( const uint8_t child,
			const uint8_t destination = 0,
			const bool ack = false);

	MyNodeAction _nextAction;
	MyNodeTime _nextTime;

private:
	uint8_t _childc;	// child count
	uint8_t *_childv;	// child IDs
};

#endif
