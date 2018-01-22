/*
 * MAX72S19 8x8 matrix / 8 digit 7 segment display driver library
 * By MichD
 */

#include <Arduino.h>
#include <SPI.h>
#include <String.h>
#include <stdint.h>

#ifndef MAX72S19_h // Ensure we're not double-defining the library if it was already included
#define MAX72S19_h

// Register addresses as defined in datasheet

#define REG_NOOP        0x00
#define REG_DIGIT0      0x01
#define REG_DIGIT1      0x02
#define REG_DIGIT2      0x03
#define REG_DIGIT3      0x04
#define REG_DIGIT4      0x05
#define REG_DIGIT5      0x06
#define REG_DIGIT6      0x07
#define REG_DIGIT7      0x08
#define REG_DECODEMODE  0x09
#define REG_INTENSITY   0x0A
#define REG_SCANLIMIT   0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPLAYTEST 0x0F

#define MIN_DISPLAY_REG REG_DIGIT0
#define MAX_DISPLAY_REG REG_DIGIT7

#define MAX_DIGITS      8
#define MAX_ROWS        MAX_DIGITS
#define NUM_SEGMENTS    8
#define MAX_COLUMNS     NUM_SEGMENTS

class MAX72S19 {
  public:
    MAX72S19(uint8_t); // Constructor, takes chip select pin
    void begin();      // Start the SPI bug

    // Set a specific LED on or off
    // column (0-7), row (0,7), on/off
    void setLED(uint8_t, uint8_t, bool);

    // Set the values for a row of LEDs
    // row index (0-7)
    // Bits for each LED on the row
    void setRow(uint8_t, uint8_t);

    // Set the values for a column of LEDs
    // colum index (0-7)
    // Bits for each LED on the column
    void setColumn(uint8_t, uint8_t);

    // Clears the entire display (turn all LEDs off)
    void clear();

    // direct raw write
    void write(uint8_t, uint8_t);

    // Writes a character to a given digit
    // digit index (0-7)
    // Character to write (A-Z, 0-9)
    // Dot on/off
    // Note: not all characters in the latin alphabet can be
    // properly displayed on a 7 segment display.
    void writeChar(uint8_t, char, bool);

    // Shorthand for writeChar without a dot.
    void writeChar(uint8_t, char);

    // Writes a single digit number to a given digit index
    void writeNumber(uint8_t, uint8_t);

    // Writes a character array starting from a given digit index
    // Starting index (0-7)
    // Array of characters to write (A-Z, 0-9)
    // This will stop when end of String is detected,
    // or when we run of out digits, whichever comes first
    // Note: not all characters in the latin alphabet can be
    // properly displayed on a 7 segment display.
    void print(uint8_t, String);

    // Sets whether to use the builtin decoder for each digit
    void setDecodeMode(uint8_t);

    // Sets the intensity of the display, in a range of 0-15
    // 0 is not off, it is minimum intensity.
    void setIntensity(uint8_t);

    // Sets how many digits are being used
    // If you're only using, say, 4 digits, setting the scan limit to 4
    // will improve brightness of those digits as the multiplexer doesn't
    // iterate over the unused digits.
    void setScanLimit(uint8_t);

    // Overrides all other settings and turns on all LEDs to full brightness
    void startDisplayTest();

    // End display test mode, reverting back to other settings and data
    void stopDisplayTest();

    void shutdown();

    void activate();

  private:
    uint8_t _displayCache[MAX_DIGITS];
    uint8_t _pinChipSelect;
    SPISettings _spiSettings;
    void _beginTransmission();
    void _endTransmission();
    void _transmitSetRegister(uint8_t, uint8_t);
    void _setRegister(uint8_t, uint8_t);
    uint8_t _mapChar(char);
};

#endif // MAX72S19_h