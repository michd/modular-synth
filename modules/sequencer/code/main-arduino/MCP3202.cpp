#include <SPI.h>
#include "MCP3202.h"

MCP3202::MCP3202(uint8_t pinChipSelect) {
  _pinChipSelect = pinChipSelect;
}

void MCP3202::begin() {
  ::pinMode(_pinChipSelect, OUTPUT);
  ::digitalWrite(_pinChipSelect, 1);
  SPI.begin();
  _spiSettings = SPISettings(18000000, MSBFIRST, SPI_MODE0);
}

unsigned int MCP3202::analogRead(bool channel) {
  return _read(1, channel);
}

unsigned int MCP3202::analogReadDifferential(bool sign) {
  return _read(0, sign);
}

unsigned int MCP3202::_read(bool singleOrDiff, bool oddOrSign) {
  // See MCB3202 datasheet page 13 ("5. Serial Communications") for details

  // TODO: consider whether most significant bit first should be configurable
  byte commandBits = B00001001; // Leading zeroes, 2 config bits, MSBFIRST

  commandBits |= singleOrDiff << 2;
  commandBits |= oddOrSign << 1;

  _beginTransmission();
  
  // Transfer command bits (with leading 0s as we work in bytes)
  SPI.transfer(commandBits);
  // Get most significant 7 bits - 7 because the first one (MSB) is a null bit
  unsigned int msbits7 = SPI.transfer(0) & B01111111;
  // Get leas significant 5 bits (and 3 extra bits we'll discard)
  unsigned int lsbbits5 = SPI.transfer(0) & B11111000;
 
  _endTransmission();
  
  // Return the compound result
  // Shift msbits7 5 bits to the right to make room for the least significant bits
  // Shift least significant bits 3 to the right to get rid of extra bits and move
  // the ones we want to the right place
  // then OR them together to form a 12 bit number
  return (msbits7 << 5) | (lsbbits5 >> 3);
}

void MCP3202::_beginTransmission() {
  ::digitalWrite(_pinChipSelect, 0);
  SPI.beginTransaction(_spiSettings);
}

void MCP3202::_endTransmission() {
  SPI.endTransaction();
  ::digitalWrite(_pinChipSelect, 1);
}
