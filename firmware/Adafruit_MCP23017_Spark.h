/*************************************************** 
  This is a library for the MCP23017 i2c port expander

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _Adafruit_MCP23017_Spark_H_
#define _Adafruit_MCP23017_Spark_H_

#include "application.h"

// Register address -- these are only good if IOCON.BANK is set to 0
#define MCP23017_IODIRA 0x00
#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLA 0x02
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENA 0x04
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALA 0x06
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONA 0x08
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONA 0x0A  // The differentation between IOCON A & B
#define MCP23017_IOCONB 0x0B  // doesn't actually mean anything. See datasheet
#define MCP23017_GPPUA 0x0C
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFA 0x0E
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPA 0x10
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOA 0x12
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATA 0x14
#define MCP23017_OLATB 0x15

#define MCP23017_INT_ERR 255

// reads the bit of a number
#define bitRead(number, bit) ( (number >> bit) & 0x01 )
// writes a 1 to a bit of a number
#define bitSet(number, bit) ( number |= (1UL << bit) )
// writes a 0 to a bit of a number
#define bitClear(number, bit) ( number &= ~(1UL << bit) )
// writes a specified value (0 or 1) to a bit of a number
#define bitWrite(number, bit, value) ( value ? bitSet(number, bit) : bitClear(number, bit) )


class Adafruit_MCP23017_Spark {
 public:
  Adafruit_MCP23017_Spark(uint8_t addr = 0x20, bool debug = false);
  void begin(void);
  void setPinMode(char bank, uint8_t pin, uint8_t mode);
  void setPullUp(char bank, uint8_t pin, uint8_t mode);
  uint8_t readPin(char bank, uint8_t pin);
  uint8_t readBank(char bank);
  uint16_t readAll(void);
  void writePin(char bank, uint8_t pin, uint8_t value);
  void writeBank(char bank, uint8_t value);
  void writeAll(uint16_t value);
  void configure(uint8_t mirroring, uint8_t open, uint8_t polarity);

 private:
  uint8_t i2caddr;
  bool debugging;

  uint8_t readRegister(uint8_t addr);
  void writeRegister(uint8_t addr, uint8_t value);
  void setBit(uint8_t regAddr, uint8_t bit, uint8_t value);

};

#endif
