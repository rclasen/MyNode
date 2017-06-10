#include "MyTime.h"

MyTime _delta = 0;

void MyTimeDelta( MyTime delta )
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
