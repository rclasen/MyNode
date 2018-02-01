#ifndef MyNodeItemBatteryVcc_h
#define MyNodeItemBatteryVcc_h

#include "MyNode.h"
#include "MyBattery.h"

/*
 * provide battery status if your node is directly powered from the
 * battery without step-up/down regulator
 *
 * arduino 3v no VRM:	min=2.7,	max=4.5
 * NRF24:		min=1.9,	max=3.6
 * TSL2591:		min=2.7,	max=3.6
 * BH1750:		min=2.4,	max=3.6
 *
 * current battery-level percentage calculation assumes, voltage is
 * dropping mostly lineary during discharge - as with alkaline cells.
 *
 */

class MyNodeItemBatteryVcc : public MyNodeItem {
public:
	MyNodeItemBatteryVcc();

	void setCircuitMin( uint16_t min );
	void setBatteryType( MyBatteryType type );
	void setBatteryCells( uint8_t cells );

protected:
	virtual void registered(void);
	virtual void runAction(MyNodeAction action);
	void actionPollRun(void);

private:
	uint16_t _min;
	MyBatteryType _type;
	uint8_t _cells;
};

#endif
