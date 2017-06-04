#include "MyNodeVcc.h"

#include <Arduino.h>
#include <readvcc.h>

MyNodeTime last(MyNodeNow());
long vcc = 0;

long MyNodeVcc( MyNodeTime maxage )
{
	MyNodeTime now = MyNodeNow();

	if( ! vcc || now - last > maxage ){
		vcc = readVcc( MYNODE_VCC_CALIBRATION );
		last = now;
	}

	return vcc;
}

