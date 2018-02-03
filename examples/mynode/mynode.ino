
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
#define WANT_BATTERY
#ifdef WANT_BATTERY
#include <MyNodeItemBatteryVcc.h>
#endif

// TODO: external battery status - when using a voltage regulator

#define WANT_VOLT
#ifdef WANT_VOLT
#include <MyNodeItemVolt.h>
#endif

//#define WANT_TSL2591
#ifdef WANT_TSL2591
#include <MyNodeItemTSL2591.h>
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

	// VOLT
	SENSORID_VOLT,

	SENSORID_MAX,
};

#ifdef WANT_TSL2591
//#define TSL_INTERVAL (150L * 1000)
#define TSL_INTERVAL (30L * 1000)
MyNodeItemTSL2591 tsl(
	SENSORID_LUX, SENSORID_VISIBLE, SENSORID_IR,
	5
);
#endif

#ifdef WANT_VOLT
#define VOLT_INTERVAL (150L * 1000)
#define VOLT_FACTOR 1.0
#define VOLT_PIN_ANALOG A0
#define VOLT_PIN_VCC 2
MyNodeItemVolt volt(
	SENSORID_VOLT,
	VOLT_PIN_ANALOG, VOLT_PIN_VCC
);
#endif

#ifdef WANT_BATTERY
#define BAT_INTERVAL (300L * 1000)
#define BAT_MVOLT_MIN 2600
MyNodeItemBatteryVcc battery;
#endif

MyNodeItems(
#ifdef WANT_HEARTBEAT
	&heartbeat,
#endif
#ifdef WANT_TSL2591
	&tsl,
#endif
#ifdef WANT_VOLT
	&volt,
#endif
#ifdef WANT_CONTROL
	&control
#endif
#ifdef WANT_BATTERY
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
	tsl.setPolls(1);
	tsl.setAvg(2);

	tsl.setGain(TSL2591_GAIN_LOW);
	// tsl.setGain(TSL2591_GAIN_HIGH);

	// tsl.setIntegration(TSL2591_INTEGRATIONTIME_100MS);
	// ... 200, 300, 400, 500, ...
	// tsl.setIntegration(TSL2591_INTEGRATIONTIME_600MS);
	tsl.setIntegration(TSL2591_INTEGRATIONTIME_200MS);
#endif

#ifdef WANT_VOLT
	volt.setSendInterval(VOLT_INTERVAL);
	volt.setFactor(VOLT_FACTOR);
#endif

#ifdef WANT_BATTERY
	battery.setSendInterval(BAT_INTERVAL);
	battery.setCircuitMin(BAT_MVOLT_MIN);
#endif
}

