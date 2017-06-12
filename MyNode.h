#ifndef MyNode_h
#define MyNode_h

#include <Arduino.h>
#include <core/MySensorsCore.h>

#define MYNODE_DEBUG 1
#define MYNODE_ERROR 1

#include "MyProgmem.h"
#include "MyAssert.h"
#include "MyTime.h"

/************************************************************
 *
 * MyNodeItem
 *
 ************************************************************

 * interfacing with actual sensor/actor (=item)
 * init, poll, ...
 * get/set values
 * power on/off as needed

 * even though virtual methods + inheritance come with a memory penalty
 * for vtables, it's still the way to go. Custom pointer magic to
 * replicate the same functionality is a pain and way less powerful

 * TODO: consider define for static sensorv allocation
 */

#define MYNODE_SENSORNUM_NONE UINT8_MAX
#define MYNODE_PIN_NONE UINT8_MAX

// some internal / common sensor IDs
enum {
	MYNODE_SENSORID_CONFIG = 0,
	MYNODE_SENSORID_BATTERY,
	MYNODE_SENSORID_LAST,
	MYNODE_SENSORID_NONE = UINT8_MAX,
};

// some internal / common Actions for runAction statemachine
typedef enum {
	MYNODE_ACTION_INIT,
	MYNODE_ACTION_POLLPREPARE,
	MYNODE_ACTION_POLLRUN,
	MYNODE_ACTION_NONE,
	MYNODE_ACTION_LAST,
} MyNodeAction;

class MyNodeItemSensor {
public:
	MyNodeItemSensor( uint8_t _id = MYNODE_SENSORID_NONE, mysensor_sensor
			_type = S_CUSTOM ) : id(_id), type(_type) {};

	uint8_t id;
	uint8_t type;
};

// shared message buffer
extern MyMessage _nodeMsg;

class MyNodeItem {
public:
	// constructor argument order:
	// - sendor IDs
	// - pins (maybe with defaults)
	// - allocation sizes (with defaults)
	// other settings should use setSomething methods
	MyNodeItem( uint8_t sensorc );
#if do_deletes
	virtual ~MyNodeItem();
#endif

	void setSendInterval( MyTime interval );

	// sensor list management:
	uint8_t getSensorCount( void );
	uint8_t getSensorId(uint8_t snum);
	uint8_t getSensorById(uint8_t id);
	bool haveSensorId(uint8_t id);

	// inteface for MyNode:
	virtual void registered(void);
	void presentation(void);
	void schedule( void );

	MyTime getNextTime( void );

	// TODO: move (optional) send interval to base class

	// TODO: runtime config?
	// TODO: receive data?
	// TODO: only send data when it changed?
	// TODO: retry failed send
	// TODO: retry on missing ack

protected:
	void MyNodeItem::setSensor(uint8_t snum, uint8_t id, mysensor_sensor type );

	// statemachine of this item:
	virtual void runAction( MyNodeAction action ) = 0;

	MyNodeAction getNextAction( void );

	// scheduling helper:
	void nextAt( MyNodeAction action, MyTime time );

	inline void nextAction( MyNodeAction action, MyTime delay = 0 )
	{
		nextAt( action, MyTimeNext( delay ));
	};

	// message helper:
	inline MyMessage& _msg_set( const uint8_t snum,
			const mysensor_data vtype,
			const uint8_t destination = 0,
			const mysensor_command cmd = C_SET,
			const bool ack = false)
	{
		return build( _nodeMsg, destination, getSensorId(snum), cmd,
				vtype, ack);
	};

	MyTime _interval;
private:
	uint8_t _sensorc;	// sensor count
	MyNodeItemSensor *_sensorv;	// sensor IDs

	MyNodeAction _nextAction;
	MyTime _nextTime;
};

/************************************************************
 *
 * MyNode
 *
 ************************************************************

 * no need to waste space by using a class for MyNode
 * as there must be a single node instance only
 * and customization should happen in MyNodeItems

 * This core layer just does scheduling / dispatching to items

 * use MyNodeItemBatteryVCC for battery reporting

 * use MyNodeItemControl for
 * - TODO: assign new node id?
 * - TODO: get/set runtime config of items / node
 * - TODO: trigger update of all items

 * each item has builtin support for
 * - TODO: trigger an update

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

// enable Hearbeats
void MyNodeRegisterHeartbeat( MyTime interval = 24L * 3600 * 1000 );
// Items just report activity... no matter if heartbeats are enabled.
void MyNodeActivity( void );

// hooks to run from mysensors:
// need to be called manually in your .ino
void MyNodePresentation( const __FlashStringHelper *name, const __FlashStringHelper *version );
void MyNodeLoop();
void MyNodeReceive(const MyMessage & msg);
void MyNodeReceiveTime(unsigned long ts);

// MyNodeItems may request extra features
// linker only adds this code, if it's used
void MyNodeEnableAdc( void );

// TODO: optional panic reason
void MyNodePanic( void );


#endif
