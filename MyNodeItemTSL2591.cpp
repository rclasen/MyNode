#include "MyNodeItemTSL2591.h"

MyNodeItemTSL2591::MyNodeItemTSL2591( uint8_t lux, uint8_t visible,
		uint8_t ir, MyNodeTime sleep  ) :
	MyNodeItem( 3 )
{
	_sleep = sleep;
	setChild( 0, lux, S_LIGHT_LEVEL );
	setChild( 1, visible, S_LIGHT_LEVEL );
	setChild( 2, ir, S_LIGHT_LEVEL );
};

void MyNodeItemTSL2591::before(void)
{
	nextAction( MYNODE_ACTION_POLLRUN ); // TODO: actionPollPrepare

	_sensor.begin();
	_sensor.setGain(TSL2591_GAIN_LOW);
	//_sensor.setGain(TSL2591_GAIN_HIGH);
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_100MS);
	//... 200, 300, 400, 500, ...
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_600MS);
	_sensor.setTiming(TSL2591_INTEGRATIONTIME_200MS);
};

void MyNodeItemTSL2591::runAction( MyNodeAction action )
{
	switch(action){
	case MYNODE_ACTION_POLLRUN:
		actionPollRun();
		return;
		;;

	case MYNODE_ACTION_INIT:
	case MYNODE_ACTION_POLLPREPARE:
		actionPollPrepare();;
		return;
		;;
	}

	Serial.print(F("!TSL2591 action="));
	Serial.println(action);
}

void MyNodeItemTSL2591::actionPollPrepare(void)
{
	nextAction( MYNODE_ACTION_POLLRUN ); // TODO: actionPollPrepare
}

void MyNodeItemTSL2591::actionPollRun(void)
{
	nextAction( MYNODE_ACTION_POLLRUN, _sleep ); // TODO: actionPollPrepare

	uint32_t raw = _sensor.getFullLuminosity();
	if( raw == UINT32_MAX ){
		Serial.println(F("!TSL2591 get"));
		return;
	}

	uint16_t full = raw & 0xFFFF;
	uint16_t ir = raw >> 16;
	uint16_t visible = full - ir;
	uint32_t lux = _sensor.calculateLux( full, ir );

#if MYNODE_DEBUG
	Serial.println(F("TSL2591"));
	Serial.print(F(" raw: ")); Serial.println( raw );
	Serial.print(F(" full: ")); Serial.println( full );
	Serial.print(F(" ir: ")); Serial.println( ir );
	Serial.print(F(" visible: ")); Serial.println( visible );
	Serial.print(F(" lux: ")); Serial.println( lux );
#endif

	if( lux >= TSL2591_LUX_CLIPPED ){
		Serial.println(F("!TSL2591 lux"));
		lux=TSL2591_LUX_CLIPPED; // maxes out at 88 kLux
	}

	send(_msg_set(0, V_LIGHT_LEVEL).set(lux));
	send(_msg_set(1, V_LIGHT_LEVEL).set(visible));
	send(_msg_set(2, V_LIGHT_LEVEL).set(ir));
	// TODO: handle failed send
}


