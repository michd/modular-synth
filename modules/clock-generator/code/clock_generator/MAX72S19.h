/*
 * MAX72S19.h
 *
 * Created: 17/11/2017 13:03:14
 *  Author: Mich
 */ 

#include <stdbool.h>
#include <avr/io.h>

#ifndef MAX72S19_H_
#define MAX72S19_H_

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

#define MAX_DIGITS      8

void displaySetup(uint8_t pinChipSelect, uint8_t pinDataOut, uint8_t pinClock);
void displaySetLED(uint8_t row, uint8_t column, bool on);
void displaySetRow(uint8_t row, uint8_t states);
void displaySetColumn(uint8_t column, uint8_t states);
void displayClear();
void displayWrite(uint8_t reg, uint8_t value);
void displayWriteChar(uint8_t digitIndex, char character, bool dotOn);
void displayWriteNumber(uint8_t digitIndex, uint8_t number);
void displayPrint(uint8_t startDigitIndex, char characters[]);
void displaySetDecodeMode(uint8_t modes);
void displaySetIntensity(uint8_t intensity);
void displaySetScanLimit(uint8_t scanLimit);
void displayStartDisplayTest();
void displayStopDisplayTest();
void displayShutdown();
void displayActivate();


#endif /* MAX72S19_H_ */