#ifndef MyAdc_h
#define MyAdc_h

#include "MyTime.h"

// to avoid float arithmetics, we use milli-volts:
#define MYNODE_VOLT	(1000ul)

// get voltage of internal reference using externally measured Vcc as reference
uint16_t MyAdcReadIntref( uint16_t external_mvolts );

// calibration: set/get actual voltage of internal ADC reference
// it's defined to be 1100 mvolts but might vary a bit on each device
void MyAdcIntrefSet( uint16_t actual_mvolts );
uint16_t MyAdcIntrefGet();

// return VCC in millivolts
uint16_t MyAdcVcc( MyTime maxage = (MYNODE_SECOND*60) );

// return voltage at pin in mvolts (0..VCC) using VCC as reference
uint16_t MyAdcReadVcc( uint8_t pin, MyTime vccage = (MYNODE_SECOND*60) );

// voltage at pin in mvolts (0..intref) using the calibrated internal 1.1V reference
// always use this (with a voltage divider) if something else but Vcc is the power source
uint16_t MyAdcReadInt( uint8_t pin );

/*
 * if you need to measure voltages (Vreal) higher than your voltage
 * reference or want to reduce current (and thereby power), a voltage
 * divider with 2 resistors R1 and R2 might help:
 *
 * wiring: Vreal - R1 - pin - R2 - GND
 *
 * you'll then measurte the voltage drop between pin and GND and
 * calculate Vreal from this:
 *
 * Vreal = Vpin * ( R1 + R2 ) / R2
 *
 * The last term of this formula won't change at runtime, so you can
 * precalculate this correction factor and use it in subsequent
 * calculations.
 *
 * If you don't have a voltager divider, just use 1 as factor.
 *
 * Use the maximum voltages to determin the size of your resistors:
 *
 * R1 = R2 * ( Vreal / Vpin - 1 )
 *
 * Pick values for R2 until you get a "standard" value for R1.  The
 * bigger the resistance, the lower the current/power usage.
 *
 * Try to keep R2 below 10k but ensure, that there's not too much
 * current for the analog input (that has 100 MOhm resistance)
 * (http://forum.arduino.cc/index.php?topic=180478.0)
 *
 * current on R2 should be at least 100 to 1000 times higher as current
 * through analog input.
 *
 * for "steady" voltages, add a 1 uF capacitor parallel to R2
 *
 *
 *
 *
 * If you use resistive sensors, connect them like R2. Vreal then
 * becomes Vcc... and you can calculate the current resistance:
 *
 * R2 = R1 / ( Vcc / Vpin - 1 )
 *
 *
 *
 * If you need the current, this forula applies:
 *
 * I = Vpin / R2
 *
 *
 * see also: https://learn.sparkfun.com/tutorials/voltage-dividers
 */

#endif
