/*
 * MCP3202 dual channel ADC Library
 * By MichD
 */

#include <Arduino.h>
#include <stdint.h>

#ifndef MCP3202_h
#define MCP3202_h

class MCP3202 {
 public:
  // Constructor, takes chip select pin
   MCP3202(uint8_t);
  
   // Start the SPI bus
   void begin();

   // Read value from a single channel; 0 = channel 0, 1 = channel 1
   uint16_t analogRead(bool);

   // Read value in pseudodifferential mode, selecting sign
   uint16_t analogReadDifferential(bool); 
   
 private:
   uint8_t _pinChipSelect;
   SPISettings _spiSettings;
   uint16_t _read(bool, bool);
   void _beginTransmission();
   void _endTransmission();   
};

#endif // MCP3202_h
