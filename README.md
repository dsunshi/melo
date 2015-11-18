# Melo

Melo is an open source software framework for communication in embedded systems. Melo's format is inspired by both
XCP by ASAM and UDS (ISO 15765-3 and ISO 14229-3) but designed for even the smallest 8-bit controllers. In addition
Melo is designed to easily interface with any physical communication channel such as: I2C, SPI, or RS-232.

## Direct Memory Access

As a built-in feature Melo provides a direct interface to the controller's memory. This allows a user to view
and/or modify variables by memory addresses.

### Dependencies - PC "Master"
* [scons](http://www.scons.org/)   - software construction tool and build environment
 * [Python](http://www.python.org) - scripting language

### Dependencies - embedded "Slave"

Melo uses a custom solution for generating its main state-machine. Although most users should never need to
modify this, the following tools are required.
* [Python](http://www.python.org) - scripting language
 * [Mako](http://www.makotemplates.org/) -  template library written in Python
 * [Cog](http://nedbatchelder.com/code/cog/) - file generation tool written in Python

### Install

Get the code:

    git clone hhttps://github.com/sunshin-es/melo.git

### Installation - Slave

To integrate Melo into the slave device the following to functions are required to be implemented by
the users application software.

    uint8_t * MeloCreatePointer( const uint32_t address );
    void MeloTransmitBytes( const uint8_t * const bytes, const uint8_t length );

In addition to the implementation of the above functions the following functions must be called by the
user.

    void MeloInit(void);
    void MeloBackground(void);
    void MeloReceiveByte( const uint8_t byte );
    void MeloReceiveBytes( const uint8_t * const bytes, const uint8_t num );

#### MeloCreatePointer

`MeloCreatePointer` takes an unsigned 32-bit address and must return a 8-bit pointer
to that address.

#### MeloTransmitBytes

`MeloTransmitBytes` takes an array of bytes with the specified length that the application software
must transmit of whatever physical channel. Once the user software has completed the transmission
it **must** call `MeloTransmitComplete`.

#### MeloReceiveByte/MeloReceiveBytes

Either `MeloReceiveByte` or `MeloReceiveBytes` must be called when the slave receives data from the
physical channel. **Note:** `MeloReceiveBytes` is provided as a convenience when it is possible to
receive multiple bytes; this function calls `MeloReceiveByte`.

#### MeloInit

`MeloInit` is required to be called **once** at startup.

#### MeloBackground

`MeloBackground` is the main processing task for Melo. It is designed such that there are no requirements
for a specific time period between calls. However **it must be called continuously** in order to allow the
main state machine to continue processing.

For example in an Arduino environment:

```c
#include "melo.h"

uint8_t * MeloCreatePointer( const uint32_t address )
{
  /* Convert the address to a pointer */
  return ( (uint8_t *) address );
}

void MeloTransmitBytes( const uint8_t * const bytes, const uint8_t length )
{
  uint8_t bytesSent = 0;

  /* Transmit the data using the Arduino Serial library */
  do
  {
    bytesSent += (uint8_t) Serial.write(bytes, length);
  } while(bytesSent < length);

  /* Notify Melo that the transmission is complete */
  MeloTransmitComplete();
}

void setup()
{
  Serial.begin(9600);
  MeloInit();
}

void loop()
{
  uint8_t input_byte;

  if ( Serial.available() )
  {
    /* Receive a new byte using the Arduino Serial library */
    input_byte = (uint8_t) Serial.read();
    MeloReceiveByte( input_byte );
    MeloBackground();
  }
}
```

### Installation - Master

To integrate Melo into the master device the following to functions are required to be implemented by
the users application software.

    uint8_t * MeloCreatePointer( const uint32_t address );
    void MeloTransmitBytes( const uint8_t * const bytes, const uint8_t length );
    void MeloRequestBytes( const uint8_t num );
    void MeloReceiveResponse( const uint8_t service, const uint8_t subfunction, const uint8_t * const bytes, const uint8_t length, bool postive );

#### MeloRequestBytes

`MeloRequestBytes` is used by the master to request bytes from the slave over the physical channel. This
is required for interfaces such as i2c. In the master implementation this callback must request the
specified number of bytes from the channel. Once the request has completed `MeloTransmitComplete` **must**
be called. If the communication interface does not require the master to request a response from
the slave as in RS-232 the implementation for `MeloRequestBytes` should be as follows:

```c
/* Implementation when slave is capable of sending information without bits clocked from the master */
void MeloRequestBytes( const uint8_t num )
{
    MeloTransmitComplete();
}
```

#### MeloReceiveResponse

`MeloReceiveResponse` is a callback from the main Melo state machine when a slave response has been received.

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
