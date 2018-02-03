## Synopsis

MyNode is an "Operating System" for MySensors nodes.

To recap MySensors terms:

* a **node** is an actual device on the MySensors netowrk.
* each node can have several **sensors**
* each sensor has a **sensor child id** 0..255 within the node
* there are predefined **sensor types** (S_something defines)
* each sensor can send/receive different **value types** (V_something defines)
* MySensors mostly tries to hide what kind of **datat type** you're using for values (integer, float, string)...
* in addition to sensors, a node also handles **internal** commands - eg. to report battery level

MyNode providing the infrastructure for MyNodeItems. It's taking over
the usual hooks (loop(), before(), ...). Instead it just takes a global
list of MyNodeItems and their configuration. The list of services
provided for the MyNodeItems is growing:

* power saving
* Scheduling for async operation of multiple MyNodeItems.
* automated EEPROM loading/saving
* managing ADC reference calibration data
* heartbeats (if desired)
* dispatching received messages to items
* TODO: retransmit handling (if desired)

Each MyNodeItems can map data retrieved from a single source to a number
of MySensors child ids.

MyNodeItems are also used to implement "special" tasks like handling
some of the internal commands (provide battery level) or offering means
for runtime configuration of the node.

"MyNodeItem" actually is a virtual base from which the individual
implementations are derived. While virtual classes come with a SRAM
memory penalty for the vtables (one for each class), this allows easy
implementation of new items. The lib comes with a growing list of
predefined MyNodeItems:

* TSL2951 light/ir sensor
* Dallas DS18B20 temperature sensors
* Analog inputs
* Battery Level (direcly powered)
* TODO: Battery Level (analog input, when using a converter)
* TODO: interrupt binary input
* TODO: binary output
* TODO: BH1750 light sensor

There are also lower level helpers for common tasks:

* MyAssert: assertions without memory penalty
* MyAvg: averaging of data
* MyAdc: calibrated use of internal 1.1v reference for analog input
* MyTime: time estmate hack as millis() doesn't increase during sleep
* MyBattery: get remaining capacity for specific cell types (alkaline, ...)

## Motivation

The design goal was to be highly flexible, having some real scheduling
and a clean abstraction while still being as conservative to memory as
possible.

This means, I'm trying to

* avoid pulling in float routines (may save flash space)
* avoid dynamic/heap memory (no memory fragmentation)
* use PROGMEM wherever possible (save RAM space)
* use virtual classes only when benefit is bigger than memory penalty
* allow the linker to omit code for unused features

When looking at (very few) other node implementations I didn't see this.
This project also felt like having a reasonable scope to get some
practice for my hobbyist C++ skills.

## Code Example

Please take a look at the example node in examples/mynode/mynode.ino on
how to use existing Items.

For adding new item types, it's likely a good idea to start by looking
at the TSL2591 item class.

**WARNING**: The API is still in development and changes frequently.

## Installation

Just pull/download/copy this to your Arduino libraries folder.

**WARNING**: currently this needs some tiny modifications to MySensors
and some of the sensor libraries I haven't bothered to push upstream,
yet.

Use the examples/mynode sketch as a start and customize as needed.

If you want to use the internal reference voltage calibration, take a
look at the examples/calibrateVref sketch to meassure the internal
reference voltage and store it as calibration in EEPROM.

Please note, that the TSL2591 support is using my own local(?) fork of
the Adafruit_TSL2591 library to avoid some of it's shortcommings.

## License

MIT License - see LICENSE file included in distribution
