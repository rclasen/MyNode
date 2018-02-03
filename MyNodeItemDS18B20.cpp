#include "MyNodeItemDS18B20.h"

static const char name[] PROGMEM = "DS18B20";

MyNodeItemDS18B20::MyNodeItemDS18B20( uint8_t id, OneWire *bus,
	uint8_t avg_size ) :
	MyNodeItem( 1 ),
	_sensors( bus ),
	atemp( avg_size )
{
	_polls = avg_size;
	_run = 0;
	_avg = avg_size;
	// TODO: memset(_address)?

	_sensors.setWaitForConversion(false);

	setSensor( 0, id, S_TEMP );
	setSendInterval( MYNODE_SECOND * 150 ); // 2.5min
};

const __FlashStringHelper *MyNodeItemDS18B20::getName( void )
{
	return PGMT(name);
}

void MyNodeItemDS18B20::setPolls( uint8_t polls )
{
	_polls = polls; // TODO: nv
}

void MyNodeItemDS18B20::setAvg( uint8_t num )
{
	_avg = num; // TODO: nv
}

void MyNodeItemDS18B20::runAction( MyNodeAction action )
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

void MyNodeItemDS18B20::nextActionReinit(void)
{
#ifdef MYNODE_ERROR
	Serial.println(F("!DS18B20 reinit"));
#endif
	nextAction(MYNODE_ACTION_INIT, MYNODE_SECOND * 30 );
}

bool MyNodeItemDS18B20::_sensorSetup(void)
{
	_sensors.begin();
	if( _sensors.getDS18Count() != 1
		|| _sensors.getDeviceCount() != 1 ){

		sendError(0, F("sensor count"));
		return false;
	}

	if( ! _sensors.getAddress( _address, 0 ) ){
		sendError(0, F("address failed"));
		return false;
	}

// TODO: send _address
/* TODO: need sensors supporting resolution:
	if( ! _sensors.setResolution( _address, _resolution ))
		return false;
*/

	return true;
}

void MyNodeItemDS18B20::actionInit(void)
{
	if( _sensorSetup() )
		nextAction(MYNODE_ACTION_POLLPREPARE);
	else
		nextActionReinit();
}

void MyNodeItemDS18B20::actionPollPrepare(void)
{
	_resolution = _sensors.getResolution(_address);
	if( _resolution == 0 ){
		sendError(0, F("vanished"));
		nextActionReinit();

	} else if( ! _sensors.requestTemperaturesByAddress( _address )){
		sendError(0, F("request failed"));
		nextActionReinit();

	} else
		nextAction( MYNODE_ACTION_POLLRUN,
			_sensors.millisToWaitForConversion(_resolution) );
}

void MyNodeItemDS18B20::actionPollRun(void)
{
	++_run;

	uint16_t raw = _sensors.getTemp( _address );

	if( raw == DEVICE_DISCONNECTED_RAW ){
		sendError(0, F("get failed"));
		nextActionReinit();
		return;
	}

	nextAction( MYNODE_ACTION_POLLPREPARE,
		_interval / _polls - _sensors.millisToWaitForConversion(_resolution) );

	uint16_t dtemp = MYNODE_CELSIUS * raw / 128;
	atemp.add( dtemp );

#if MYNODE_DEBUG
	Serial.print(F("DS18B20 got ")); Serial.println(raw);
	Serial.print(F(" dtemp ")); Serial.println(dtemp);
#endif

	if( _run < _polls )
		return;

	_run = 0;

	dtemp = atemp.calc(_avg);

#if MYNODE_DEBUG
	Serial.print(F("DS18B20 send ")); Serial.println(raw);
	Serial.print(F(" dtemp ")); Serial.println(dtemp);
#endif

	send(_msg(0, V_TEMP).set(dtemp));

	// TODO: handle failed send
}


