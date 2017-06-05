#ifndef MyNode_h
#define MyNode_h

#include "MyNodeItem.h"
#include "MyNodePanic.h"

#define MYNODE_ITEM_NONE 255

// no need to waste space by using a class
// as there must be a single node instance only
// and customization should happen in MyNodeItems

// This core layer just does scheduling / dispatching to items

// TODO: make itemv extern... for static allocation in sketch
// TODO: or use define for static itemv allocation
// TODO: remove clientv, lookup IDs on items

void MyNodeInit( uint8_t itemc = 3, uint8_t childc = 5 );
#if do_deletes
void MyNodeEnd( void );
#endif

void MyNodeRegisterItem( MyNodeItem *item );

// hooks to run from mysensors:
// need to be called manually in your .ino
void MyNodeBefore();
void MyNodePresentation( PGM_P name, PGM_P version );
void MyNodeLoop();
void MyNodeReceive(const MyMessage & msg);
void MyNodeReceiveTime(unsigned long ts);

#endif
