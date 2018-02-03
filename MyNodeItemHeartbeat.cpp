#include "MyNodeItemHeartbeat.h"
#include "MyAdc.h"

MyNodeItemHeartbeat _MyNodeHeartbeat;

static const char name[] PROGMEM = "Heartbeat";

MyNodeItemHeartbeat::MyNodeItemHeartbeat() : MyNodeItem( 0 )
{
	setSendInterval( MYNODE_DAY );
};

const __FlashStringHelper *MyNodeItemHeartbeat::getName( void )
{
	return PGMT(name);
}

void MyNodeItemHeartbeat::setup( void )
{
	activity();
}

void MyNodeItemHeartbeat::activity( void )
{
	nextAction( MYNODE_ACTION_POLLRUN, _interval );
}

void MyNodeItemHeartbeat::runAction( MyNodeAction action )
{
	switch(action){
	case MYNODE_ACTION_POLLRUN:
		actionPollRun();
		return;
		;;
	}

#ifdef MYNODE_ERROR
	Serial.print(F("!MNI Heartbeat action="));
	Serial.println(action);
#endif
}

void MyNodeItemHeartbeat::actionPollRun( void )
{
	sendHeartbeat();
	activity();
	// TODO: handle failed send
}

