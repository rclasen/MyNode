#include "MyNodeItemTSL2591.h"

MyNodeItemTSL2591::MyNodeItemTSL2591( uint8_t lux, uint8_t visible,
		uint8_t ir, MyNodeTime sleep  ) :
	MyNodeItem( 3 )
{
	_sleep = sleep;
	setChild( 0, lux, S_LIGHT_LEVEL );
	setChild( 1, visible, S_LIGHT_LEVEL );
	setChild( 2, ir, S_LIGHT_LEVEL );
	nextAction( MYNODE_ACTION_POLLRUN ); // TODO: actionPollPrepare
};

bool MyNodeItemTSL2591::before(void)
{
	_sensor.begin();
	_sensor.setGain(TSL2591_GAIN_LOW);
	//_sensor.setGain(TSL2591_GAIN_HIGH);
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_100MS);
	//... 200, 300, 400, 500, ...
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_600MS);
	_sensor.setTiming(TSL2591_INTEGRATIONTIME_200MS);
};

bool MyNodeItemTSL2591::runAction( MyNodeAction action )
{
	switch(action){
	case MYNODE_ACTION_POLLRUN:
		return actionPollRun();
		break;
		;;

	case MYNODE_ACTION_INIT:
	case MYNODE_ACTION_POLLPREPARE:
		return actionPollPrepare();;
		break;
		;;
	}

	Serial.print(F("MNI TSL2591 bad action="));
	Serial.println(action);
	return false;
}

bool MyNodeItemTSL2591::actionPollPrepare(void)
{
#if MYNODE_DEBUG
	Serial.println(F("MNI TSL2591 actionPollPrepare"));
#endif
	nextAction( MYNODE_ACTION_POLLRUN ); // TODO: actionPollPrepare
	return true;
}

bool MyNodeItemTSL2591::actionPollRun(void)
{
#if MYNODE_DEBUG
	Serial.println(F("MNI TSL2591 actionPollRun"));
#endif
	nextAction( MYNODE_ACTION_POLLRUN, _sleep ); // TODO: actionPollPrepare

	uint32_t raw = _sensor.getFullLuminosity();
	if( raw == UINT32_MAX ){
		Serial.println(F("TSL2591 FAILED to get raw data"));
		return false;
	}

	uint16_t full = raw & 0xFFFF;
	uint16_t ir = raw >> 16;
	uint16_t visible = full - ir;
	uint32_t lux = _sensor.calculateLux( full, ir );

#if MYNODE_DEBUG
	Serial.print(F(" raw: ")); Serial.println( raw );
	Serial.print(F(" full: ")); Serial.println( full );
	Serial.print(F(" ir: ")); Serial.println( ir );
	Serial.print(F(" visible: ")); Serial.println( visible );
	Serial.print(F(" lux: ")); Serial.println( lux );
#endif

	if( lux >= TSL2591_LUX_CLIPPED ){
		Serial.println(F(" FAILED to get lux"));
		lux=TSL2591_LUX_CLIPPED; // maxes out at 88 kLux
	}

	bool result = true;
	result &= send(_msg_set(0, V_LIGHT_LEVEL).set(lux));
	result &= send(_msg_set(1, V_LIGHT_LEVEL).set(visible));
	result &= send(_msg_set(2, V_LIGHT_LEVEL).set(ir));

	return result;
}


