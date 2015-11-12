#Melo

Melo is an open source software framework for communication in embedded systems. Melo's format is inspired by both
XCP by ASAM and UDS (ISO 15765-3 and ISO 14229-3) but designed for even the smallest 8-bit controllers. In addition
Melo is designed to easily interface with any physical communication channel such as: I2C, SPI, or RS-232.

## Direct Memory Access

As a built-in feature Melo provides a direct interface to the controller's memory. This allows a user to view
and/or modify variables usign memory addresses.

### Dependencies (for PC "Master" interface)
* [scons](http://www.scons.org/)   - software contruction tool and build environment
 * [Python](http://www.python.org) - scripting language

### Dependencies (for embedded "Slave")

Melo uses a custom solution for generating its main state-machine. Although most users should never need to
modify this, the following tools are required.
* [Python](http://www.python.org) - scripting language
* [Mako](http://www.makotemplates.org/) -  template library written in Python
* [Cog](http://nedbatchelder.com/code/cog/) - file generation tool written in Python

### Install

Get the code:

    git clone hhttps://github.com/sunshin-es/melo.git

Generate and Update the main state-machine:

    make gen


### Determining Memory Address using Arduino

1. In the Arduino IDE select: Arduino -> Preferences
then select "Show verbose output during compilation"

2. After the sketch has finished compiling, near the end of the output there will be the path to the .elf file.

```bash
# 3. Generate the map file for the sketch
$ avr-objdump -t sketch_name.elf > sketch_name.map
```

**Note: Currently the only provided "master" example has been tested on Windows.

## Windows Master Example

```bash
# Compile the master from the win-master directory.
# This will generate an executable: SimpleMeloTerm.
$ scons
```

## Arduino "Hello World"

Using the ability to write directly to memory we will demonstrate how to turn an LED on and off.

```sh
$ SimpleMeloTerm.exe
> c
Select device (0 to ?): Select the Arduino COM Port
> 1
Memory address (HEX): 24 <- This is the PORTB configuration register for ATMEGA328
Value to write (HEX): ff <- Set all pins as output
> 1
Memory address (HEX): 25 <- This is the PORTB output register for ATMEGA328
Value to write (HEX): ff <- Set all pins to high
> x
```

### License

Copyright (c) 2015 David Sunshine, <http://sunshin.es>

### Author(s)

David Sunshine