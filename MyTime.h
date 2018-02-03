#ifndef MyTime_h
#define MyTime_h

#include <Arduino.h>

// some millisecond-ish timer
// not precise, at all
// attempt to workaround millis() not increasing while powered down
// will rollover every 49 days (as millis() does)
// so... only compare durations... don't compare absolute timestamps

#define MYNODE_SECOND1	(1000UL)

typedef uint32_t MyTime;
#define MYNODE_TIME_MAXDUR (UINT32_MAX>>1)

// current "time"-stamp
MyTime MyTimeNow( void );

// future timestamp
MyTime MyTimeNext( MyTime delta );

// add time correction millis() didn't notice
void MyTimeFixup( MyTime delta );

MyTime MyTimeDuration( MyTime start, MyTime end );

#endif
