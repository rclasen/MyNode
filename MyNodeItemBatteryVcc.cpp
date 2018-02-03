#include "MyNodeItemBatteryVcc.h"
#include "MyAdc.h"

static const char name[] PROGMEM = "BatteryVcc";

MyNodeItemBatteryVcc::MyNodeItemBatteryVcc() : MyNodeItem( 1 )
{
	_min = 2700; // arduino minimum
	_type = MyBatteryAlkaline;
	_cells = 2;
	setSendInterval( MYNODE_DAY );
	setSensor(0, MYNODE_SENSORID_BATTERY, S_MULTIMETER );
};


const __FlashStringHelper *MyNodeItemBatteryVcc::getName( void )
{
	return PGMT(name);
}

void MyNodeItemBatteryVcc::setCircuitMin( uint16_t min )
{
	_min = min;
}

void MyNodeItemBatteryVcc::setBatteryType( MyBatteryType type )
{
	_type = type;
}

void MyNodeItemBatteryVcc::setBatteryCells( uint8_t cells )
{
	_cells = cells;
}

void MyNodeItemBatteryVcc::setup( void )
{
	MyNodeEnableAdc();

	nextAction( MYNODE_ACTION_POLLRUN );
}

void MyNodeItemBatteryVcc::runAction( MyNodeAction action )
{
	switch(action){
	case MYNODE_ACTION_POLLRUN:
		actionPollRun();
		return;
		;;
	}

#ifdef MYNODE_ERROR
	Serial.print(F("!MNI BatVcc action="));
	Serial.println(action);
#endif
}

void MyNodeItemBatteryVcc::actionPollRun( void )
{
	nextAction( MYNODE_ACTION_POLLRUN, _interval );

	uint16_t mvolt = MyAdcVcc();

	uint8_t level = MyBatteryCircuit( mvolt, _min, _cells, _type );

#if MYNODE_DEBUG
	Serial.print(F("MNI BatVcc mvolt="));
	Serial.print(mvolt);
	Serial.print(F(" level="));
	Serial.println(level);
#endif

	send(_msg(0, V_VOLTAGE).set(mvolt,2));
	sendBatteryLevel( level );
	// TODO: handle failed send
}

