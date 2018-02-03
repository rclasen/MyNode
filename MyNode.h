#ifndef MyNode_h
#define MyNode_h

#include <Arduino.h>
#include <core/MySensorsCore.h>

#include "MyNodeItem.h"
#include "MyNodeItemHeartbeat.h"
// TODO: #include "MyNodeItemControl.h"

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
 * - TODO: set Adc intref
 * - TODO: set heartbeat interval
 * - TODO: get/set runtime config of items / node
 * - TODO: trigger update of all items

 * each item has builtin support for
 * - TODO: trigger an update

 * MySensors has builtin support for
 * - reboot
 * - ping/heartbeat
 * - bootup message (MyNode may send sketch name/version, too)

 */

extern const char SKETCH_NAME[] PROGMEM;
extern const char SKETCH_VERSION[] PROGMEM;

// TODO: @_MyNodeControl,\

#define MyNodeItems(...) \
MyNodeItem *_MyNodeItems[] = {\
	&_MyNodeHeartbeat,\
	__VA_ARGS__,\
	NULL\
};

void MyNodeInit( void ) __attribute__((weak));

#endif
