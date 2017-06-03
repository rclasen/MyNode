#include <assert.h>
#include "MyNodeItem.h"

static MyMessage _msgTmp;

/************************************************************
 * MyNodeItem
 */

MyNodeItem::MyNodeItem( uint8_t child_id )
{
	MyNodeItem( 1, &child_id );
}

MyNodeItem::MyNodeItem( uint8_t childc, uint8_t *childv )
{
	_childv = new uint8_t[childc];
	memcpy( _childv, childv, childc );
	_childc = childc;
}

MyNodeItem::~MyNodeItem()
{
	delete _childv;
}

uint8_t MyNodeItem::getChildCount( void )
{
	return _childc;
}

uint8_t MyNodeItem::getChildId(uint8_t child)
{
	assert(child < _childc);

	return _childv[child];
}

mysensor_sensor MyNodeItem::getChildSensor(uint8_t child)
{
	return S_CUSTOM;
}

uint8_t MyNodeItem::getChildMax( void )
{
	uint8_t max = 0;

	for( uint8_t c = 0; c < _childc; ++c ){
		if( _childv[c] > max )
			max = _childv[c];
	}

	return max;
}

bool MyNodeItem::presentation( void )
{
	bool result = true;

	for( uint8_t c = 0; c < _childc; ++c )
		result &= present( getChildId(c), getChildSensor(c) );

	return result;
}

MyMessage& MyNodeItem::_msg_set( const uint8_t child,
		const uint8_t destination,
		const bool ack)
{
	return build( _msgTmp, destination, getChildId(child), C_SET,
			getChildSensor(child), ack);
}

