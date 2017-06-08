#include "MyNodeItemTSL2591.h"

MyNodeItemTSL2591::MyNodeItemTSL2591( uint8_t id_lux, uint8_t id_vis,
		uint8_t id_ir, MyNodeTime interval  ) :
	MyNodeItem( 3 ),
	alux( 5 ),
	avis( 5 ),
	air( 5 )
{
	_interval = interval;
	_polls = 3;
	_run = 0;
	setSensor( 0, id_lux, S_LIGHT_LEVEL );
	setSensor( 1, id_vis, S_LIGHT_LEVEL );
	setSensor( 2, id_ir, S_LIGHT_LEVEL );
};

void MyNodeItemTSL2591::before(void)
{
	nextAction( MYNODE_ACTION_POLLPREPARE );

	_sensor.begin(); // TODO: handle error, re-init?
	_sensor.setGain(TSL2591_GAIN_LOW);
	//_sensor.setGain(TSL2591_GAIN_HIGH);
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_100MS);
	//... 200, 300, 400, 500, ...
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_600MS);
	_sensor.setTiming(TSL2591_INTEGRATIONTIME_200MS);
	_sensor.disable();
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
	_sensor.enable();
	nextAction( MYNODE_ACTION_POLLRUN, _sensor.getTime() );
}

void MyNodeItemTSL2591::actionPollRun(void)
{
	++_run;
	nextAction( MYNODE_ACTION_POLLPREPARE, _interval / _polls );
	MyNodeTime now = MyNodeNow();

	uint32_t raw = _sensor.getFullLuminosity();
	_sensor.disable();

	if( raw == UINT32_MAX ){
		Serial.println(F("!TSL2591 get"));
		return;
	}

	uint16_t full = raw & 0xFFFF;
	uint16_t ir = raw >> 16;
	uint16_t visible = full - ir;
	float lux = _sensor.calculateLux( full, ir );

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

	if( _run < _polls )
		return;

	Serial.println(F("TSL2591 send"));
	_run = 0;

	send(_msg_set(0, V_LIGHT_LEVEL).set(alux.calc(),3));
	send(_msg_set(1, V_LIGHT_LEVEL).set(avis.calc()));
	send(_msg_set(2, V_LIGHT_LEVEL).set(air.calc()));

	// TODO: handle failed send
}


