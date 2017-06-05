#include "MyNodeItemTSL2591.h"

MyNodeItemTSL2591::MyNodeItemTSL2591( uint8_t lux, uint8_t visible,
		uint8_t ir, MyNodeTime sleep  ) :
	MyNodeItem( 3 ),
	alux( 5 ),
	avis( 5 ),
	air( 5 )
{
	_sleep = sleep;
	setChild( 0, lux, S_LIGHT_LEVEL );
	setChild( 1, visible, S_LIGHT_LEVEL );
	setChild( 2, ir, S_LIGHT_LEVEL );
	nextSend = MyNodeNow();
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
	MyNodeTime now = MyNodeNow();

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
	Serial.print(F(" lux: ")); Serial.println( lux );
	Serial.print(F(" visible: ")); Serial.println( visible );
	Serial.print(F(" ir: ")); Serial.println( ir );
#endif

	air.add( ir );
	avis.add( visible );
	alux.add( lux );

	if( lux >= TSL2591_LUX_CLIPPED ){
		Serial.println(F("!TSL2591 lux"));
	}

	if( nextSend - now < MYNODE_TIME_MAXDUR )
		return;

	Serial.println(F("TSL2591 send"));
	nextSend = now + 5 * _sleep;

	send(_msg_set(0, V_LIGHT_LEVEL).set(alux.calc()));
	send(_msg_set(1, V_LIGHT_LEVEL).set(avis.calc()));
	send(_msg_set(2, V_LIGHT_LEVEL).set(air.calc()));

	// TODO: handle failed send
}


