#include "MyNode.h"
#include "MyAdc.h"

#include <core/MyIndication.h>
#include <core/MySensorsCore.h>

static const char ALOC[] PROGMEM = "MyNode.cpp";
#define ASSERT(e) myassert(PGMT(ALOC), e );

extern MyNodeItem *_MyNodeItems[];

static void nvDefaults( void )
{
#if MYNODE_DEBUG
	Serial.println(F("NM nvDefaults"));
#endif
	MyNodeNvSet16( MYNODE_NV_INTREF, 1100L );

	for( MyNodeItem **item = _MyNodeItems; *item; ++item ){
		(*item)->nvDefaults();
	}
}

void before( void )
{
#if MYNODE_DEBUG
	Serial.print(F("NM BEFORE, free memory: "));
	Serial.println(hwFreeMem());
#endif

	size_t nvSize = 0;
	for( MyNodeItem **item = _MyNodeItems; *item; ++item ){
		(*item)->setNvStart( MYNODE_NV_ITEMS + nvSize );
		nvSize += (*item)->getNvSize();
	}

	nvSize += MYNODE_NV_MAGIC_SIZE
		+ MYNODE_NV_INTREF_SIZE;

#if MYNODE_DEBUG
	Serial.print(F("MN nv Size="));
	Serial.println(nvSize);
#endif

	// TODO: better NV magic tag
	if( nvSize != MyNodeNvGet16( MYNODE_NV_MAGIC ) ){
		nvDefaults();
		MyNodeNvSet16( MYNODE_NV_MAGIC, nvSize );
	} else {
#if MYNODE_DEBUG
		Serial.println(F("MN nv Ok"));
#endif
	}

	if( MyNodeInit )
		MyNodeInit();

}

void setup( void )
{
#if MYNODE_DEBUG
	Serial.println(F("NM SETUP"));
#endif

	for( MyNodeItem **item = _MyNodeItems; *item; ++item ){
#if MYNODE_DEBUG
		Serial.print(F("MN SETUP "));
		Serial.print((*item)->getName());
		Serial.print(F(" item="));
		Serial.println((unsigned int)(*item));
#endif

		(*item)->setup();
	}
	Serial.println(F("NM setup done "));
}

void presentation()
{
	sendSketchInfo( SKETCH_NAME, SKETCH_VERSION );
#ifdef MYNODE_WITH_HEARTBEAT
	_MyNodeHeartbeat.activity();
#endif

	for( MyNodeItem **item = _MyNodeItems; *item; ++item )
		(*item)->presentation();
}

void loop()
{
	// run items... which might take some time
	for( MyNodeItem **item = _MyNodeItems; *item; ++item ){
		MyTime remaining = MyTimeDuration( MyTimeNow(),
			(*item)->getNextTime() );

		if( ! remaining )
			(*item)->schedule();
	}

	// now do a quick run to calculate sleep
	MyTime sleep_needed = MYNODE_TIME_MAXDUR;
	MyTime now = MyTimeNow();
	MyNodeItem *nextitem = NULL;

	for( MyNodeItem **item = _MyNodeItems; *item; ++item ){
		MyTime next = (*item)->getNextTime();
		MyTime remaining = MyTimeDuration( now, next );

#if MYNODE_DEBUG
		Serial.print(F("MN loop "));
		Serial.print((*item)->getName());
		Serial.print(F(" item="));
		Serial.print((unsigned int)(*item));
		Serial.print(F(" now="));
		Serial.print(now);
		Serial.print(F(" next="));
		Serial.print(next);
		Serial.print(F(" remaining="));
		Serial.println(remaining);
#endif
		if( remaining < sleep_needed ){
			sleep_needed = remaining;
			nextitem = (*item);
		}
	}

#if MYNODE_DEBUG
	Serial.print(F("MN loop sleep="));
	Serial.print(sleep_needed);
	Serial.print(F(" by "));
	Serial.print(nextitem->getName());
	Serial.print(F(" item="));
	Serial.println((unsigned int)nextitem);
#endif
	if( ! sleep_needed )
		return;

	if( sleep_needed <= MY_SMART_SLEEP_WAIT_DURATION_MS ){
		wait( MY_SMART_SLEEP_WAIT_DURATION_MS );
		return;
	}

	// TODO: smart sleep
	//(void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,
	//		I_PRE_SLEEP_NOTIFICATION).set((uint32_t)MY_SMART_SLEEP_WAIT_DURATION_MS));
	wait( MY_SMART_SLEEP_WAIT_DURATION_MS );
	sleep_needed -= MY_SMART_SLEEP_WAIT_DURATION_MS;

	// TODO: use multiple sleep()s for large sleep_neede to reduce
	// impact on time guestimation when an interrupt is received.

	int8_t ret = sleep( sleep_needed );
	if( ret == MY_WAKE_UP_BY_TIMER ){
		MyTimeFixup( sleep_needed );

	} else if( ret == MY_SLEEP_NOT_POSSIBLE ){
		// ok, we did something else but sleeping to cause the delay

	} else {
#if MYNODE_DEBUG
		Serial.print(F("MN loop int="));
		Serial.println(ret);
#endif
		// TODO: guess actual sleep duration
		// TODO: handle received interrupts
	}
}

void receive(const MyMessage & msg)
{
	for( MyNodeItem **item = _MyNodeItems; *item; ++item ){
		uint8_t snum = (*item)->getSensorById( msg.sensor );
		if( snum != MYNODE_SENSORNUM_NONE ){
#if MYNODE_DEBUG
			Serial.print(F("MN RCV "));
			Serial.print((*item)->getName());
			Serial.print(F(" item="));
			Serial.println((unsigned int)(*item));
#endif
			(*item)->receive( snum, msg );
			break;
		}
	}
}

void receiveTime( unsigned long ts )
{
#if MYNODE_DEBUG
	Serial.print(F("MN receiveTime: "));
	Serial.println(ts);
#endif
	// TODO: receiveTime()
}

/************************************************************
 * MyAdc - optional
 */

bool adc_init = false;

void MyNodeEnableAdc( void )
{
	if( adc_init )
		return;

	uint16_t intref = MyNodeNvGet16( MYNODE_NV_INTREF );
#if MYNODE_DEBUG
	Serial.print(F("NM intref: ")); Serial.println(intref);
#endif
	MyAdcIntrefSet( intref );
	adc_init = true;
}

/************************************************************
 * catch and send assertions
 */

void __myassert(const __FlashStringHelper *__location,
		int __lineno,
		const __FlashStringHelper *__sexp)
{
#ifdef MYNODE_DEBUG
	// transmit diagnostic informations through serial link.
	Serial.print(F("assertion "));
	Serial.println(__location);
	Serial.print(F("line "));
	Serial.println(__lineno, DEC);
	Serial.println(__sexp);
	Serial.flush();
#endif
	// TODO: send error message to GW
	MyNodePanic();
}

void MyNodePanic( void )
{
	setIndication(INDICATION_ERR_HW_INIT);
#ifdef MYNODE_DEBUG
	Serial.println(F("panic"));
#endif
	while(1)
		doYield();
}
