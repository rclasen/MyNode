## Synopsis

MyNode is a library for MySensors nodes.

To recap MySensors terms:

* a **node** is an actual device on the MySensors netowrk.
* each node can have several **sensors**
* each sensor has a **sensor child id** 0..255 within the node
* there are predefined **sensor types** (S_something defines)
* each sensor can send/receive different **value types** (V_something defines)
* MySensors mostly tries to hide what kind of **datat type** you're using for values (integer, float, string)...
* in addition to sensors, a node also handles **internal** commands - eg. to report battery level

In this lib, a MyNodeItem may implement the functionality of one or
several sensors... to allow a single external data poll to feed them
all.

MyNodeItems can also be used to implement "special" tasks like handling
some of the internal commands (provide battery level) or offering means
for runtime configuration of the node.

As a node usually has several items, MyNode dispatches requests to the
registered items and implements advanced scheduling... allowing to save
power or perform other tasks while one item is waiting for somthing else
to happen.

The lib comes with some predefined MyNodeItems, but it's easy to add
your own ones by inheriting from MyNodeItem or any of it's derived
classes.

There are also helpers for common tasks:

* MyAssert: assertions without memory penalty
* MyAvg: averaging of data
* MyAdc: calibrated use of internal 1.1v reference for analog input
* MyTime: time estmate hack as millis() doesn't increase during sleep

## Motivation

The design goal was to be highly flexible, having some real scheduling
and a clean abstraction while still being as conservative to memory as
possible.

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

Use the examples/mynode sketch as a start and customize as needed.

If you want to use the internal reference voltage calibration, take a
look at the examples/calibrateVref sketch to meassure the internal
reference voltage and store it as calibration in EEPROM.

Please note, that the TSL2591 support is using my own local(?) fork of
the Adafruit_TSL2591 library to avoid some of it's shortcommings.

## License

MIT License - see LICENSE file included in distribution
