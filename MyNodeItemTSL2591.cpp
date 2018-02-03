#include "MyNodeItemTSL2591.h"

static const char name[] PROGMEM = "TSL2591";

MyNodeItemTSL2591::MyNodeItemTSL2591( uint8_t id_lux, uint8_t id_vis, uint8_t id_ir,
	uint8_t avg_size ) :
	MyNodeItem( 3 ),
	alux( avg_size ),
	avis( avg_size ),
	air( avg_size )
{
	_polls = avg_size;
	_run = 0;
	_avg = avg_size;

	setSensor( 0, id_lux, S_LIGHT_LEVEL );
	setSensor( 1, id_vis, S_LIGHT_LEVEL );
	setSensor( 2, id_ir, S_LIGHT_LEVEL );
	setSendInterval( MYNODE_SECOND * 150 ); // 2.5min
};

const __FlashStringHelper *MyNodeItemTSL2591::getName( void )
{
	return PGMT(name);
}

void MyNodeItemTSL2591::setPolls( uint8_t polls )
{
	_polls = polls; // TODO: nv
}

void MyNodeItemTSL2591::setAvg( uint8_t num )
{
	_avg = num; // TODO: nv
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

	//case MYNODE_ACTION_INIT:
	default:
		actionInit();;
		return;
		;;
	}
}

void MyNodeItemTSL2591::nextActionReinit(void)
{
#ifdef MYNODE_ERROR
	Serial.println(F("!TSL2591 reinit"));
#endif
	nextAction(MYNODE_ACTION_INIT, MYNODE_SECOND * 30 );
}

void MyNodeItemTSL2591::actionInit(void)
{
	if( _sensor.setup() )
		nextAction(MYNODE_ACTION_POLLPREPARE);
	else {
		sendError(0, F("setup failed"));
		nextActionReinit();
	}
}

void MyNodeItemTSL2591::actionPollPrepare(void)
{
	if( _sensor.enable() )
		nextAction( MYNODE_ACTION_POLLRUN, _sensor.getTime() );
	else {
		nextActionReinit();
		sendError(0, F("enable failed"));
	}
}

void MyNodeItemTSL2591::actionPollRun(void)
{
	++_run;

	uint32_t raw = _sensor.getFullLuminosity();
	_sensor.disable();

	if( raw == UINT32_MAX ){
		sendError(0, F("get failed"));
		nextActionReinit();
		return;
	}

	nextAction( MYNODE_ACTION_POLLPREPARE,
		_interval / _polls - _sensor.getTime() );

	uint16_t full = raw & 0xFFFF;
	uint16_t ir = raw >> 16;
	uint16_t visible = full - ir;
	uint32_t mlux = MYNODE_LUX * _sensor.calculateLux( full, ir );

#if MYNODE_DEBUG
	Serial.print(F("TSL2591 run ")); Serial.println(_run);
	Serial.print(F(" mlux: ")); Serial.println( mlux );
	Serial.print(F(" visible: ")); Serial.println( visible );
	Serial.print(F(" ir: ")); Serial.println( ir );
#endif

	air.add( ir );
	avis.add( visible );
	alux.add( mlux );

	if( mlux >= MYNODE_LUX * TSL2591_LUX_CLIPPED ){
		sendError(0, F("clipped"));
	}

	if( _run < _polls )
		return;

	_run = 0;

	mlux = alux.calc(_avg);
	visible = avis.calc(_avg);
	ir = air.calc(_avg);

#ifdef MYNODE_DEBUG
	Serial.println(F("TSL2591 send"));
	Serial.print(F(" mlux: ")); Serial.println( mlux );
	Serial.print(F(" visible: ")); Serial.println( visible );
	Serial.print(F(" ir: ")); Serial.println( ir );
#endif

	send(_msg(0, V_LIGHT_LEVEL).set(mlux));
	send(_msg(1, V_LIGHT_LEVEL).set(MYNODE_LUX * visible));
	send(_msg(2, V_LIGHT_LEVEL).set(MYNODE_LUX * ir));

	// TODO: handle failed send
}


