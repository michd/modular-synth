#include "MAX72S19.h"

MAX72S19::MAX72S19(uint8_t pinChipSelect) {
  _pinChipSelect = pinChipSelect;
}

void MAX72S19::begin() {
  ::pinMode(_pinChipSelect, OUTPUT);
  ::digitalWrite(_pinChipSelect, 1);
  SPI.begin();
  _spiSettings = SPISettings(10000000, MSBFIRST, SPI_MODE2);
}

void MAX72S19::setLED(uint8_t row, uint8_t column, bool on) {
  // TODO
}

void MAX72S19::setRow(uint8_t row, uint8_t states) {
  // TODO
}

void MAX72S19::setColumn(uint8_t column, uint8_t states) {
  // TODO
}

void MAX72S19::clear() {
  for (uint8_t i = 0; i < MAX_DIGITS; i++) {
    _setRegister(REG_DIGIT0 + i, 0x00);
  }
}

void MAX72S19::write(uint8_t reg, uint8_t value) {
  _setRegister(reg, value);
}

void MAX72S19::writeChar(uint8_t digitIndex, char character, bool dotOn) {
  digitIndex = constrain(digitIndex, 0, 7);
  uint8_t value = _mapChar(character);

  if (dotOn) {
    value |= B10000000;
  }

  _setRegister(REG_DIGIT0 + digitIndex, value);
}

void MAX72S19::writeChar(uint8_t digitIndex, char character) {
  writeChar(digitIndex, character, false);
}

void MAX72S19::writeNumber(uint8_t digitIndex, uint8_t number) {
  number = constrain(number, 0, 9);
  writeChar(digitIndex, '0' + number);
}

void MAX72S19::print(uint8_t startDigitIndex, char characters[]) {
  uint8_t curDigit = constrain(startDigitIndex, 0, 7);
  uint8_t charIndex = 0;
  char curChar = characters[charIndex];

  while (curDigit < MAX_DIGITS * 2) {
    if (curChar == '\0') break;
    bool curCharDot = characters[charIndex + 1] == '.';
    writeChar(curDigit++, curChar, curCharDot);
    if (curCharDot) ++charIndex;
    curChar = characters[++charIndex];
  }
}

void MAX72S19::setDecodeMode(uint8_t modes) {
  _setRegister(REG_DECODEMODE, modes);
}

void MAX72S19::setIntensity(uint8_t intensity) {
  _setRegister(REG_INTENSITY, constrain(intensity, 0x0, 0xF));
}

void MAX72S19::setScanLimit(uint8_t scanLimit) {
  _setRegister(REG_SCANLIMIT, constrain(scanLimit, 0x0, 0xF));
}

void MAX72S19::startDisplayTest() {
  _setRegister(REG_DISPLAYTEST, 1);
}

void MAX72S19::stopDisplayTest() {
  _setRegister(REG_DISPLAYTEST, 0);
}

void MAX72S19::shutdown() {
  _setRegister(REG_SHUTDOWN, 0);
}

void MAX72S19::activate() {
  _setRegister(REG_SHUTDOWN, 1);
}

// Private method

uint8_t MAX72S19::_mapChar(char inputChar) {
  switch (inputChar) {
    //  Segments legend:
    //          _______
    //        /   A   / 
    //     F /       / B
    //      /_______/ 
    //     /   G   /
    //  E /       / C
    //   /_______/ * dp
    //       D

    //               dpABCDEFG
    case '0': return B01111110;
    case '1': return B00110000;
    case '2': return B01101101;
    case '3': return B01111001;
    case '4': return B00110011;
    case '5': return B01011011;
    case '6': return B01011111;
    case '7': return B01110000;
    case '8': return B01111111;
    case '9': return B01111011;
    case 'a':
    case 'A': return B01110111;
    case 'b':
    case 'B': return B00011111;
    case 'c': return B00001101;
    case 'C': return B01001110;
    case 'd':
    case 'D': return B00111101;
    case 'e':
    case 'E': return B01001111;
    case 'f':
    case 'F': return B01000111;
    case 'g':
    case 'G': return B01011110;
    case 'h':
    case 'H': return B00010111;
    case 'i': return B00000100;
    case 'I': return B00000110;
    case 'j': return B00011000;
    case 'J': return B00111000;
    case 'k':
    case 'K': return B00000111;
    case 'l':
    case 'L': return B00001110;
    case 'm':
    case 'M': return B01110110;
    case 'n':
    case 'N': return B00010101;
    case 'o': return B00011101;
    case 'O': return B01111110;
    case 'p':
    case 'P': return B01100111;
    case 'q':
    case 'Q': return B01110011;
    case 'r': return B00000101;
    case 'R': return B10000110;
    case 's':
    case 'S': return B01011011;
    case 't':
    case 'T': return B00001111;
    case 'u':
    case 'U': return B00111110;
    case 'v':
    case 'V': return B00011100;
    case 'w':
    case 'W': return B00101010;
    case 'x':
    case 'X': return B00110111;
    case 'y':
    case 'Y': return B00111011;
    case 'z':
    case 'Z': return B01101001;
    case '-': return B00000001;
    case '_': return B00001000;
    case '.': return B10000000;
    case '/': return B00100100;
    case ' ':
    default:  return B00000000;
  }
}

void MAX72S19::_beginTransmission() {
  ::digitalWrite(_pinChipSelect, 0);
  SPI.beginTransaction(_spiSettings);
}

void MAX72S19::_endTransmission() {
  SPI.endTransaction();
  ::digitalWrite(_pinChipSelect, 1);
}

void MAX72S19::_setRegister(uint8_t reg, uint8_t data) {
  _beginTransmission();
  SPI.transfer(reg);
  SPI.transfer(data);
  _endTransmission();
}
