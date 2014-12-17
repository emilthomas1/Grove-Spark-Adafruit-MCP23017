/*************************************************** 
 This is a library for the Adafruit MCP23017 I2C port expander

 Adafruit invests time and resources providing this open source code, please 
 support Adafruit and open-source hardware by purchasing products from 
 Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 
 Adapted for Spark Core by Paul Kourany, Sept 3, 2014
 Extended for Grove Labs by Louis DeScioli, Dec 16 2014
 ****************************************************/

#include "Adafruit_MCP23017_Spark.h"

/**
 * Initializes the MCP23017 with an I2C address.
 * The address must match what the address bits are wired to. If no parameters
 * are given, it will be assumed that the address pins are 0 and the default
 * address as specified in the header file will be used.
 * @param addr    the I2C address of the chip. Should be 0->7, only 3 bits
 * @param debug   whether or not to print debugging statements to Serial port
 */
Adafruit_MCP23017_Spark::Adafruit_MCP23017_Spark(uint8_t addr, bool debug) {
  i2caddr = addr;
  debugging = debug;
}

/**
 * Join the I2C bus as a master
 * Not necessarily needed if Wire.begin() is called elsewhere
 */
void Adafruit_MCP23017_Spark::begin(void) {
  Wire.begin();
}

/**
 * Set the mode of a GPIO pin
 * @param bank  The GPIO bank, either A or B. If not 'a' assumed to be 'b'
 * @param pin   The GPIO pin number
 * @param mode  Either INPUT (1) or OUTPUT (0)
 */
void Adafruit_MCP23017_Spark::setPinMode(char bank, uint8_t pin, uint8_t mode) {
  tolower(bank) == 'a' ? setBit(MCP23017_IODIRA, pin, mode) : setBit(MCP23017_IODIRB, pin, mode);
}

/**
 * Enable or disable an internal pullup resistor for a given GPIO pin
 * @param bank  The GPIO bank, either A or B. If not 'a' assumed to be 'b'
 * @param pin   The GPIO pin number, should be 0->7
 * @param mode  The state of the pinup, should be 0 or 1
 */
void Adafruit_MCP23017_Spark::setPullUp(char bank, uint8_t pin, uint8_t mode) {
  tolower(bank) == 'a' ? setBit(MCP23017_GPPUA, pin, mode) : setBit(MCP23017_GPPUB, pin, mode);
}

/**
 * Get the value of a single GPIO pin for a given bank
 * @param  bank  The GPIO bank, either A or B. If not 'a' assumed to be 'b'
 * @param  pin   The pin to read, 0->7
 * @return       The digital value of the pin, either 0 or 1
 */
uint8_t Adafruit_MCP23017_Spark::readPin(char bank, uint8_t pin) {
  return bitRead(readBank(bank), pin);
}

/**
 * Get the values of a bank of GPIO pins
 * @param  bank  The GPIO bank, either A or B. If not 'a' assumed to be 'b'
 * @return  The values of the 8 GPIO pins in the given bank, as an 8-bit int
 */
uint8_t Adafruit_MCP23017_Spark::readBank(char bank) {
  return tolower(bank) == 'a' ? readRegister(MCP23017_GPIOA) : readRegister(MCP23017_GPIOB);
}

/**
 * Get the values of all 16 GPIO pins
 * Make sure that the GPIO pins have been setup as inputs!
 * @return  The 16 values of the GPIO pins as an unsigned 16-bit int.
 *          8-MSB are bank B, 8-LSB are bank A
 */
uint16_t Adafruit_MCP23017_Spark::readAll() {
  uint16_t result = 0;
  uint8_t resultA = readRegister(MCP23017_GPIOA);
  uint8_t resultB = readRegister(MCP23017_GPIOB);
  result |= resultB;
  result <<= 8;
  result |= resultA;
  return result;
}

/**
 * Write a value out of a GPIO pin for a given bank, leaving the others untouched
 * @param bank  A GPIO bank, either A or B. If not 'a' will be assumed to be 'b'
 * @param pin   The specific pin outputting the given value
 * @param value The digital output value for the pin, either 0 or 1
 */
void Adafruit_MCP23017_Spark::writePin(char bank, uint8_t pin, uint8_t value) {
  bank == 'a' ? setBit(MCP23017_GPIOA, pin, value) : setBit(MCP23017_GPIOB, pin, value);
}

/**
 * Write a byte out for a given bank of GPIO pins
 * @param bank  The bank of GPIO pins. If not 'a' will be treated as 'b'
 * @param value The 8-bit value to be output
 */
void Adafruit_MCP23017_Spark::writeBank(char bank, uint8_t value) {
  tolower(bank) == 'a' ? writeRegister(MCP23017_GPIOA, value) : writeRegister(MCP23017_GPIOB, value);
}

/**
 * Writes to all of the GPIO in one go. This method is very useful if you are
 * implementing a multiplexed matrix and want to get a decent refresh rate.
 * Make sure that the GPIO pins have been setup as outputs!
 * @param value  The 16 bit value to be written. 
 *               The 8-LSB go to bank A, 8-MSB go to bank B
 */
void Adafruit_MCP23017_Spark::writeAll(uint16_t value) {
  uint8_t aValue = (value & 0xff);
  uint8_t bValue = (( value >> 8 ) & 0xff);
  writeRegister(MCP23017_GPIOA, aValue);
  writeRegister(MCP23017_GPIOB, bValue);
}

/**
 * Configure some parts of the chip. Port A and B are assigned the same configuration.
 * @param mirroring  OR the INTA and INTB pins. POR value is false
 * @param openDrain  Set the INT pin to value or open drain. POR value is false
 * @param polarity   Set LOW or HIGH on interrupt. POR value is LOW
 */
void Adafruit_MCP23017_Spark::configure(uint8_t mirroring, uint8_t openDrain, uint8_t polarity){
  // configure the port A
  uint8_t ioconfValue = readRegister(MCP23017_IOCONA);
  bitWrite(ioconfValue, 6, mirroring);
  bitWrite(ioconfValue, 2, openDrain);
  bitWrite(ioconfValue, 1, polarity);
  writeRegister(MCP23017_IOCONA, ioconfValue);

  // Configure the port B
  ioconfValue = readRegister(MCP23017_IOCONB);
  bitWrite(ioconfValue, 6, mirroring);
  bitWrite(ioconfValue, 2, openDrain);
  bitWrite(ioconfValue, 1, polarity);
  writeRegister(MCP23017_IOCONB, ioconfValue);
}

///////////////////////////////////////////////////////////////////////////////
// Private Methods
/////////////////////////////////////////////////////////////////////////////// 

/**
 * Read the byte stored in a register
 * @param  addr  The address of the register on the MCP23017 to read
 * @return       The 8-bit value stored in the register
 */
uint8_t Adafruit_MCP23017_Spark::readRegister(uint8_t addr){
  Wire.beginTransmission(i2caddr);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(i2caddr, 1);
  return Wire.read();
}

/**
 * Write a byte to a given register
 * @param addr   The address of the register on the MCP23017 to write to
 * @param value  The 8-bit value to write
 */
void Adafruit_MCP23017_Spark::writeRegister(uint8_t addr, uint8_t value){
  Wire.beginTransmission(i2caddr);
  Wire.write(addr);
  Wire.write(value);
  Wire.endTransmission();
}

/**
 * Edit a single bit of a given register
 * @param regAddr The address of the register to be written to
 * @param bit     The bit position to be set
 * @param value   The value to be set at the bit position (0 or 1)
 */
void Adafruit_MCP23017_Spark::setBit(uint8_t regAddr, uint8_t bit, uint8_t value) {
  uint8_t number = readRegister(regAddr);   // Read out the current value
  bitWrite(number, bit, value);           // write the value at the bit position
  writeRegister(regAddr, number);     // write the byte back to the register
}
