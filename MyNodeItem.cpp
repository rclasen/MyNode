#include "MyNode.h"

static const char ALOC[] PROGMEM = "MyNodeItem.h";
#define ASSERT(e) myassert(PGMT(ALOC), e );

MyMessage _nodeMsg;


/************************************************************
 * MyNodeItem
 */

MyNodeItem::MyNodeItem( uint8_t sensorc )
{
	_sensorv = new MyNodeItemSensor[sensorc]();
	ASSERT( _sensorv );

	_sensorc = sensorc;
	_interval = 300L * 1000; // 5 min
	nextAction( MYNODE_ACTION_INIT );
}

#if do_deletes
MyNodeItem::~MyNodeItem()
{
	delete _sensorv;
}
#endif

void MyNodeItem::setSendInterval( MyTime interval )
{
	_interval = interval;
}

uint8_t MyNodeItem::getSensorCount( void )
{
	return _sensorc;
}

void MyNodeItem::setSensor(uint8_t snum, uint8_t id, mysensor_sensor type )
{
	ASSERT( snum < _sensorc );

	_sensorv[snum].id = id;
	_sensorv[snum].type = type;
}

uint8_t MyNodeItem::getSensorId(uint8_t snum)
{
	ASSERT( snum < _sensorc );

	return _sensorv[snum].id;
}

uint8_t MyNodeItem::getSensorById(uint8_t id)
{
	for( uint8_t c = 0; c < _sensorc; ++c ){
		if( _sensorv[c].id == MYNODE_SENSORID_NONE )
			continue;

		if( _sensorv[c].id == id )
			return c;
	}

	return MYNODE_SENSORNUM_NONE;
}

bool MyNodeItem::haveSensorId(uint8_t id)
{
	return getSensorById( id ) != MYNODE_SENSORNUM_NONE;
}

void MyNodeItem::registered( void )
{
}

void MyNodeItem::presentation( void )
{
	for( uint8_t c = 0; c < _sensorc; ++c ){
		if( _sensorv[c].id == MYNODE_SENSORID_NONE )
			continue;

		present( _sensorv[c].id, _sensorv[c].type );
		// TODO: handle failed present()
	}
}

MyNodeAction MyNodeItem::getNextAction( void )
{
	return _nextAction;
}

MyTime MyNodeItem::getNextTime( void )
{
	return _nextTime;
}

void MyNodeItem::schedule( void )
{
#if MYNODE_DEBUG
	Serial.print(F("MNI SCHED item="));
	Serial.print((unsigned int)this);
	Serial.print(F(" action="));
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

void MyNodeItem::nextAt( MyNodeAction action, MyTime time )
{
#if 0
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

