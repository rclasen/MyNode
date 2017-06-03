#include "MyNodeItemTSL2591.h"

MyNodeItemTSL2591::MyNodeItemTSL2591( uint8_t lux, uint8_t visible, uint8_t ir )
{
	uint8_t childv[] = { lux, visible, ir };
	MyNodeItem( 3, childv );
};

mysensor_sensor MyNodeItemTSL2591::getChildSensor(uint8_t child)
{
	return S_LIGHT_LEVEL;
};

bool MyNodeItemTSL2591::before(void)
{
	_sensor.begin();
	_sensor.setGain(TSL2591_GAIN_LOW);
	//_sensor.setGain(TSL2591_GAIN_HIGH);
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_100MS);
	//... 200, 300, 400, 500, ...
	//_sensor.setTiming(TSL2591_INTEGRATIONTIME_600MS);
	_sensor.setTiming(TSL2591_INTEGRATIONTIME_200MS);
};

bool MyNodeItemTSL2591::loop(void)
{
	return sendAll();
};

bool MyNodeItemTSL2591::sendAll(void)
{
	bool result = true;

	uint32_t lum = _sensor.getFullLuminosity();
	if( lum == UINT32_MAX ){
		Serial.println(F("TSL2591 FAILED to get lum"));
		return false;
	}

	uint16_t ir = lum >> 16;
	uint16_t full = lum & 0xFFFF;
	uint16_t visible = full - ir;
	uint32_t lux = _sensor.calculateLux( full, ir );

#if DEBUG
	Serial.print(F(" lum: ")); Serial.println( lum );
	Serial.print(F(" ir: ")); Serial.println( ir );
	Serial.print(F(" full: ")); Serial.println( full );
	Serial.print(F(" visible: ")); Serial.println( visible );
	Serial.print(F(" lux: ")); Serial.println( lux );
#endif

	result &= send(_msg_set(2).set(ir));
	result &= send(_msg_set(2).set(visible));
	if( lux >= TSL2591_LUX_CLIPPED ){
		Serial.println(F(" FAILED to get lux"));
		lux=TSL2591_LUX_CLIPPED; // maxes out at 88 kLux
	}
	result &= send(_msg_set(0).set(lux));

	return result;
}


