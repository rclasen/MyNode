#ifndef MyNodeTime_h
#define MyNodeTime_h

#include <Arduino.h>

// some millisecond-ish timer
// not precise, at all
// attempt to workaround millis() not increasing while powered down
// will rollover every 49 days (as millis() does)
// so... only compare durations... don't compare absolute timestamps

typedef uint32_t MyNodeTime;
#define MYNODE_TIME_MAXDUR (UINT32_MAX>>1)

// current "time"-stamp
MyNodeTime MyNodeNow( void );

// future timestamp
MyNodeTime MyNodeNext( MyNodeTime delta );

// add time correction millis() didn't notice
void MyNodeDelta( MyNodeTime delta );

#endif
