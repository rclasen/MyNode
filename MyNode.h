#ifndef MyNode_h
#define MyNode_h

#include "MyNodeItem.h"

// hooks are called by mysensors core lib
// manages children, sleep time, scheduling, ...
// in therory "There can be only one" ... so, OO interface is just meant
// for derived classes
class MyNode {
public:
	MyNode( uint8_t itemc, const MyNodeItem *itemv );
	virtual ~MyNode();

	// hooks to run from mysensors:
	// needs to be setup manuall in your .ino
	void before();
	void presentation( PGM_P name, PGM_P version );
	void setup();
	void loop();
	void receive(const MyMessage & msg);
	void receiveTime(unsigned long ts);

private:
	uint8_t _itemc;		// item count
	MyNodeItem *_itemv;	// item array

	// map child -> item
	// duplicates Items' _childv
	uint8_t _childc;	// child count
	uint8_t *_childv;	// child -> item mapping
};

#endif
