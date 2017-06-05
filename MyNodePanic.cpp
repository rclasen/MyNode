#include "MyNodePanic.h"

#include <Arduino.h>
#include <core/MyIndication.h>
#include <core/MySensorsCore.h>

void MyNodePanic( void )
{
	setIndication(INDICATION_ERR_HW_INIT);
	while(1)
		doYield();
}
