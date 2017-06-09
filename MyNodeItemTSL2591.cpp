#include "MyNodeItemTSL2591.h"

MyNodeItemTSL2591::MyNodeItemTSL2591( uint8_t id_lux, uint8_t id_vis, uint8_t id_ir,
	uint8_t avg_size ) :
	MyNodeItem( 3 ),
	alux( avg_size ),
	avis( avg_size ),
	air( avg_size )
{
	_polls = 3;
	_run = 0;
	setSensor( 0, id_lux, S_LIGHT_LEVEL );
	setSensor( 1, id_vis, S_LIGHT_LEVEL );
	setSensor( 2, id_ir, S_LIGHT_LEVEL );
	setSendInterval( 150L * 1000 ); // 2.5min
};

void MyNodeItemTSL2591::setPolls( uint8_t polls )
{
	_polls = polls;
}

void MyNodeItemTSL2591::setAvg( uint8_t num )
{
	_avg = num;
}

void MyNodeItemTSL2591::setGain( tsl2591Gain_t gain )
{
	_sensor.setGain( gain );
}

void MyNodeItemTSL2591::setIntegration( tsl2591IntegrationTime_t integration )
{
	_sensor.setIntegration( integration );
}

void MyNodeItemTSL2591::runAction( MyNodeAction action )
{
	switch(action){
	case MYNODE_ACTION_POLLRUN:
		actionPollRun();
		return;
		;;

	case MYNODE_ACTION_POLLPREPARE:
		actionPollPrepare();;
		return;
		;;

	case MYNODE_ACTION_INIT:
		actionInit();;
		return;
		;;
	}

	send(_msg_set(0, V_CUSTOM).set(F("assert: action")));
#ifdef MYNODE_ERROR
	Serial.print(F("!TSL2591 action="));
	Serial.println(action);
#endif
}

void MyNodeItemTSL2591::nextActionReinit(void)
{
#ifdef MYNODE_ERROR
	Serial.println(F("!TSL2591 reinit"));
#endif
	nextAction(MYNODE_ACTION_INIT, 30000 );
	send(_msg_set(0, V_CUSTOM).set(F("reinit")));
}

void MyNodeItemTSL2591::actionInit(void)
{
	if( _sensor.setup() )
		nextAction(MYNODE_ACTION_POLLPREPARE);
	else
		nextActionReinit();
}

void MyNodeItemTSL2591::actionPollPrepare(void)
{
	if( _sensor.enable() )
		nextAction( MYNODE_ACTION_POLLRUN, _sensor.getTime() );
	else
		nextActionReinit();
}

void MyNodeItemTSL2591::actionPollRun(void)
{
	++_run;

	uint32_t raw = _sensor.getFullLuminosity();
	_sensor.disable();

	if( raw == UINT32_MAX ){
#ifdef MYNODE_ERROR
		Serial.println(F("!TSL2591 get"));
#endif
		nextActionReinit();
		return;
	}

	nextAction( MYNODE_ACTION_POLLPREPARE, _interval / _polls );

	uint16_t full = raw & 0xFFFF;
	uint16_t ir = raw >> 16;
	uint16_t visible = full - ir;
	float lux = _sensor.calculateLux( full, ir );

#if MYNODE_DEBUG
	Serial.print(F("TSL2591 run ")); Serial.println(_run);
	Serial.print(F(" lux: ")); Serial.println( lux );
	Serial.print(F(" visible: ")); Serial.println( visible );
	Serial.print(F(" ir: ")); Serial.println( ir );
#endif

	air.add( ir );
	avis.add( visible );
	alux.add( lux );

	if( lux >= TSL2591_LUX_CLIPPED ){
		send(_msg_set(0, V_CUSTOM).set(F("clipped")));
#ifdef MYNODE_ERROR
		Serial.println(F("!TSL2591 lux"));
#endif
	}

	if( _run < _polls )
		return;

#ifdef MYNODE_DEBUG
	Serial.println(F("TSL2591 send"));
#endif
	_run = 0;

	send(_msg_set(0, V_LIGHT_LEVEL).set(alux.calc(_avg),3));
	send(_msg_set(1, V_LIGHT_LEVEL).set(avis.calc(_avg)));
	send(_msg_set(2, V_LIGHT_LEVEL).set(air.calc(_avg)));

	// TODO: handle failed send
}


