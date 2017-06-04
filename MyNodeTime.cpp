#include "MyNodeTime.h"

MyNodeTime _delta = 0;

void MyNodeDelta( MyNodeTime delta )
{
	_delta += delta;
}

MyNodeTime MyNodeNow( void )
{
	return _delta + millis();
}

MyNodeTime MyNodeNext( MyNodeTime delta )
{
	if( delta > MYNODE_TIME_MAXDUR )
		delta = MYNODE_TIME_MAXDUR;

	return MyNodeNow() + delta;
}
