#include "MyNodeItemAnalog.h"

#define ADC_MAX 1023

MyNodeItemAnalog::MyNodeItemAnalog( uint8_t analog_pin, uint8_t vcc_pin,
		MyNodeTime wait ) : MyNodeItem( 1 )
{
	_analog = analog_pin;
	_vcc = vcc_pin;
	_wait = wait;
};


void MyNodeItemAnalog::before( void )
{
	if( _vcc != MYNODE_PIN_NONE ){
		pinMode( _vcc, OUTPUT );
		powerOff();
	}

	nextActionPoll();
}

void MyNodeItemAnalog::runAction( MyNodeAction action )
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

	Serial.print(F("!MNI Analog action="));
	Serial.println(action);
}

void MyNodeItemAnalog::actionPollPrepare( void )
{
	if( _vcc != MYNODE_PIN_NONE ){
		powerOn();
		nextAction( MYNODE_ACTION_POLLRUN, _wait );

	} else {
		nextAction( MYNODE_ACTION_POLLRUN );
	}
}

void MyNodeItemAnalog::powerOn( void )
{
	if( _vcc != MYNODE_PIN_NONE )
		digitalWrite( _vcc, HIGH );
}

void MyNodeItemAnalog::powerOff( void )
{
	if( _vcc != MYNODE_PIN_NONE )
		digitalWrite( _vcc, LOW );
}

uint16_t MyNodeItemAnalog::getMVolt( void )
{
	long vcc = MyNodeVcc();

	analogReference(DEFAULT);
	uint16_t raw = analogRead(_analog);

	return vcc * raw / ADC_MAX;
}

