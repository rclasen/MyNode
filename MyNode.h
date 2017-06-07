#ifndef MyNode_h
#define MyNode_h

#include <Arduino.h>
#include <core/MySensorsCore.h>

#define MYNODE_DEBUG 1

#include "MyNodeTime.h"
#include "MyNodePanic.h"

#define PGMT( pgm_ptr ) \
	(reinterpret_cast< const __FlashStringHelper * >( pgm_ptr ))

/************************************************************
 *
 * MyNodeItem
 *
 ************************************************************/
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
	MYNODE_ACTION_NONE,
	MYNODE_ACTION_LAST,
} MyNodeAction;

#define MYNODE_PIN_NONE UINT8_MAX

// TODO: consider define for static clientv allocation

class MyNodeItemChild {
public:
	MyNodeItemChild( uint8_t _id = MYNODE_CHILD_NONE, mysensor_sensor
			_sensor = S_CUSTOM ) : id(_id), sensor(_sensor) {};

	uint8_t id;
	uint8_t sensor;
};

extern MyMessage _nodeMsg;

// interfacing with actual sensor/actor (=item)
// init, poll, ...
// get/set values
// power on/off as needed
class MyNodeItem {
public:
	MyNodeItem( uint8_t childc );
#if do_deletes
	virtual ~MyNodeItem();
#endif

	// for MyNode init:
	uint8_t getChildCount( void );
	uint8_t getChildId(uint8_t child);
	uint8_t getChildById(uint8_t id);
	bool haveChildId(uint8_t id);

	// dispatched by MyNode:
	virtual void before(void);
	void presentation(void);
	void schedule( void );

	MyNodeTime getNextTime( void );

	// TODO: runtime config?
	// TODO: receive data?
	// TODO: only send data when it changed?

protected:
	void MyNodeItem::setChild(uint8_t child, uint8_t id, mysensor_sensor sensor );

	virtual void runAction( MyNodeAction action );

	MyNodeAction getNextAction( void );

	void nextAt( MyNodeAction action, MyNodeTime time );

	inline void nextAction( MyNodeAction action, MyNodeTime delay = 0 )
	{
		nextAt( action, MyNodeNext( delay ));
	};

	inline MyMessage& _msg_set( const uint8_t child,
			const mysensor_data vtype,
			const uint8_t destination = 0,
			const mysensor_command cmd = C_SET,
			const bool ack = false)
	{
		return build( _nodeMsg, destination, getChildId(child), cmd,
				vtype, ack);
	};

private:
	uint8_t _childc;	// child count
	MyNodeItemChild *_childv;	// child IDs

	MyNodeAction _nextAction;
	MyNodeTime _nextTime;
};

/************************************************************
 *
 * MyNode
 *
 ************************************************************/

/*
 * no need to waste space by using a class for MyNode
 * as there must be a single node instance only
 * and customization should happen in MyNodeItems

 * This core layer just does scheduling / dispatching to items

 * use MyNodeItemBatteryVCC for battery reporting

 * use MyNodeItemControl for
 * - assign new node id?
 * - get/set runtime config of items / node
 * - trigger update of all items

 * each item has builtin support for
 * - trigger an update

 * MySensors has builtin support for
 * - reboot
 * - ping/heartbeat
 * - bootup message (MyNode may send sketch name/version, too)

 * TODO: consider to make itemv extern... for static allocation in
 * sketch - or use define for static itemv allocation
 */

#define MYNODE_ITEM_NONE 255

void MyNodeInit( uint8_t itemc = 3 );
#if do_deletes
void MyNodeEnd( void );
#endif

void MyNodeRegisterItem( MyNodeItem *item );

// hooks to run from mysensors:
// need to be called manually in your .ino
void MyNodeBefore();
void MyNodePresentation( const __FlashStringHelper *name, const __FlashStringHelper *version );
void MyNodeLoop();
void MyNodeReceive(const MyMessage & msg);
void MyNodeReceiveTime(unsigned long ts);

// MyNodeItems may request extra features
// linker only adds this code, if it's used
void MyNodeEnableAdc( void );

#endif
