#include "MyNodeItem.h"

MyMessage _nodeMsg;


/************************************************************
 * MyNodeItem
 */

MyNodeItem::MyNodeItem( uint8_t childc )
{
	_childc = childc;
	_childv = new MyNodeItemChild[childc]();
	nextAction( MYNODE_ACTION_INIT );
}

MyNodeItem::~MyNodeItem()
{
	delete _childv;
}

uint8_t MyNodeItem::getChildCount( void )
{
	return _childc;
}

bool MyNodeItem::setChild(uint8_t child, uint8_t id, mysensor_sensor sensor )
{
	if( child >= _childc ){
		Serial.println(F("MNI setChild: child out of bounds"));
		return false;
	}

	_childv[child].id = id;
	_childv[child].sensor = sensor;
	return true;
}

uint8_t MyNodeItem::getChildId(uint8_t child)
{
	if( child >= _childc ){
		Serial.println(F("MNI getChildId: child out of bounds"));
		return 0;
	}

	return _childv[child].id;
}

bool MyNodeItem::before( void )
{
	nextAction( MYNODE_ACTION_POLLPREPARE );
	return true;
}

bool MyNodeItem::presentation( void )
{
	bool result = true;

	for( uint8_t c = 0; c < _childc; ++c ){
		if( _childv[c].id == MYNODE_CHILD_NONE )
			continue;

		result &= present( _childv[c].id, _childv[c].sensor );
	}

	return result;
}

MyNodeAction MyNodeItem::getNextAction( void )
{
	return _nextAction;
}

MyNodeTime MyNodeItem::getNextTime( void )
{
	return _nextTime;
}

bool MyNodeItem::schedule( void )
{
#if MYNODE_DEBUG
	Serial.print(F("MNI schedule: action="));
	Serial.println(_nextAction);
#endif
	if( _nextAction == MYNODE_ACTION_NONE ){
		nextAction( MYNODE_ACTION_NONE, MYNODE_TIME_MAXDUR );
		return true;
	}

	MyNodeAction action = _nextAction;
	_nextAction = MYNODE_ACTION_NONE;

	bool result = runAction( action );

	if( _nextAction == MYNODE_ACTION_NONE )
		nextAction( MYNODE_ACTION_NONE, MYNODE_TIME_MAXDUR );

	return result;
}

bool MyNodeItem::runAction( MyNodeAction action )
{
	Serial.println(F("MNI runAction: unknown action"));
	return false;
}

