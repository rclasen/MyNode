#include "MyNodeItemVolt.h"

static const char name[] PROGMEM = "Volt";

MyNodeItemVolt::MyNodeItemVolt( uint8_t id,
		uint8_t analog_pin, uint8_t vcc_pin
		) : MyNodeItem( 1 )
{
	_analog = analog_pin;
	_vcc = vcc_pin;
	_mfactor = MYNODE_VOLT;
	setSensor(0, id, S_MULTIMETER );
};

const __FlashStringHelper *MyNodeItemVolt::getName( void )
{
	return PGMT(name);
}

void MyNodeItemVolt::setFactor( uint32_t mfactor )
{
	_mfactor = mfactor;
}

void MyNodeItemVolt::setup( void )
{
	MyNodeEnableAdc();

	if( _vcc != MYNODE_PIN_NONE ){
		pinMode( _vcc, OUTPUT );
		powerOff();
	}

	nextActionPoll();
}

void MyNodeItemVolt::runAction( MyNodeAction action )
{
	switch(action){
	case MYNODE_ACTION_POLLRUN:
		actionPollRun();
		return;
		;;

	//case MYNODE_ACTION_INIT:
	//case MYNODE_ACTION_POLLPREPARE:
	default:
		actionPollPrepare();;
		return;
		;;
	}
}

void MyNodeItemVolt::actionPollPrepare( void )
{
	if( _vcc != MYNODE_PIN_NONE ){
		powerOn();
		nextAction( MYNODE_ACTION_POLLRUN, 50 );

	} else {
		nextAction( MYNODE_ACTION_POLLRUN );
	}
}

void MyNodeItemVolt::actionPollRun( void )
{
	nextActionPoll( _interval );

	uint16_t raw = MyAdcReadInt( _analog );
	powerOff();

	_mvolt = _mfactor * raw / MYNODE_VOLT;

#if MYNODE_DEBUG
	Serial.print(F("MNI Volt mvolt="));
	Serial.println(_mvolt);
#endif

	send(_msg(0, V_VOLTAGE).set(_mvolt,2));
	// TODO: handle failed send
}

void MyNodeItemVolt::powerOn( void )
{
	if( _vcc != MYNODE_PIN_NONE )
		digitalWrite( _vcc, HIGH );
}

void MyNodeItemVolt::powerOff( void )
{
	if( _vcc != MYNODE_PIN_NONE )
		digitalWrite( _vcc, LOW );
}
