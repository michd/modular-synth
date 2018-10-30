/*
  MCP23S17.cpp  Version 0.2
  Microchip MCP23S17 SPI I/O Expander Class for Arduino
  Created by Cort Buffington & Keith Neufeld
  March, 2011
  January, 2013
  January, 2015
  February, 2016

  Features Implemented (by word and bit):
    I/O Direction
    Pull-up on/off
    Input inversion
    Output write
    Input read

  Interrupt features are not implemented in this version
  byte based (portA, portB) functions are not implemented in this version

  NOTE:  Addresses below are only valid when IOCON.BANK=0 (register addressing mode)
         This means one of the control register values can change register addresses!
         The default values is 0, so that's how we're using it.

         All registers except ICON (0xA and 0xB) are paired as A/B for each 8-bit GPIO port.
         Comments identify the port's name, and notes on how it is used.

         *THIS CLASS ENABLES THE ADDRESS PINS ON ALL CHIPS ON THE BUS WHEN THE FIRST CHIP OBJECT IS INSTANTIATED!

  USAGE: All Read/Write functions except wordWrite are implemented in two different ways.
         Individual pin values are set by referencing "pin #" and On/Off, Input/Output or High/Low where
         portA represents pins 0-7 and portB 8-15. So to set the most significant bit of portB, set pin # 15.
         To Read/Write the values for the entire chip at once, a word mode is supported buy passing a
         single argument to the function as 0x(portB)(portA). I/O mode Output is represented by 0.
         The wordWrite function was to be used internally, but was made public for advanced users to have
         direct and more efficient control by writing a value to a specific register pair.

  VERSION RELESE NOTES:
        V0.2
        Changed direct manipulation of pin 10 on ATMega168/328 via "PORTB" to use digitalWrite on an arbitrary
        SlaveSelect pin passed to the object through the constructor
*/

#include <SPI.h>                 // Arduino IDE SPI library - uses AVR hardware SPI features
#include "MCP23S17.h"            // Header files for this class

// Defines to keep logical information symbolic go here

#ifndef HIGH
#define    HIGH          (1)
#endif

#ifndef LOW
#define    LOW           (0)
#endif

#ifndef ON
#define    ON            (1)
#endif

#ifndef OFF
#define    OFF           (0)
#endif

// Control byte and configuration register information - Control Byte: "0100 A2 A1 A0 R/W" -- W=0

#define    OPCODEW       (0b01000000)  // Opcode for MCP23S17 with LSB (bit0) set to write (0), address OR'd in later, bits 1-3
#define    OPCODER       (0b01000001)  // Opcode for MCP23S17 with LSB (bit0) set to read (1), address OR'd in later, bits 1-3
// Configuration register for MCP23S17,
// the only things we change are:
// - enabling hardware addressing (bit 3)
// - enabling interrupt pin mirroring (so you only need to use one on the MCU)
#define    ADDR_ENABLE   (0b01001100)  

// Constructor to instantiate an instance of MCP to a specific chip (address)
MCP23S17::MCP23S17(uint8_t address, uint8_t ss) {
  _address     = constrain(address, 0, 7);
  _ss          = ss;
  _modeCache   = 0xFFFF;                // Default I/O mode is all input, 0xFFFF
  _outputCache = 0x0000;                // Default output state is all off, 0x0000
  _inputCache  = 0x0000;                // Default all input pins to off
  _pullupCache = 0x0000;                // Default pull-up state is all off, 0x0000
  _invertCache = 0x0000;                // Default input inversion state is not inverted, 0x0000
  _interruptOnChangeCache = 0x0000;     // Default to no interrupts set up for any pin
  _interruptCompareToDefaultCache = 0x0000;  // Default all pins to interrupt on change
  _interruptCompareDefaultsCache = 0x0000;        // Default all defaults to compare to to 0
  _interruptLocalCompareToDefault = 0x0000;
  _interruptLocalCompareDefaults = 0x0000;
};

void MCP23S17::begin() {
  ::pinMode(_ss, OUTPUT);               // Set SlaveSelect pin as an output
  ::digitalWrite(_ss, HIGH);            // Set SlaveSelect HIGH (chip de-selected)
  SPI.begin();                          // Start up the SPI bus
  _spiSettings = SPISettings(10000000, MSBFIRST, SPI_MODE0); // Sets up SPI settings for later transmission use  
  byteWrite(IOCON, ADDR_ENABLE);
}

// GENERIC BYTE WRITE - will write a byte to a register, arguments are register address and the value to write

void MCP23S17::byteWrite(uint8_t reg, uint8_t value) { // Accept the register and byte
  _beginTransmission();
  SPI.transfer(OPCODEW | (_address << 1));             // Send the MCP23S17 opcode, chip address, and write bit
  SPI.transfer(reg);                                   // Send the register we want to write
  SPI.transfer(value);                                 // Send the byte
  _endTransmission();
}

// GENERIC WORD WRITE - will write a word to a register pair, LSB to first register, MSB to next higher value register 

void MCP23S17::wordWrite(uint8_t reg, uint16_t word) {  // Accept the start register and word 
  _beginTransmission();
  SPI.transfer(OPCODEW | (_address << 1));             // Send the MCP23S17 opcode, chip address, and write bit
  SPI.transfer(reg);                                   // Send the register we want to write 
  SPI.transfer((uint8_t)word);                      // Send the low byte (register address pointer will auto-increment after write)
  SPI.transfer((uint8_t) (word >> 8));                 // Shift the high byte down to the low byte location and send
  _endTransmission();
}

// MODE SETTING FUNCTIONS - BY PIN AND BY WORD

void MCP23S17::pinMode(uint8_t pin, bool mode) {  // Accept the pin # and I/O mode
  if (!_isValidPin(pin)) return;
  _toggleBit(_modeCache, pin - 1, mode == INPUT);
  wordWrite(IODIRA, _modeCache);                // Call the generic word writer with start register and the mode cache
}

void MCP23S17::pinMode(uint16_t mode) {     // Accept the word…
  wordWrite(IODIRA, mode);                // Call the the generic word writer with start register and the mode cache
  _modeCache = mode;
}

// THE FOLLOWING WRITE FUNCTIONS ARE NEARLY IDENTICAL TO THE FIRST AND ARE NOT INDIVIDUALLY COMMENTED

// WEAK PULL-UP SETTING FUNCTIONS - BY WORD AND BY PIN

void MCP23S17::pullupMode(uint8_t pin, bool pullUpEnabled) {
  if (!_isValidPin(pin)) return;
  _toggleBit(_pullupCache, pin, pullUpEnabled);
  wordWrite(GPPUA, _pullupCache);
}


void MCP23S17::pullupMode(uint16_t mode) { 
  wordWrite(GPPUA, mode);
  _pullupCache = mode;
}


// INPUT INVERSION SETTING FUNCTIONS - BY WORD AND BY PIN

void MCP23S17::inputInvert(uint8_t pin, bool invertEnabled) {
  if (!_isValidPin(pin)) return;
  _toggleBit(_invertCache, pin, invertEnabled);
  wordWrite(IPOLA, _invertCache);
}

void MCP23S17::inputInvert(uint16_t mode) { 
  wordWrite(IPOLA, mode);
  _invertCache = mode;
}

// INTERRUPT CONFIGURATION SETTING FUNCTIONS

void MCP23S17::interruptOnChange(uint8_t pin, bool enabled) {
  if (!_isValidPin(pin)) return;
  _toggleBit(_interruptOnChangeCache, pin, enabled);
  wordWrite(GPINTENA, _interruptOnChangeCache);
}

void MCP23S17::interruptOnChange(uint16_t enableds) {
  wordWrite(GPINTENA, enableds);
  _interruptOnChangeCache = enableds;
}

void MCP23S17::interruptCompareToDefault(uint8_t pin, bool enabled) {
  if (!_isValidPin(pin)) return;
  _toggleBit(_interruptCompareToDefaultCache, pin, enabled);
  wordWrite(INTCONA, _interruptCompareToDefaultCache);
}

void MCP23S17::interruptCompareToDefault(uint16_t enableds) {
  wordWrite(INTCONA, enableds);
  _interruptCompareToDefaultCache = enableds;
}

void MCP23S17::interruptSetDefault(uint8_t pin, bool value) {
  if (!_isValidPin(pin)) return;
  _toggleBit(_interruptCompareDefaultsCache, pin, value);
  wordWrite(DEFVALA, _interruptCompareDefaultsCache);
}

void MCP23S17::interruptSetDefault(uint16_t values) {
  wordWrite(DEFVALA, values);
  _interruptCompareDefaultsCache = values;
}


// WRITE FUNCTIONS - BY WORD AND BY PIN

void MCP23S17::digitalWrite(uint8_t pin, bool value) {
  if (!_isValidPin(pin)) return;
  _toggleBit(_outputCache, pin, value);
  wordWrite(GPIOA, _outputCache);
}

void MCP23S17::digitalWrite(uint16_t value) { 
  wordWrite(GPIOA, value);
  _outputCache = value;
}


// READ FUNCTIONS - BY WORD, BYTE AND BY PIN

uint16_t MCP23S17::digitalRead(void) {       // This function will read all 16 bits of I/O, and return them as a word in the format 0x(portB)(portA)
  uint16_t value = 0;                   // Initialize a variable to hold the read values to be returned
  _beginTransmission();
  SPI.transfer(OPCODER | (_address << 1));  // Send the MCP23S17 opcode, chip address, and read bit
  SPI.transfer(GPIOA);                      // Send the register we want to read
  value = SPI.transfer(0x00);               // Send any byte, the function will return the read value (register address pointer will auto-increment after write)
  value |= (SPI.transfer(0x00) << 8);       // Read in the "high byte" (portB) and shift it up to the high location and merge with the "low byte"
  _endTransmission();
  _inputCache = value;                      // Cache what we just read
  return value;                             // Return the constructed word, the format is 0x(portB)(portA)
}

uint8_t MCP23S17::byteRead(uint8_t reg) {        // This function will read a single register, and return it
  uint8_t value = 0;                        // Initialize a variable to hold the read values to be returned
  _beginTransmission();
  SPI.transfer(OPCODER | (_address << 1));  // Send the MCP23S17 opcode, chip address, and read bit
  SPI.transfer(reg);                        // Send the register we want to read
  value = SPI.transfer(0x00);               // Send any byte, the function will return the read value
  _endTransmission();

  // If we're reading the GPIO registers, we want to cache the values read
  if (reg == GPIOB) {
    _inputCache |= value << 8;
  } else if (reg == GPIOA) {
    _inputCache |= value;
  }

  return value;                             // Return the constructed word, the format is 0x(register value)
}

bool MCP23S17::digitalRead(uint8_t pin) {                    // Return a single bit value, supply the necessary bit (1-16)
  if (!_isValidPin(pin)) return false;                    // If the pin value is not valid (1-16) return, do nothing and return
  digitalRead();
  return digitalReadCache(pin);

}

uint16_t MCP23S17::digitalReadCache(void) {
  return _inputCache;
}

 bool MCP23S17::digitalReadCache(uint8_t pin) { // Return a single pin from last cache
  if (!_isValidPin(pin)) return false;
  return _getBit(_inputCache, pin);
}

// INTERRUPT PROCESSING

void MCP23S17::processInterrupt() {
  uint8_t pin = _getInterruptCausingPin();

  // Need to read the GPIO register to clear the interrupt
  uint16_t oldValues = _inputCache;
  uint16_t newValues = digitalRead();

  bool interruptOnAnyFlank = !_getBit(_interruptLocalCompareToDefault, pin);

  bool relevantOldBit = _getBit(oldValues, pin);
  bool relevantNewBit = _getBit(newValues, pin);

  if (relevantOldBit == relevantNewBit) {
    // Don't trigger over and over
    return;
  }

  if (interruptOnAnyFlank) {
    (*_interruptHandlers[pin - 1])();
    return;
  }  

  bool isFallingFlank = relevantNewBit;

  bool interruptOnFallingFlank = _getBit(_interruptLocalCompareDefaults, pin);

  if (isFallingFlank != interruptOnFallingFlank) {
    (*_interruptHandlers[pin - 1])();    
  }
}

void MCP23S17::attachInterrupt(uint8_t pin, InterruptHandler handler, int mode) {
  // Toggle this pin on for interrupts
  interruptOnChange(pin, true);

  // Unless this bit is switched to 1, interrupt will be triggered on any change
  // of this bit. So if we only care about either RISING or FALLING edges,
  // we want to compare to a given default value.
  // Hence: flip this bit to 1 if the mode is not "CHANGE".
  _toggleBit(_interruptLocalCompareToDefault, pin, mode != CHANGE);

  // If mode is CHANGE, the default value is not relevant so we don't bother
  // to update it in the port expander (not wasting clock cycles on pointless SPI)
  if (mode != CHANGE) {
    // Compare to a default value; if we want to interrupt on falling edge,
    // then we compare to 1, otherwise to 0
    _toggleBit(_interruptLocalCompareDefaults, pin, mode == FALLING);
  }

  _interruptHandlers[pin - 1] = handler;
}

void MCP23S17::detachInterrupt(uint8_t pin) {
  interruptOnChange(pin, false);
  _interruptHandlers[pin - 1] = 0; // probably illegal
}

// PRIVATE METHODS

void MCP23S17::_beginTransmission() {
  ::digitalWrite(_ss, LOW);
  SPI.beginTransaction(_spiSettings);
}

void MCP23S17::_endTransmission() {
  SPI.endTransaction();
  ::digitalWrite(_ss, HIGH);
}

bool MCP23S17::_isValidPin(uint8_t pin) {
  return pin > 0 && pin < 17;
}

void MCP23S17::_toggleBit(uint16_t &wordToEdit, uint8_t pinIndex, bool on) {
  if (on) {
    wordToEdit |= 1 << (pinIndex - 1);
  } else {
    wordToEdit &= ~(1 << (pinIndex - 1));
  }
}

bool MCP23S17::_getBit(uint16_t input, uint8_t pinIndex) {
  return (input & (0x1 << (pinIndex - 1)));
}

// Retrieves the 1-index pin that caused the last interrupt
// If this value is 0, it means no pin did
uint8_t MCP23S17::_getInterruptCausingPin() {
  uint16_t interruptFlags = 0;
  uint8_t interruptIndexOffset = 0;
  uint8_t interruptIndex = 0;

  _beginTransmission();
  
  SPI.transfer(OPCODER | (_address << 1));  // Send the MCP23S17 opcode, chip address, and read bit
  SPI.transfer(INTFA);                      // Send the register we want to read

  interruptFlags = SPI.transfer(0x00);

  if (interruptFlags == 0) { 
    // interrupt did not original from port A, read another byte to get port B
    interruptFlags = SPI.transfer(0x00); 

    // Since we know it's in port B, we can set this offset here so we don't have
    // to iterate more than 8 spots, slight optimization
    interruptIndexOffset = 8;
  }

  _endTransmission();

  if (interruptFlags == 0) {
    return 0;
  }

  do {
    if (interruptFlags & (1 << interruptIndex)) {
      return interruptIndex + interruptIndexOffset + 1;
    }
  } while (++interruptIndex < 8);

  // Well we should never get here but anyway
  return 0;
}