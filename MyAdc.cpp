#include "MyAdc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// cached vcc reading
MyTime _vcc_time(0);
uint16_t _vcc = 0;

// calibration of internal 1.1V reference defaults to exact 1.1v
uint16_t _adc_intref = 1100;

void MyAdcIntrefSet( uint16_t actual_mvolts )
{
	_adc_intref = actual_mvolts;
}

uint16_t MyAdcIntrefGet( void )
{
	return _adc_intref;
}

// TODO: this is arduino/avr specific, use hwCPUVoltage or similar elsewhere

uint16_t readVcc( ) {
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = _BV(MUX3) | _BV(MUX2);
#else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

	delay(20); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Start conversion
	while (bit_is_set(ADCSRA,ADSC)); // measuring

	uint16_t raw = ADC;
	if( ! raw )
		return 0;

	return ( 1023L * _adc_intref / raw );
}

uint16_t MyAdcVcc( MyTime maxage )
{
	MyTime now = MyTimeNow();

	if( ! _vcc || now - _vcc_time > maxage ){
		_vcc = readVcc();
		_vcc_time = now;
	}

	return _vcc;
}

uint16_t MyAdcRead( uint8_t pin, MyTime vccage )
{
	uint16_t vcc = MyAdcVcc( vccage );

	// DEFAULT: use vcc as reference
	analogReference(DEFAULT);
	uint16_t raw = analogRead(pin);

	return (uint32_t)vcc * raw / 1023;
}
