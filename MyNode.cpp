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
	// TODO
}

void MyNode::loop()
{
#if MYNODE_DEBUG
	Serial.print(F("MN loop time: ")); Serial.println( MyNodeNow());
#endif

	MyNodeTime next_sleep = MYNODE_TIME_NONE;

	for( uint8_t i = 0; i < _itemn; ++i ){
		MyNodeTime next = _itemv[i]->getNextTime();
#if MYNODE_DEBUG
		Serial.print(F("MN loop item i="));
		Serial.print(i);
		Serial.print(F(" next="));
		Serial.println(next);
#endif
		if( next <= MyNodeNow() ){
			_itemv[i]->runAction();

			next = _itemv[i]->getNextTime();
#if MYNODE_DEBUG
			Serial.print(F("MN loop item i="));
			Serial.print(i);
			Serial.print(F(" ran, next="));
			Serial.println(next);
#endif
		}

		if( next <= next_sleep )
			next_sleep = next;
	}

	MyNodeTime now = MyNodeNow();
	if( next_sleep <= now )
		return;

	MyNodeTime duration = next_sleep - now;
#if MYNODE_DEBUG
	Serial.print(F("MN loop sleep="));
	Serial.println(duration);
#endif
	sleep( duration ); // TODO: check result, allow interrupts
	MyNodeDelta( duration );
}

void MyNode::receive(const MyMessage & msg)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receive"));
#endif
	// TODO
}

void MyNode::receiveTime(unsigned long ts)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receiveTime"));
#endif
	// TODO
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

