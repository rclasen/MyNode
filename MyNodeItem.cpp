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

#if do_deletes
MyNodeItem::~MyNodeItem()
{
	delete _childv;
}
#endif

uint8_t MyNodeItem::getChildCount( void )
{
	return _childc;
}

void MyNodeItem::setChild(uint8_t child, uint8_t id, mysensor_sensor sensor )
{
	if( child >= _childc ){
		Serial.println(F("!MNI childc"));
		return;
	}

	_childv[child].id = id;
	_childv[child].sensor = sensor;
}

uint8_t MyNodeItem::getChildId(uint8_t child)
{
	if( child >= _childc ){
		Serial.println(F("!MNI childc"));
		return 0;
	}

	return _childv[child].id;
}

void MyNodeItem::before( void )
{
	nextAction( MYNODE_ACTION_POLLPREPARE );
}

void MyNodeItem::presentation( void )
{
	for( uint8_t c = 0; c < _childc; ++c ){
		if( _childv[c].id == MYNODE_CHILD_NONE )
			continue;

		present( _childv[c].id, _childv[c].sensor );
		// TODO: report present issues
	}
}

MyNodeAction MyNodeItem::getNextAction( void )
{
	return _nextAction;
}

MyNodeTime MyNodeItem::getNextTime( void )
{
	return _nextTime;
}

void MyNodeItem::schedule( void )
{
#if MYNODE_DEBUG
	Serial.print(F("MNI SCHED item="));
	Serial.print((unsigned int)this);
	Serial.print(F(" ation="));
	Serial.println(_nextAction);
#endif
	if( _nextAction == MYNODE_ACTION_NONE ){
		nextAction( MYNODE_ACTION_NONE, MYNODE_TIME_MAXDUR );
		return true;
	}

	MyNodeAction action = _nextAction;
	_nextAction = MYNODE_ACTION_NONE;

	runAction( action );

	if( _nextAction == MYNODE_ACTION_NONE )
		nextAction( MYNODE_ACTION_NONE, MYNODE_TIME_MAXDUR );
}

void MyNodeItem::runAction( MyNodeAction action )
{
	Serial.println(F("!MNI RUN: action"));
}

void MyNodeItem::nextAt( MyNodeAction action, MyNodeTime time )
{
#if MYNODE_DEBUG
	Serial.print(F("MNI NXT item="));
	Serial.print((unsigned int)this);
	Serial.print(F(" action="));
	Serial.print(action);
	Serial.print(F(" time="));
	Serial.println(time);
#endif
	_nextAction = action;
	_nextTime = time;
}

