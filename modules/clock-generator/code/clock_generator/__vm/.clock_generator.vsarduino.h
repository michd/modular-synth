/* 
	Editor: http://www.visualmicro.com
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: ATtiny24/44/84 w/ ATtiny84, Platform=avr, Package=attiny
*/

#define __AVR_ATtiny84__
#define __AVR_ATTINY84__
#define ARDUINO 10801
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU {build.f_cpu}
#define ARDUINO 10801
#define ARDUINO_attiny
#define ARDUINO_ARCH_AVR
//
long calcTimerPeriod(int bpm);
void internalTimerTick();
//
int loadTempo();
void saveTempo();
void scanButtons(bool even);
void updateTempo(int newTempo);
void displayNumber(int number);
char* itoa(int i, char b[]);
char* pad(char in[], byte len);
void buttonUpPressed();
void buttonDownPressed();
bool debounce();

#include "pins_arduino.h" 
#include "Arduino.h"
#include "clock_generator.ino"
