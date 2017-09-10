/*
  MCP23S17.h  Version 0.2
  Microchip MCP23S17 SPI I/O Expander Class for Arduino
  Created by Cort Buffington & Keith Neufeld
  March, 2011
  January, 2013
  January, 2015
  February, 2016

  Modifications by Michael Duerinckx (MichD)

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

         Code:

         Outside any method, instantiate the class:

         ```
         MCP23S17 PortExpander(address, chipSelectPin);
         ```

         in setup():

         ```
         PortExpander.begin();
         // Set up your pin modes and any other config, see public methods for details
         ```

         Using attachInterrupt:

         in your sketch's setup(), add the following:
         `attachInterrupt(INTERRUPT_PIN, processPortExpanderInterrupt, FALLING);`

         Add a fuction `processPortExpanderInterrupt` as follows:

         ```
         void processPortExpanderInterrup() {
            PortExpander.processInterrupt();
         }
         ```

         Then, also in setup(), you can attach interrupts to individual pins:
         ```
         PortExpander.attachInterrupt(pin, onSomeInputChange, RISING);
         ```

         and make sure to declare onSomeInputChange:
         ```
         void onSomeInputChange() {
            Called on a rising flank from that pin
         }
         ```
		 
  VERSION RELESE NOTES:
	    V0.2
	   	Changed direct manipulation of pin 10 on ATMega168/328 via "PORTB" to use digitalWrite on an arbitrary
        SlaveSelect pin passed to the object through the constructor
*/
 
#ifndef MCP23S17_h
#define MCP23S17_h

// REGISTERS ARE DEFINED HERE SO THAT THEY MAY BE USED IN THE MAIN PROGRAM

#define    IODIRA    (0x00)      // MCP23x17 I/O Direction Register
#define    IODIRB    (0x01)      // 1 = Input (default), 0 = Output

#define    IPOLA     (0x02)      // MCP23x17 Input Polarity Register
#define    IPOLB     (0x03)      // 0 = Normal (default)(low reads as 0), 1 = Inverted (low reads as 1)

#define    GPINTENA  (0x04)      // MCP23x17 Interrupt on Change Pin Assignements
#define    GPINTENB  (0x05)      // 0 = No Interrupt on Change (default), 1 = Interrupt on Change

#define    DEFVALA   (0x06)      // MCP23x17 Default Compare Register for Interrupt on Change
#define    DEFVALB   (0x07)      // Opposite of what is here will trigger an interrupt (default = 0)

#define    INTCONA   (0x08)      // MCP23x17 Interrupt on Change Control Register
#define    INTCONB   (0x09)      // 1 = pin is compared to DEFVAL, 0 = pin is compared to previous state (default)

#define    IOCON     (0x0A)      // MCP23x17 Configuration Register
//                   (0x0B)      //     Also Configuration Register

#define    GPPUA     (0x0C)      // MCP23x17 Weak Pull-Up Resistor Register
#define    GPPUB     (0x0D)      // INPUT ONLY: 0 = No Internal 100k Pull-Up (default) 1 = Internal 100k Pull-Up 

#define    INTFA     (0x0E)      // MCP23x17 Interrupt Flag Register
#define    INTFB     (0x0F)      // READ ONLY: 1 = This Pin Triggered the Interrupt

#define    INTCAPA   (0x10)      // MCP23x17 Interrupt Captured Value for Port Register
#define    INTCAPB   (0x11)      // READ ONLY: State of the Pin at the Time the Interrupt Occurred

#define    GPIOA     (0x12)      // MCP23x17 GPIO Port Register
#define    GPIOB     (0x13)      // Value on the Port - Writing Sets Bits in the Output Latch

#define    OLATA     (0x14)      // MCP23x17 Output Latch Register
#define    OLATB     (0x15)      // 1 = Latch High, 0 = Latch Low (default) Reading Returns Latch State, Not Port Value!

#include <Arduino.h>



class MCP23S17 {

  typedef void (*InterruptHandler)();
  
  public:
    // Constructor to instantiate a discrete IC as an object:
    // address 0-7
    // chipSelect pin
    MCP23S17(byte, byte);
    
    // Start the SPI Bus
    void begin();

    // Typically only used internally, but allows the user to write any register pair if needed, so it's public
    void wordWrite(byte, word);
    
    // Typically only used internally, but allows the user to write any register if needed, so it's public
    void byteWrite(byte, byte);
    
    // Sets the mode (input or output) of a single I/O pin
    // Pin number, input (1) / output (0)
    void pinMode(byte, bool);
    
    // Sets the mode (input or output) of all I/O pins at once 
    void pinMode(word);
    
    // Selects internal 100k input pull-up of a single I/O pin
    // Pin number, pull-up enabled (1) / disabled (0)
    void pullupMode(byte, bool);
    
    // Selects internal 100k input pull-up of all I/O pins at once
    void pullupMode(word);
    
    // Selects input state inversion of a single I/O pin (writing 1 turns on inversion) 
    // Pin number, invert enabled (1) / disabled (0)
    void inputInvert(byte, bool);
    
    // Selects input state inversion of all I/O pins at once (writing a 1 turns on inversion)
    void inputInvert(word);
    
    // Sets a single pin to trigger interrupts when it changes
    void interruptOnChange(byte, bool);

    // Configures which pins should trigger an interrupt when changed
    void interruptOnChange(word);

    // Configures for a single pin whether to compare to default value or just change
    // to trigger an interrupt
    void interruptCompareToDefault(byte, bool);

    // Configures which pins should compare to default value or just change
    // to trigger an interrupt
    void interruptCompareToDefault(word);

    // Sets the default to compare to to trigger an interrupt, for a given pin
    void interruptSetDefault(byte, bool);

    // Sets all the defaults to compare to trigger interrupts in one fell swoop
    void interruptSetDefault(word);
    
    // Sets an individual output pin HIGH or LOW
    void digitalWrite(byte, bool);
    
    // Sets all output pins at once. If some pins are configured as input, those bits will be ignored on write
    void digitalWrite(word);
    
    // Reads an individual input pin
    bool digitalRead(byte);
    
    // Reads an individual register and returns the byte. Argument is the register address
    byte byteRead(byte);
    
    // Reads all input pins at once. Be sure it ignore the value of pins configured as output!
    word digitalRead(void);
    
    // Return an individual input pin from cached value - not using SPI to read up to date value
    bool digitalReadCache(byte);
    
    // Return all input pins at once from cache
    word digitalReadCache(void);

    // Attach an interrupt handler to a pin, using same modes are on Arduino's version
    // pin number, handler function, CHANGE / FALLING / RISING
    void attachInterrupt(byte, InterruptHandler, int mode);

    // Remove an interrupt handler from a pin
    void detachInterrupt(byte);

    // Handle interrupt from main program
    void processInterrupt(void);
  
  private:
    // 3-bit address of the MCP23S17 in use
    byte _address;
	
    // Slave-select pin
    byte _ss;
  
    // Settings to use for SPI transmissions, initialized in begin()
    SPISettings _spiSettings;
  
    // Caches the mode (input/output) configuration of I/O pins
    word _modeCache;
  
    // Caches the internal pull-up configuration of input pins (values persist across mode changes)
    word _pullupCache;
  
    // Caches the input pin inversion selection (values persist across mode changes)
    word _invertCache;

    // Caches the interrupt-on-change selections
    word _interruptOnChangeCache;

    // Caches the interrupt comparison mode selections
    // If a bit here is 1, it compares to the default values rather than on change
    word _interruptCompareToDefaultCache;

    // Caches the default values compared to for triggering interrupts
    // These are only relevant for pins where we're comparing to default value
    word _interruptCompareDefaultsCache;

    // The MCP23S17 functionality for compare to default is a bit undesirable
    // in that it keeps the interrupt active until conditions change, rather
    // than just on change, so we're doing what it's meant to do, locally.
    // Each bit here indicates whether to compare whether we've changed values,
    // and whether that's a change from the default
    word _interruptLocalCompareToDefault;

    word _interruptLocalCompareDefaults;
  
    // Caches the output pin state of pins
    word _outputCache;
  
    // Caches input pins whenever they are read, so they can be retrieved from cache directly
    word _inputCache;

    // Function pointers to interrupt handlers for each pin
    InterruptHandler _interruptHandlers[16];
  
    // Internal helpers to start/end transmission
    void _beginTransmission();               
    void _endTransmission();

    // Helper to check whether pin is in range
    bool _isValidPin(byte);

    // Helper for common action of setting one bit in a word
    // Input word, 1-indexed bit, new value for that bit
    // variable is passed by reference, this will modify the given variable
    void _toggleBit(word &, byte, bool);

    // Returns the state of a single bit from a word
    // bit is 1-indexed
    bool _getBit(word, byte);

    // Helper for _interruptHandler, determines 1-index pin number that caused interrupt
    // With some nice optimizations, like not bothering to request the seconds
    // (B) register if the A register was non-zero
    byte _getInterruptCausingPin(void);
 };
		
#endif //MCP23S17
