
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

#define WANT_TSL2591
#ifdef WANT_TSL2591
#include <MyNodeItemTSL2591.h>
#endif

// TODO: #define WANT_CONTROL
#ifdef WANT_CONTROL
#include <MyNodeItemConntrol.h>
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
#define TSL_INTERVAL (150L * 1000)
#endif

#ifdef WANT_VOLT
#define VOLT_INTERVAL (150L * 1000)
#define VOLT_PIN_ANALOG A0
#define VOLT_PIN_VCC 2
#endif

#ifdef WANT_BATTERY
#define BAT_INTERVAL (300L * 1000)
#define BAT_MVOLT_MIN 2600
#define BAT_MVOLT_MAX 3300
#endif

/***********************************
 * no config changes below this line
 */


// before
void before() {
#ifdef DEBUG
	// setup the serial port baud rate
	Serial.begin(MY_BAUD_RATE);
	Serial.println(F("before"));
#endif

	MyNodeInit( 4 );

#ifdef WANT_TSL2591
	MyNodeRegisterItem( new MyNodeItemTSL2591(
		SENSORID_LUX, SENSORID_VISIBLE, SENSORID_IR,
		TSL_INTERVAL
	));
#endif

#ifdef WANT_VOLT
	MyNodeRegisterItem( new MyNodeItemVolt(
		SENSORID_VOLT,
		VOLT_PIN_ANALOG, VOLT_PIN_VCC,
		VOLT_INTERVAL, 30
	));
#endif

#ifdef WANT_CONTROL
	MyNodeRegisterItem( new MyNodeItemControl(
	));
#endif

#ifdef WANT_BATTERY
	// battery should be last item:
	MyNodeRegisterItem( new MyNodeItemBatteryVcc(
		BAT_MVOLT_MIN, BAT_MVOLT_MAX,
		BAT_INTERVAL
	));
#endif

	MyNodeBefore();
}

/***********************************
 * no changes below this line
 */

// presentation
void presentation() {
	MyNodePresentation( PGMT(SKETCH_NAME), PGMT(SKETCH_VERSION) );

}

// loop
void loop() {
	MyNodeLoop();

}

// receive
void receive(const MyMessage &message) {
	MyNodeReceive(message);
}

// receive
void receiveTime( unsigned long ts ){
	MyNodeReceiveTime(ts);
}
