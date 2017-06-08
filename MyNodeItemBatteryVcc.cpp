#include "MyNodeItemBatteryVcc.h"
#include "MyNodeAdc.h"

MyNodeItemBatteryVcc::MyNodeItemBatteryVcc(
		uint16_t min, uint16_t max,
		MyNodeTime interval ) : MyNodeItem( 1 )
{
	_min = min;
	_max = max;
	_interval = interval;
	setSensor(0, MYNODE_SENSORID_BATTERY, S_MULTIMETER );
};


void MyNodeItemBatteryVcc::before( void )
{
	MyNodeEnableAdc();

	nextAction( MYNODE_ACTION_POLLRUN );
}

void MyNodeItemBatteryVcc::runAction( MyNodeAction action )
{
	switch(action){
	case MYNODE_ACTION_POLLRUN:
		actionPollRun();
		return;
		;;
	}

#ifdef MYNODE_ERROR
	Serial.print(F("!MNI BatVcc action="));
	Serial.println(action);
#endif
}

void MyNodeItemBatteryVcc::actionPollRun( void )
{
	nextAction( MYNODE_ACTION_POLLRUN, _interval );

	uint16_t mvolt = MyNodeAdcVcc();

	float volt = .001 * mvolt;

	uint8_t level;
	if( mvolt <= _min ){
		level = 0;

	} else if( mvolt > _max ){
		level = 100;

	} else {
		level = 100.0 * (mvolt - _min) / (_max - _min);;
	}


#if MYNODE_DEBUG
	Serial.print(F("MNI BatVcc volt="));
	Serial.print(volt);
	Serial.print(F(" mvolt="));
	Serial.print(mvolt);
	Serial.print(F(" level="));
	Serial.println(level);
#endif

	send(_msg_set(0, V_VOLTAGE).set(volt,2));
	sendBatteryLevel( level );
	// TODO: handle failed send
}

