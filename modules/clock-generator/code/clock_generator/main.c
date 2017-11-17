#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

// Needs to be defined before including util/delay, or it will overwrite its own
#define F_CPU 16000000UL
#include <util/delay.h>

#include "MAX72S19.h"

#define _BV(bit) (1 << (bit))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define PPQ 24 // pulses per quarter note, from MIDI

#define PIN_LED_OUT PINA0
#define PIN_PPQ_OUT PINA5

// Serial interface to MAX72S19
#define PIN_DISPLAY_CLK PINA4
#define PIN_DISPLAY_DATA_OUT PINA6
#define PIN_DISPLAY_CS PINA3

#define PIN_BUTTON_UP PINA2
#define PIN_BUTTON_DOWN PINA1

#define MIN_TEMPO 20
#define DEFAULT_TEMPO 120
#define MAX_TEMPO 300

#define MAIN_LOOP_TICK_TIME_MS 50
#define DEBOUNCE_TICKS 1

#define TICKS_TIL_REPEAT 10
#define TICKS_TIL_FAST_REPEAT 50

#define TEMPO_SAVE_DELAY_TICKS 600 // 30 seconds
#define TEMPO_EEPROM_ADDRESS 0

volatile long pulseCount = 0;

// Main loop ticks
bool evenTick;

volatile bool ledOn;
volatile bool ppqOn = true;

volatile unsigned short int tempo = 0;
volatile unsigned short int eepromTempo;

volatile uint8_t portACache;

volatile int buttonUpTicksHeld;
volatile int buttonDownTicksHeld;

// Used for debounce timekeeping and "button held" timekeeping
volatile unsigned long mainLoopTicks;

// To ensure we're not constantly writing the EEPROM
volatile unsigned long tempoLastUpdated;

// Debouncing buttons
volatile unsigned long debounceLastPressed;

void internalTimerTick();
unsigned long calcFreqForTimer(int);
void configureTimerForFrequency(unsigned long);
void displayNumber(int);
void scanButtons(bool);
unsigned short int loadTempo();
void saveTempo();
void updateTempo(unsigned short int);

char* itoa(int, char[]);
char* pad(char[], uint8_t);
void buttonUpPressed();
void buttonDownPressed();
bool debounce();

// Handle button press interrupts
// Data sheet table 9.1, PCINT0_vect is the vector for pin change interrupts 0 (port A)
ISR(PCINT0_vect) {
	for (uint8_t i = 0; i < 8; i++) {
		if ((PINA & _BV(i)) != (portACache & _BV(i))) {
			portACache = PINA;
			bool direction = !!(PINA & _BV(i));	
			// only care about down flanks
			// TODO: better readable system to handle all these
			if (direction) {
				switch (i) {
					case PIN_BUTTON_UP: buttonUpTicksHeld = 0; break;// buttonUpPressed();
					case PIN_BUTTON_DOWN: buttonDownTicksHeld = 0; break;return buttonDownPressed();
				}	
				return;
			}
			
			switch (i) {
				case PIN_BUTTON_UP: return buttonUpPressed();
				case PIN_BUTTON_DOWN: return buttonDownPressed();
			}
			
			break;
		}
	}
	
	portACache = PINA;
}

ISR(TIM1_COMPA_vect) {
	internalTimerTick();
}

int main(void)
{
	// Configure outputs: PIN_LED, PIN_PPQ
	DDRA |= _BV(PIN_LED_OUT) | _BV(PIN_PPQ_OUT);
	
	// Configure inputs: PIN_BUTTON_DOWN, PIN_BUTTON_UP
	DDRA &= ~(_BV(PIN_BUTTON_DOWN) | _BV(PIN_BUTTON_UP));
	// Set input puts as pull-ups
	PORTA |= (_BV(PIN_BUTTON_DOWN) | _BV(PIN_BUTTON_UP));
	
	portACache = PINA;
	
	// Pin change interrupts
	// Data sheet section 9.3.2
	// using pins in port A for interrupts, so setting bit PCIE0
	GIMSK |= _BV(PCIE0);
	// Data sheet section 9.3.5
	// Note: interrupt numbers happen to match the pin numbers on the ATtiny84
	// If porting to a different chip, ensure you have the right bits to map to
	// port pins
	PCMSK0 |= _BV(PIN_BUTTON_UP) | _BV(PIN_BUTTON_DOWN);
	
	// Configure the display
	displaySetup(PIN_DISPLAY_CS, PIN_DISPLAY_DATA_OUT, PIN_DISPLAY_CLK);
	displaySetDecodeMode(0x00);
	displaySetIntensity(0xF);
	displaySetScanLimit(3);
	displayClear();
	displayActivate();
	
	// Timer interrupt setup
	// Set up CTC mode (clear on compare)
	// This ensures the counter register gets cleared (reset to 0) when it matches the compare register
	TCCR1B |= _BV(WGM12);
	// Enable interrupt on comparison timer 1, comparison A
	TIMSK1 |= _BV(OCIE1A);
	
	// globally enable interrupts
	sei();
	
	updateTempo(loadTempo());
	
	while (1)
	{
		mainLoopTicks++;
		evenTick = !evenTick;
		scanButtons(evenTick);
		if (evenTick) saveTempo();
		_delay_ms(MAIN_LOOP_TICK_TIME_MS);
	}
}

// Tempo Read/write/update

unsigned short int loadTempo() {
	eepromTempo = eeprom_read_word((uint16_t*)TEMPO_EEPROM_ADDRESS);
	
	if (eepromTempo < MIN_TEMPO || eepromTempo > MAX_TEMPO) eepromTempo = DEFAULT_TEMPO;
	return eepromTempo;
}

// Saves tempo to eeprom
void saveTempo() {	
	if (tempo == eepromTempo) return;
	if (mainLoopTicks - tempoLastUpdated < TEMPO_SAVE_DELAY_TICKS) return;
	eepromTempo = tempo;
	eeprom_write_word((uint16_t*)TEMPO_EEPROM_ADDRESS, tempo);
}

void updateTempo(unsigned short int newTempo) {
	newTempo = constrain(newTempo, MIN_TEMPO, MAX_TEMPO);
	
	if (tempo == newTempo) return;
	
	tempo = newTempo;
	
	tempoLastUpdated = mainLoopTicks;
	
	configureTimerForFrequency(calcFreqForTimer(tempo));
	
	displayNumber(tempo);
}

// Button handling

void buttonUpPressed() {
	// Get rid of hardware jitter
	if (debounce()) return;
	
	// If we were already holding the button, don't have interrupts cause
	// further tempo updates, the scanButtons() loop will take care of
	// repeating normally
	if (!!buttonUpTicksHeld) return;
	updateTempo(tempo + 1);
}

void buttonDownPressed() {
	if (debounce()) return;
	if (!!buttonDownTicksHeld) return;
	updateTempo(tempo - 1);
}

// Handles buttons being held, for automatic repeating of moving tempo up/down,
// or resettings to default
void scanButtons(bool even) {
	bool upPressed = !(PINA & _BV(PIN_BUTTON_UP));
	bool downPressed = !(PINA & _BV(PIN_BUTTON_DOWN));
	
	// If neither are held, reset how long we've held them
	if (!upPressed) buttonUpTicksHeld = 0;
	if (!downPressed) buttonDownTicksHeld = 0;
	
	// No buttons pressed? Nothing left to do
	if (!(upPressed || downPressed)) return;
	
	// If both buttons are pressed, and they've been held long enough,
	// reset to default. Increment baked into the condition
	if (upPressed && downPressed) {
		if (++buttonUpTicksHeld >= TICKS_TIL_REPEAT &&
		++buttonDownTicksHeld >= TICKS_TIL_REPEAT) {
			buttonUpTicksHeld = 0;
			buttonDownTicksHeld = 0;
			updateTempo(DEFAULT_TEMPO);
		}
		
		return;
	}
	
	// If up is held, determine how fast to update etc
	if (upPressed) {
		++buttonUpTicksHeld;
		if (buttonUpTicksHeld >= TICKS_TIL_FAST_REPEAT) {
			updateTempo(tempo + 1);
			buttonUpTicksHeld = TICKS_TIL_FAST_REPEAT;
		} else if (buttonUpTicksHeld >= TICKS_TIL_REPEAT && even) {
			updateTempo(tempo + 1);
		}
	}
	
	if (downPressed) {
		++buttonDownTicksHeld;
		if (buttonDownTicksHeld >= TICKS_TIL_FAST_REPEAT) {
			updateTempo(tempo - 1);
			buttonDownTicksHeld = TICKS_TIL_FAST_REPEAT;
		} else if (buttonDownTicksHeld >= TICKS_TIL_REPEAT && even) {
			updateTempo(tempo - 1);
		}
	}
}

// uC Timer configuration / handling

unsigned long calcFreqForTimer(int bpm) {
	// Times 2 so we have double the frequency of PPQ pulses
	// Allow to toggle the PPQ output on and off
	// 60 as in seconds to a minute
	return (unsigned long)((float)(bpm * PPQ * 2) / (float)60);
}


// Change pre-scaler and compare value for achieving the frequency we want
void configureTimerForFrequency(unsigned long frequency) {
	unsigned char clockSelectBits = 0;
	const long resolution = 0x10000;
	long cycles = F_CPU / frequency;
	
	// Taken from TimerOne source
	if (cycles < resolution) {
		// No pre-scale, full clock speed
		clockSelectBits = _BV(CS10);
	} else if((cycles >>= 3) < resolution)  {
		// Pre-scale by /8
		clockSelectBits = _BV(CS11);
	} else if((cycles >>= 3) < resolution) {
		// Pre-scale byte /64
		clockSelectBits = _BV(CS11) | _BV(CS10);
	} else if((cycles >>= 2) < resolution) {
		// Pre-scale by /256
		clockSelectBits = _BV(CS12);
	} else if((cycles >>= 2) < resolution) {
		// Pre-scale by /1024
		clockSelectBits = _BV(CS12) | _BV(CS10);
	} else {
		// Out of bounds, set to maximum pre-scale and cycles.
		cycles = resolution,
		clockSelectBits = _BV(CS12) | _BV(CS10);
	}
	
	// Backup interrupt settings register
	unsigned char oldSREG = SREG;
	
	// Disable interrupts while we update counter-related registers
	cli();
	// Clear pre-scaling-related bits before rewriting them
	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
	TCCR1B |= clockSelectBits;
	// Set the compare value
	OCR1A = cycles - 1;
	// Re-enable interrupts by restoring interrupt setting register
	SREG = oldSREG;
}

// Timer interrupt triggered
void internalTimerTick() {
	// Toggle PPQ output
	ppqOn = !ppqOn;
	
	if (ppqOn) PORTA |= _BV(PIN_PPQ_OUT);
	else PORTA &= ~_BV(PIN_PPQ_OUT);
	
	// If we're not on the on part of the cycle, don't bother with divided output
	if (!ppqOn) return;

	// Increment pulse counter for secondary output that only happens every 4th note
	pulseCount++;
	
	if (pulseCount >= PPQ / 2) {
		pulseCount = 0;
		ledOn = !ledOn;
		if (ledOn) PORTA |= _BV(PIN_LED_OUT);
		else PORTA &= ~_BV(PIN_LED_OUT);
	}
}

// Display helpers

void displayNumber(int number) {
	char str[4];
	itoa(number, str);
	pad(str, 3);
	displayPrint(0, str);
}

// Convert an integer to a character array
char* itoa(int i, char b[]){
	char const digit[] = "0123456789";
	
	char* p = b;
	int shifter = i;
	
	// Count how many chars we'll need to represent this number;
	// advancing the pointer to where the null terminator goes
	do {
		++p;
		shifter /= 10;
	} while(shifter);
	
	*p = '\0';
	
	// Iterate over the digits, starting with the least significant digit
	// Grab char for the digit
	
	do {
		*--p = digit[i % 10];
		i /= 10;
	} while(i);
	
	return b;
}

// Pad start of string with spaces to achieve given length
char* pad(char in[], uint8_t len) {
	in[len] = '\0';
	uint8_t existingLen = strlen(in);
	uint8_t diff = len - existingLen;
	
	uint8_t i = len - 1;
	
	do {
		in[i] = i >= diff ? in[i - diff] : ' ';
	} while (i-- > 0);
	return in;
}

// Button helpers

bool debounce() {
	unsigned long now = mainLoopTicks;

	if (now - debounceLastPressed >= DEBOUNCE_TICKS) {
		debounceLastPressed = now;
		return false;
	}
	
	return true;
}
