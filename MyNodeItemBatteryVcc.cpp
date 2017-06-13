#include "MyNodeItemBatteryVcc.h"
#include "MyAdc.h"

MyNodeItemBatteryVcc::MyNodeItemBatteryVcc() : MyNodeItem( 1 )
{
	_min = 2700; // arduino minimum
	_type = MyBatteryAlkaline;
	_cells = 2;
	setSendInterval( 24L * 3600 * 1000 ); // 1 day
	setSensor(0, MYNODE_SENSORID_BATTERY, S_MULTIMETER );
};


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

void MyNodeItemBatteryVcc::registered( void )
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

	float volt = .001 * mvolt;

	uint8_t level = MyBatteryCircuit( mvolt, _min, _cells, _type );

#if MYNODE_DEBUG
	Serial.print(F("MNI BatVcc volt="));
	Serial.print(volt);
	Serial.print(F(" mvolt="));
	Serial.print(mvolt);
	Serial.print(F(" level="));
	Serial.println(level);
#endif

	send(_msg_set(0, V_VOLTAGE).set(volt,2));
	sendBatteryLevel( level );
	// TODO: handle failed send
}

