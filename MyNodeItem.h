#ifndef MyNodeItem_h
#define MyNodeItem_h

#include <Arduino.h>
#include <core/MySensorsCore.h>

//#include "MyNodeItem.h"

enum {
	MYNODE_CHILD_CONFIG,
	MYNODE_CHILD_BATTERY,
	MYNODE_CHILD_LAST,
};

// interfacing with actual sensor/actor (=item)
// init, poll, ...
// get/set values
// power on/off as needed
class MyNodeItem {
public:
	MyNodeItem( ) : _childc(0), _childv(NULL) {};
	MyNodeItem( uint8_t child_id );
	MyNodeItem( uint8_t childc, uint8_t *childv );
	virtual ~MyNodeItem();

	// for MyNode init:
	uint8_t getChildCount( void );
	uint8_t getChildId(uint8_t child);
	uint8_t getChildMax( void );

	virtual mysensor_sensor getChildSensor(uint8_t child);

	// dispatched by MyNode:
	virtual bool before(void) { return true; };
	bool presentation(void);
	virtual bool loop(void) { return true; };
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
