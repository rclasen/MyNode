#ifndef MyNodeVcc_h
#define MyNodeVcc_h

#include "MyNodeTime.h"

// return VCC in millivolts
long MyNodeVcc( MyNodeTime maxage = 60000 );

// set calibration for a specific device
void MyNodeVccCalibration( long calibration );

#endif
