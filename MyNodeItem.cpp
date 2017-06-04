#include "MyNodeItem.h"

static MyMessage _msgTmp;

/************************************************************
 * MyNodeItem
 */

MyNodeItem::MyNodeItem( uint8_t childc )
{
	_childc = childc;
	_childv = new uint8_t[childc](MYNODE_CHILD_NONE);
}

MyNodeItem::~MyNodeItem()
{
	delete _childv;
}

uint8_t MyNodeItem::getChildCount( void )
{
	return _childc;
}

bool MyNodeItem::setChildId(uint8_t child, uint8_t id)
{
	if( child >= _childc ){
		Serial.println(F("MNI setChildId: child out of bounds"));
		return false;
	}

	_childv[child] = id;
	return true;
}

uint8_t MyNodeItem::getChildId(uint8_t child)
{
	if( child >= _childc ){
		Serial.println(F("MNI getChildId: child out of bounds"));
		return 0;
	}

#if MYNODE_DEBUG
	Serial.print(F("MNI getChildID "));
	Serial.print(child);
	Serial.print(F(" id="));
	Serial.println(_childv[child]);
#endif
	return _childv[child];
}

mysensor_sensor MyNodeItem::getChildSensor(uint8_t child)
{
	(void)child;

	return S_CUSTOM;
}

uint8_t MyNodeItem::getChildMax( void )
{
	uint8_t max = 0;

	for( uint8_t c = 0; c < _childc; ++c ){
		if( _childv[c] == MYNODE_CHILD_NONE )
			continue;

		if( _childv[c] > max )
			max = _childv[c];
	}

#if MYNODE_DEBUG
	Serial.print(F("MNI getChildMax "));
	Serial.println(max);
#endif
	return max;
}

bool MyNodeItem::before( void )
{
#if MYNODE_DEBUG
	Serial.print(F("MNI before childc="));
	Serial.println(_childc);
#endif
}


bool MyNodeItem::presentation( void )
{
	bool result = true;

	for( uint8_t c = 0; c < _childc; ++c ){
#if MYNODE_DEBUG
		Serial.print(F("MNI presentation c="));
		Serial.println( c );
#endif
		result &= present( getChildId(c), getChildSensor(c) );
	}

#if MYNODE_DEBUG
	Serial.print(F("MNI presentation result="));
	Serial.println( result );
#endif
	return result;
}

bool MyNodeItem::loop( void )
{
#if MYNODE_DEBUG
	Serial.println(F("MNI loop"));
#endif
}



MyMessage& MyNodeItem::_msg_set( const uint8_t child,
		const uint8_t destination,
		const bool ack)
{
	return build( _msgTmp, destination, getChildId(child), C_SET,
			getChildSensor(child), ack);
}

