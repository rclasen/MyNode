#include "MyNode.h"

#define MYNODE_ITEM_NONE 255

/************************************************************
 * MyNode
 */

MyNode::MyNode( uint8_t itemc, uint8_t childc )
{

	_itemn = 0;
	_itemc = itemc;
	_itemv = new MyNodeItem*[_itemc](NULL);

	_childc = childc;
	_childv = new uint8_t[_childc](MYNODE_ITEM_NONE);
}

MyNode::~MyNode()
{
	for( uint8_t i = 0; i < _itemn; ++i )
		delete _itemv[i];
	delete _itemv;
	delete _childv;
}

bool MyNode::registerItem( MyNodeItem *item )
{
#if MYNODE_DEBUG
	Serial.print(F("MN registerItem itemn="));
	Serial.print(_itemn);
	Serial.print(F(" item="));
	Serial.println((unsigned int)item);
#endif
	if( ! item ){
		Serial.println(F("MN registerItem: itemn undefined"));
		return false;
	}

	if( _itemn >= _itemc ){
		Serial.println(F("MN registerItem: too many items"));
		return false;
	}

	_itemv[_itemn] = item;

	uint8_t num = item->getChildCount();
#if MYNODE_DEBUG
	Serial.print(F("MN registerItem num="));
	Serial.println(num);
#endif
	for( uint8_t c = 0; c < num; ++c ){
		uint8_t id = item->getChildId( c );
#if MYNODE_DEBUG
		Serial.print(F("MN registerItem c="));
		Serial.print(c);
		Serial.print(F(" id="));
		Serial.println(id);
#endif

		if( id == MYNODE_CHILD_NONE )
			continue;

		if( id >= _childc ){
			Serial.println(F("MN registerItem: child id out of range"));
			return false;
		}

		_childv[id] = _itemn;
	}

	++_itemn;
	return true;
}

void MyNode::before()
{
#if MYNODE_DEBUG
	Serial.print(F("MN before itemc="));
	Serial.print(_itemc);
	Serial.print(F(" itemn="));
	Serial.print(_itemn);
	Serial.print(F(" childc="));
	Serial.println(_childc);
#endif
	for( uint8_t i = 0; i < _itemn; ++i ){
#if MYNODE_DEBUG
		Serial.print(F("MN before"));
		Serial.print(F(" i="));
		Serial.print(i);
		Serial.print(F(" item="));
		Serial.println((unsigned int)_itemv[i]);
#endif
		_itemv[i]->before();
	}
}

void MyNode::presentation( PGM_P name, PGM_P version )
{
#if MYNODE_DEBUG
	Serial.println(F("MN presentation"));
#endif
#if 1
	{
		char buf[MAX_PAYLOAD];

		strncpy_P(buf, name, MAX_PAYLOAD );
		buf[MAX_PAYLOAD-1] = 0;
#if MYNODE_DEBUG
		Serial.print(F("MN sketch name: ")); Serial.println(buf);
#endif
		sendSketchInfo( buf, NULL );

		strncpy_P(buf, version, MAX_PAYLOAD );
		buf[MAX_PAYLOAD-1] = 0;
#if MYNODE_DEBUG
		Serial.print(F("MN sketch version: ")); Serial.println(buf);
#endif
		sendSketchInfo( NULL, buf );
	}
#endif
	for( uint8_t i = 0; i < _itemn; ++i ){
#if MYNODE_DEBUG
		Serial.print(F("MN presentation i="));
		Serial.print(i);
		Serial.print(F(" item="));
		Serial.println((unsigned int)_itemv[i]);
#endif

		bool result =  _itemv[i]->presentation();
#if MYNODE_DEBUG
		Serial.print(F("MN presentation result="));
		Serial.println(result);
#endif
	}
}

void MyNode::setup()
{
#if MYNODE_DEBUG
	Serial.println(F("MN setup"));
#endif
	// TODO: setup()
}

void MyNode::loop()
{
	// run items... which might take some time
	for( uint8_t i = 0; i < _itemn; ++i ){
		MyNodeTime now = MyNodeNow();
		MyNodeTime next = _itemv[i]->getNextTime();
		MyNodeTime remaining = next - MyNodeNow();
#if MYNODE_DEBUG
		Serial.print(F("MN loop item i="));
		Serial.print(i);
		Serial.print(F(" now="));
		Serial.print(now);
		Serial.print(F(" next="));
		Serial.print(next);
		Serial.print(F(" remaining="));
		Serial.println(remaining);
#endif
		if( remaining > MYNODE_TIME_MAXDUR )
			_itemv[i]->runAction();
	}

	// now do a quick run to calculate sleep
	MyNodeTime sleep_needed = MYNODE_TIME_MAXDUR;
	MyNodeTime now = MyNodeNow();

	for( uint8_t i = 0; i < _itemn; ++i ){
		MyNodeTime next = _itemv[i]->getNextTime();
		MyNodeTime remaining = next - now;

		if( remaining > MYNODE_TIME_MAXDUR ){
			sleep_needed = 0;

		} else if( remaining < sleep_needed ){
			sleep_needed = remaining;

		}
#if MYNODE_DEBUG
		Serial.print(F("MN loop item i="));
		Serial.print(i);
		Serial.print(F(" now="));
		Serial.print(now);
		Serial.print(F(" next="));
		Serial.print(next);
		Serial.print(F(" remaining="));
		Serial.print(remaining);
		Serial.print(F(" sleep="));
		Serial.println(sleep_needed);
#endif
	}

	if( ! sleep_needed )
		return;

	int8_t ret = sleep( sleep_needed );
	if( ret == MY_WAKE_UP_BY_TIMER ){
#if MYNODE_DEBUG
		Serial.println(F("MN loop sleep completed"));
#endif
		MyNodeDelta( sleep_needed );
	} else {
#if MYNODE_DEBUG
		Serial.print(F("MN loop sleep cancelled by int="));
		Serial.println(ret);
#endif
		// TODO: guess actual sleep duration
		// TODO: handle received interrupts
	}
}

void MyNode::receive(const MyMessage & msg)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receive"));
#endif
	// TODO: receive()
}

void MyNode::receiveTime(unsigned long ts)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receiveTime"));
#endif
	// TODO: receiveTime()
}

uint8_t MyNode::getItemCount( void )
{
	return _itemn;
}

MyNodeItem *MyNode::getItem( uint8_t item )
{
	if( item >= _itemn )
		return NULL;

	return _itemv[item];
}

uint8_t MyNode::getChildCount( void )
{
	return _childc;
}

MyNodeItem *MyNode::getItemChild( uint8_t child )
{
	if( child >= _childc )
		return NULL;

	uint8_t item = _childv[child];
	if( item >= _itemn ) // covers MYNODE_ITEM_NONE, too
		return NULL;

	return _itemv[item];
}

