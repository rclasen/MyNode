#include "MyTime.h"

static MyTime _delta = 0;

void MyTimeFixup( MyTime delta )
{
	_delta += delta;
}

MyTime MyTimeNow( void )
{
	return _delta + millis();
}

MyTime MyTimeNext( MyTime delta )
{
	if( delta > MYNODE_TIME_MAXDUR )
		delta = MYNODE_TIME_MAXDUR;

	return MyTimeNow() + delta;
}

MyTime MyTimeDuration( MyTime start, MyTime end )
{
	MyTime dur = end - start;

	if( dur > MYNODE_TIME_MAXDUR )
		return 0;

	return dur;
}

