#ifndef MyNode_h
#define MyNode_h

#include "MyNodeItem.h"

#define MYNODE_ITEM_NONE 255

// hooks are called by mysensors core lib
// manages children, sleep time, scheduling, ...
// in therory "There can be only one" ... so, OO interface is just meant
// for derived classes
class MyNode {
public:
	MyNode( unsigned long sleep = 30000, uint8_t itemc = 3,
		uint8_t childc = 5 );
	virtual ~MyNode();
	bool registerItem( MyNodeItem *item );

	// hooks to run from mysensors:
	// needs to be setup manuall in your .ino
	void before();
	void presentation( PGM_P name, PGM_P version );
	void setup();
	void loop();
	void receive(const MyMessage & msg);
	void receiveTime(unsigned long ts);

	// TODO:  heartbeat, periodic poll, battery status

protected:
	uint8_t getItemCount( void );
	MyNodeItem *getItem( uint8_t item );
	uint8_t getChildCount( void );
	MyNodeItem *getItemChild( uint8_t child );

private:
	unsigned long _sleep;

	uint8_t _itemc;		// item count
	uint8_t _itemn;		// next item index
	MyNodeItem **_itemv;	// item array

	// map child -> item
	// duplicates Items' _childv
	uint8_t _childc;	// child count
	uint8_t *_childv;	// child -> item mapping
};

#endif
