#ifndef MyNodeItem_h
#define MyNodeItem_h

#include <Arduino.h>
#include <core/MySensorsCore.h>

#define MYNODE_DEBUG 1

enum {
	MYNODE_CHILD_CONFIG = 0,
	MYNODE_CHILD_BATTERY,
	MYNODE_CHILD_LAST,
	MYNODE_CHILD_NONE = 255,
};

// interfacing with actual sensor/actor (=item)
// init, poll, ...
// get/set values
// power on/off as needed
class MyNodeItem {
public:
	MyNodeItem( uint8_t childc = 1 );
	virtual ~MyNodeItem();

	// for MyNode init:
	bool MyNodeItem::setChildId(uint8_t child, uint8_t id);
	uint8_t getChildCount( void );
	uint8_t getChildId(uint8_t child);
	uint8_t getChildMax( void );

	virtual mysensor_sensor getChildSensor(uint8_t child);

	// dispatched by MyNode:
	virtual bool before(void);
	bool presentation(void);
	virtual bool loop(void);
	// TODO& more

protected:
	MyMessage& _msg_set( const uint8_t child,
			const uint8_t destination = 0,
			const bool ack = false);

private:
	uint8_t _childc;	// child count
	uint8_t *_childv;	// child IDs
};

#endif
