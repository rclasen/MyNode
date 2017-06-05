#include "MyNodeVcc.h"

#include <Arduino.h>
#include <readvcc.h>

MyNodeTime _last(MyNodeNow());

long _vcc = 0;

// calibration defaults to exact 1.1v (1125300 = 1.1*1023*1000)
long _calibration = 1125300L;

// TODO: store/get Calibration in EEPROM

void MyNodeVccCalibration( long calibration )
{
	_calibration = calibration;
}

long MyNodeVcc( MyNodeTime maxage )
{
	MyNodeTime now = MyNodeNow();

	if( ! _vcc || now - _last > maxage ){
		_vcc = readVcc( _calibration );
		_last = now;
	}

	return _vcc;
}

