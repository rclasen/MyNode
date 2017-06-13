#include "MyNodeItemVolt.h"
#include "MyAdc.h"

MyNodeItemVolt::MyNodeItemVolt( uint8_t id,
		uint8_t analog_pin, uint8_t vcc_pin
		) : MyNodeItem( 1 )
{
	_analog = analog_pin;
	_vcc = vcc_pin;
	_wait = 50;
	setSensor(0, id, S_MULTIMETER );
};

void MyNodeItemVolt::registered( void )
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

	case MYNODE_ACTION_INIT:
	case MYNODE_ACTION_POLLPREPARE:
		actionPollPrepare();;
		return;
		;;
	}

#ifdef MYNODE_ERROR
	Serial.print(F("!MNI Volt action="));
	Serial.println(action);
#endif
}

void MyNodeItemVolt::actionPollPrepare( void )
{
	if( _vcc != MYNODE_PIN_NONE ){
		powerOn();
		nextAction( MYNODE_ACTION_POLLRUN, _wait );

	} else {
		nextAction( MYNODE_ACTION_POLLRUN );
	}
}

void MyNodeItemVolt::actionPollRun( void )
{
	nextActionPoll( _interval );

	uint16_t mvolt = MyAdcReadVcc( _analog );
	powerOff();

	float volt = .001 * mvolt;

#if MYNODE_DEBUG
	Serial.print(F("MNI Volt volt="));
	Serial.println(volt);
#endif

	send(_msg_set(0, V_VOLTAGE).set(volt,2));
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
