#include "MyNode.h"

#define MYNODE_ITEM_NONE 255

uint8_t _itemc = 0;		// item count
uint8_t _itemn = 0;		// next item index
MyNodeItem **_itemv = NULL;	// item array

// map child -> item
// duplicates Items' _childv
uint8_t _childc = 0;		// child count
uint8_t *_childv = NULL;	// child -> item mapping


/************************************************************
 * MyNode
 */

void MyNodeInit( uint8_t itemc, uint8_t childc )
{

	_itemv = new MyNodeItem*[itemc](NULL);
	if( _itemv == NULL ){
		MyNodePanic();
		return;
	}
	_itemc = itemc;
	_itemn = 0;

	_childv = new uint8_t[childc](MYNODE_ITEM_NONE);
	if( _childv == NULL ){
		MyNodePanic();
		return;
	}
	_childc = childc;
}

#if do_deletes
void MyNodeEnd()
{
	for( uint8_t i = 0; i < _itemn; ++i )
		delete _itemv[i];
	delete _itemv;
	delete _childv;
}
#endif

void MyNodeRegisterItem( MyNodeItem *item )
{
	if( ! item ){
		Serial.println(F("!MN REG: undefined"));
		MyNodePanic();
		return;
	}

	if( _itemn >= _itemc ){
		Serial.println(F("!MN REG: itemc"));
		MyNodePanic();
		return;
	}

	_itemv[_itemn] = item;

	uint8_t num = item->getChildCount();
#if MYNODE_DEBUG
	Serial.print(F("MN REG i="));
	Serial.print(_itemn);
	Serial.print(F(" item="));
	Serial.print((unsigned int)item);
	Serial.print(F(" num="));
	Serial.println(num);
#endif
	for( uint8_t c = 0; c < num; ++c ){
		uint8_t id = item->getChildId( c );

		if( id == MYNODE_CHILD_NONE )
			continue;

		if( id >= _childc ){
			Serial.println(F("!MN REG: childc"));
			MyNodePanic();
			return;
		}

		_childv[id] = _itemn;
	}

	++_itemn;
}

void MyNodeBefore()
{
	for( uint8_t i = 0; i < _itemn; ++i )
		_itemv[i]->before();
}

void MyNodePresentation( PGM_P name, PGM_P version )
{
	{
		char buf[MAX_PAYLOAD];

		strncpy_P(buf, name, MAX_PAYLOAD );
		buf[MAX_PAYLOAD-1] = 0;
		sendSketchInfo( buf, NULL );

		strncpy_P(buf, version, MAX_PAYLOAD );
		buf[MAX_PAYLOAD-1] = 0;
		sendSketchInfo( NULL, buf );
	}

	for( uint8_t i = 0; i < _itemn; ++i )
		_itemv[i]->presentation();
}

void MyNodeLoop()
{
	// run items... which might take some time
	for( uint8_t i = 0; i < _itemn; ++i ){
		MyNodeTime now = MyNodeNow();
		MyNodeTime next = _itemv[i]->getNextTime();
		MyNodeTime remaining = next - MyNodeNow();

		if( remaining > MYNODE_TIME_MAXDUR )
			_itemv[i]->schedule();
	}

	// now do a quick run to calculate sleep
	MyNodeTime sleep_needed = MYNODE_TIME_MAXDUR;
	MyNodeTime now = MyNodeNow();
	uint8_t nextitem = MYNODE_ITEM_NONE;

	for( uint8_t i = 0; i < _itemn; ++i ){
		MyNodeTime next = _itemv[i]->getNextTime();
		MyNodeTime remaining = next - now;

#if MYNODE_DEBUG
		Serial.print(F("MN loop i="));
		Serial.print(i);
		Serial.print(F(" item="));
		Serial.print((unsigned int)_itemv[i]);
		Serial.print(F(" now="));
		Serial.print(now);
		Serial.print(F(" next="));
		Serial.print(next);
		Serial.print(F(" remaining="));
		Serial.println(remaining);
#endif
		if( remaining > MYNODE_TIME_MAXDUR ){
			sleep_needed = 0;
			nextitem = i;

		} else if( remaining < sleep_needed ){
			sleep_needed = remaining;
			nextitem = i;
		}
	}

#if MYNODE_DEBUG
	Serial.print(F("MN loop sleep="));
	Serial.print(sleep_needed);
	Serial.print(F(" by="));
	Serial.println(nextitem);
#endif
	if( ! sleep_needed )
		return;

	int8_t ret = sleep( sleep_needed );
	if( ret == MY_WAKE_UP_BY_TIMER ){
		MyNodeDelta( sleep_needed );

	} else {
#if MYNODE_DEBUG
		Serial.print(F("MN loop int="));
		Serial.println(ret);
#endif
		// TODO: guess actual sleep duration
		// TODO: handle received interrupts
	}
}

void MyNodeReceive(const MyMessage & msg)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receive"));
#endif
	// TODO: receive()
}

void MyNodeReceiveTime(unsigned long ts)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receiveTime"));
#endif
	// TODO: receiveTime()
}

