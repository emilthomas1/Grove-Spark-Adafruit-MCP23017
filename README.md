# Adafruit_MCP23017 I2C expander library

This is a library for the [Adafruit MCP23017 I2C Port Expander](http://www.adafruit.com/product/732).

## Datasheet
[Found here](http://www.adafruit.com/datasheets/mcp23017.pdf)


## Usage
This library works exclusively on the Spark, it no longer checks for Arduino to do alternate imports and definitions.

### Default address
The default address of the MCP23017 is `0x20`. If you call the constructor with no parameters this address is automatically used (as set in the header).

### Default IO direction
The Power on Reset value of the I/O direction registers is `1111 1111`, which means that both banks of GPIO pins are setup as inputs. You have to explicitly set them as outputs if you want to use them as such, with `setPinMode()`

## Contributing
Currently there are no abstractions written for setting up interrupts, if someone wanted to contribute those I would happily take a pull request. The interrupt and toggle examples also need to be refactored

## Credits
Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

Adapted for Spark Core by Paul Kourany, Sept 3, 2014  
Extended for Grove Labs by Louis DeScioli, December 2014

