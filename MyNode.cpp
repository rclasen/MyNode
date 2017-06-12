#include "MyNode.h"
#include "MyAdc.h"

#include <core/MyIndication.h>
#include <core/MySensorsCore.h>

static const char ALOC[] PROGMEM = "MyNode.cpp";
#define ASSERT(e) myassert(PGMT(ALOC), e );

#define MYNODE_ITEM_NONE 255

uint8_t _itemc = 0;		// item count
uint8_t _itemn = 0;		// next item index
MyNodeItem **_itemv = NULL;	// item array

uint16_t hwFreeMem();		// from MySensors/hal/architecture/MyHw*.cpp

/************************************************************
 * MyNode
 */

void MyNodeInit( uint8_t itemc )
{
#if MYNODE_DEBUG
	Serial.print(F("NM free memory: "));
	Serial.println(hwFreeMem());
#endif

	_itemv = new MyNodeItem*[itemc](NULL);
	ASSERT( _itemv );

	_itemc = itemc;
	_itemn = 0;
}

#if do_deletes
void MyNodeEnd()
{
	// maybe?
	for( uint8_t i = 0; i < _itemn; ++i )
		delete _itemv[i];
	delete _itemv;
}
#endif

void MyNodeRegisterItem( MyNodeItem *item )
{
	ASSERT(  item );
	ASSERT(  _itemn < _itemc );

	_itemv[_itemn] = item;

	uint8_t cnt = item->getSensorCount();
#if MYNODE_DEBUG
	Serial.print(F("MN REG i="));
	Serial.print(_itemn);
	Serial.print(F(" item="));
	Serial.print((unsigned int)item);
	Serial.print(F(" cnt="));
	Serial.println(cnt);
#endif
	++_itemn;

	item->registered();
}

void MyNodePresentation( const __FlashStringHelper *name, const __FlashStringHelper *version )
{
	sendSketchInfo( name, version );

	for( uint8_t i = 0; i < _itemn; ++i )
		_itemv[i]->presentation();
}

void MyNodeLoop()
{
	// run items... which might take some time
	for( uint8_t i = 0; i < _itemn; ++i ){
		MyTime remaining = MyTimeDuration( MyTimeNow(),
			_itemv[i]->getNextTime() );

		if( ! remaining )
			_itemv[i]->schedule();
	}

	// now do a quick run to calculate sleep
	MyTime sleep_needed = MYNODE_TIME_MAXDUR;
	MyTime now = MyTimeNow();
	uint8_t nextitem = MYNODE_ITEM_NONE;

	for( uint8_t i = 0; i < _itemn; ++i ){
		MyTime next = _itemv[i]->getNextTime();
		MyTime remaining = MyTimeDuration( now, next );

#if MYNODE_DEBUG
		Serial.print(F("MN loop i="));
		Serial.print(i);
		Serial.print(F(" item="));
		Serial.print((unsigned int)_itemv[i]);
		Serial.print(F(" now="));
		Serial.print(now);
		Serial.print(F(" next="));
		Serial.print(next);
		Serial.print(F(" remaining="));
		Serial.println(remaining);
#endif
		if( remaining < sleep_needed ){
			sleep_needed = remaining;
			nextitem = i;
		}
	}

#if MYNODE_DEBUG
	Serial.print(F("MN loop sleep="));
	Serial.print(sleep_needed);
	Serial.print(F(" by="));
	Serial.println(nextitem);
#endif
	if( ! sleep_needed )
		return;

	// TODO: use multiple sleep()s for large sleep_neede to reduce
	// impact on time guestimation when an interrupt is received.

	int8_t ret = sleep( sleep_needed );
	if( ret == MY_WAKE_UP_BY_TIMER ){
		MyTimeFixup( sleep_needed );

	} else if( ret == MY_SLEEP_NOT_POSSIBLE ){
		// TODO: MY_SLEEP_NOT_POSSIBLE

	} else {
#if MYNODE_DEBUG
		Serial.print(F("MN loop int="));
		Serial.println(ret);
#endif
		// TODO: guess actual sleep duration
		// TODO: handle received interrupts
	}
}

void MyNodeReceive(const MyMessage & msg)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receive"));
#endif
	// TODO: receive()
}

void MyNodeReceiveTime(unsigned long ts)
{
#if MYNODE_DEBUG
	Serial.println(F("MN receiveTime"));
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

	// TODO: get Adc Intref calibration data from EEPROM
	MyAdcIntrefSet( 1100 );
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
	while(1)
		doYield();
}
