#ifndef MyNodeVcc_h
#define MyNodeVcc_h

#include "MyNodeTime.h"

#ifndef MYNODE_VCC_CALIBRATION
#warning "please define MYNODE_VCC_CALIBRATION for accurate meassurements"
// calibration defaults to exact 1.1v (1125300 = 1.1*1023*1000)
#define MYNODE_VCC_CALIBRATION 1125300L
#endif

// return VCC in millivolts
long MyNodeVcc( MyNodeTime maxage = 60000 );

#endif
