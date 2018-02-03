#ifndef MyNodeItem_h
#define MyNodeItem_h

#include "MyNodeCore.h";

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
	virtual ~MyNodeItem();

	void setSendInterval( MyTime interval );

	// sensor list management:
	uint8_t getSensorCount( void );
	uint8_t getSensorId(uint8_t snum);
	uint8_t getSensorById(uint8_t id);
	bool haveSensorId(uint8_t id);

	// inteface for MyNode:
	virtual const __FlashStringHelper *getName( void );
	virtual const size_t getNvSize( void );
	void setNvStart( uint8_t *start );
	virtual void nvDefaults( void );

	virtual void setup(void);
	void presentation(void);
	void schedule( void );
	virtual void receive( uint8_t snum, const MyMessage &msg );

	MyTime getNextTime( void );

	// TODO: abstract error reporting

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

	bool send( MyMessage &msg );

	// message helper:
	inline MyMessage& _msg( const uint8_t snum,
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

	uint8_t *_nvstart;

	MyNodeAction _nextAction;
	MyTime _nextTime;
};

#endif
