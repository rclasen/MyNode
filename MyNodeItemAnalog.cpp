#include "MyNodeItemAnalog.h"

#define ADC_MAX 1023

MyNodeItemAnalog::MyNodeItemAnalog( uint8_t analog_pin, uint8_t vcc_pin,
		MyNodeTime wait ) : MyNodeItem( 1 )
{
	_analog = analog_pin;
	_vcc = vcc_pin;
	_wait = wait;

	nextPoll();
};


bool MyNodeItemAnalog::before( void )
{
	if( _vcc != MYNODE_PIN_NONE ){
		pinMode( _vcc, OUTPUT );
		powerOff();
	}

	return true;
}

bool MyNodeItemAnalog::runAction( MyNodeAction action )
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

	Serial.print(F("MNI Analog bad action="));
	Serial.println(action);
	return false;
}

bool MyNodeItemAnalog::actionPollPrepare( void )
{
#if MYNODE_DEBUG
	Serial.println(F("MNI Ananlog actionPollPrepare"));
#endif
	if( _vcc != MYNODE_PIN_NONE ){
		powerOn();
		nextAction( MYNODE_ACTION_POLLRUN, _wait );

	} else {
		nextAction( MYNODE_ACTION_POLLRUN );
	}

	return true;
}

bool MyNodeItemAnalog::powerOn( void )
{
	if( _vcc != MYNODE_PIN_NONE )
		digitalWrite( _vcc, HIGH );

	return true;
}

bool MyNodeItemAnalog::powerOff( void )
{
	if( _vcc != MYNODE_PIN_NONE )
		digitalWrite( _vcc, LOW );

	return true;
}

uint16_t MyNodeItemAnalog::getMVolt( void )
{
	long vcc = MyNodeVcc();

	analogReference(DEFAULT);
	uint16_t raw = analogRead(_analog);

	return vcc * raw / ADC_MAX;
}

