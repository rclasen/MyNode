#ifndef MyNodeItemBatteryAdc_h
#define MyNodeItemBatteryAdc_h

#include "MyNodeItemVolt.h"
#include "MyBattery.h"

/*
 * provide battery status
 *
 * see MyNodeItemBatteryVcc for details on level calculation
 *
 */

class MyNodeItemBatteryAdc : public MyNodeItemVolt {
public:
	MyNodeItemBatteryAdc( uint8_t analog_pin,
			uint8_t vcc_pin = MYNODE_PIN_NONE );

	virtual const __FlashStringHelper *getName( void );
	void setCircuitMin( uint16_t min );
	void setBatteryType( MyBatteryType type );
	void setBatteryCells( uint8_t cells );

protected:
	void actionPollRun(void);

private:
	MyBatteryType _type;
	uint8_t _cells;
};

#endif
