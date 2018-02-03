
/***********************************
 * network config
 */
#include "mynetwork.h"

/***********************************
 * sketch config
 */

// if enabled, enable debug messages on serial port
#define DEBUG
#ifdef DEBUG
#define MY_DEBUG
#else
#define MY_DISABLED_SERIAL
#endif

/***********************************
 * library includes
 */

// load MySensors library
#include <MySensors.h>
// load MyNode library
#include <MyNode.h>

// when powered directly from battery
//#define WANT_BATTERY
#ifdef WANT_BATTERY
#include <MyNodeItemBatteryVcc.h>
#endif

// external battery status - when using a voltage regulator
#define WANT_BATTERY_ADC
#ifdef WANT_BATTERY_ADC
#include <MyNodeItemBatteryAdc.h>
#endif


//#define WANT_VOLT
#ifdef WANT_VOLT
#include <MyNodeItemVolt.h>
#endif

//#define WANT_TSL2591
#ifdef WANT_TSL2591
#include <MyNodeItemTSL2591.h>
#endif

#define WANT_DALLAS
#ifdef WANT_DALLAS
#include <MyNodeItemDS18B20.h>
#endif


/***********************************
 * node config
 */

const char SKETCH_NAME[] PROGMEM = "mynode";
const char SKETCH_VERSION[] PROGMEM = "1.0";

enum {
	// TSL2591
	SENSORID_LUX = MYNODE_SENSORID_LAST,
	SENSORID_VISIBLE,
	SENSORID_IR,

	// DALLAS
	SENSORID_DALLAS,

	// VOLT
	SENSORID_VOLT,

	SENSORID_MAX,
};

#ifdef WANT_TSL2591
//#define TSL_INTERVAL ( MYNODE_SECOND * 150 ) // 2.5min
#define TSL_INTERVAL ( MYNODE_SEOND * 30 )
MyNodeItemTSL2591 tsl(
	SENSORID_LUX, SENSORID_VISIBLE, SENSORID_IR,
	5
);
#endif

#ifdef WANT_DALLAS
//#define DALLAS_INTERVAL ( MYNODE_SECOND * 150 ) // 2.5min
#define DALLAS_INTERVAL ( MYNODE_SECOND * 30 )
OneWire onewire( 2 );
MyNodeItemDS18B20 dallas(
	SENSORID_DALLAS,
	&onewire
);
#endif

#ifdef WANT_VOLT
#define VOLT_INTERVAL	( MYNODE_SECOND * 150 ) // 2.5min
#define VOLT_FACTOR	MYNODE_VOLT
#define VOLT_PIN_ANALOG A0
#define VOLT_PIN_VCC 3
MyNodeItemVolt volt(
	SENSORID_VOLT,
	VOLT_PIN_ANALOG, VOLT_PIN_VCC
);
#endif

#define BAT_INTERVAL	( MYNODE_SECOND * 30 )
#ifdef WANT_BATTERY
#define BAT_MVOLT_MIN	2600
MyNodeItemBatteryVcc battery;
#endif

#ifdef WANT_BATTERY_ADC
#define BATTERY_FACTOR	MYNODE_VOLT
#define BATTERY_PIN_ANALOG A1
#define BATTERY_PIN_VCC 4
MyNodeItemBatteryAdc battery(
	BATTERY_PIN_ANALOG, BATTERY_PIN_VCC
);
#endif

MyNodeItems(
#ifdef WANT_HEARTBEAT
	&heartbeat,
#endif
#ifdef WANT_TSL2591
	&tsl,
#endif
#ifdef WANT_DALLAS
	&dallas,
#endif
#ifdef WANT_VOLT
	&volt,
#endif
#ifdef WANT_CONTROL
	&control
#endif
#if defined(WANT_BATTERY) || defined(WANT_BATTERY_ADC)
	// battery should be last item:
	&battery
#endif
);


/***********************************
 * no config changes below this line
 */

void MyNodeInit() {
#ifdef DEBUG
	Serial.println(F("before"));
#endif

#ifdef WANT_HEARTBEAT
	heartbeat.setSendInterval( HB_INTERVAL );
#endif

#ifdef WANT_TSL2591
	tsl.setSendInterval(TSL_INTERVAL);
	tsl.setPolls(2);
	tsl.setAvg(3);

	tsl.setGain(TSL2591_GAIN_LOW);
	// tsl.setGain(TSL2591_GAIN_HIGH);

	// tsl.setIntegration(TSL2591_INTEGRATIONTIME_100MS);
	// ... 200, 300, 400, 500, ...
	// tsl.setIntegration(TSL2591_INTEGRATIONTIME_600MS);
	tsl.setIntegration(TSL2591_INTEGRATIONTIME_200MS);
#endif

#ifdef WANT_DALLAS
	dallas.setSendInterval(DALLAS_INTERVAL);
	dallas.setPolls(2);
	dallas.setAvg(3);
#endif

#ifdef WANT_VOLT
	volt.setSendInterval(VOLT_INTERVAL);
	volt.setFactor(VOLT_FACTOR);
#endif

#ifdef WANT_BATTERY
	battery.setSendInterval(BAT_INTERVAL);
	battery.setCircuitMin(BAT_MVOLT_MIN);
#endif
#ifdef WANT_BATTERY_ADC
	battery.setSendInterval(BAT_INTERVAL);
	battery.setFactor(BATTERY_FACTOR);
#endif
}

