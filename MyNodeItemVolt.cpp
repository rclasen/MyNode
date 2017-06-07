#include "MyNodeItemVolt.h"
#include "MyNodeAdc.h"

MyNodeItemVolt::MyNodeItemVolt( uint8_t child,
		uint8_t analog_pin, uint8_t vcc_pin,
		MyNodeTime sleep, MyNodeTime wait ) : MyNodeItem( 1 )
{
	_analog = analog_pin;
	_vcc = vcc_pin;
	_wait = wait;
	_sleep = sleep;
	setChild(0, child, S_MULTIMETER );
};

void MyNodeItemVolt::before( void )
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

	Serial.print(F("!MNI Volt action="));
	Serial.println(action);
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
	nextActionPoll( _sleep );

	uint16_t mvolt = MyNodeAdcRead( _analog );
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
