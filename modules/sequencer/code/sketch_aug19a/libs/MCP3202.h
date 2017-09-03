/*
 * MCP3202 dual channel ADC Library
 * By MichD
 */

#include <Arduino.h>

#ifndef MCP3202_h // Ensure we're not double-defining the library if it was already included
#define MCP3202_h

class MCP3202 {
 public:
   MCP3202(uint8_t);                          // Constructor, takes chip select pin
   void begin();                              // Start the SPI bus
   unsigned int analogRead(bool);             // Read value from a single channel; 0 = channel 0, 1 = channel 1
   unsigned int analogReadDifferential(bool); // Read value in pseudodifferential mode, selecting sign
   
 private:
   uint8_t _pinChipSelect;
   SPISettings _spiSettings;
   unsigned int _read(bool, bool);
   void _beginTransmission();
   void _endTransmission();   
};

#endif // MCP3202_h
