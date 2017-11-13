#include "MAX72S19.h"
#include <EEPROM.h>

#define PIN_DISPLAY_CLK 4
#define PIN_DISPLAY_DATA_OUT 6
#define PIN_DISPLAY_CS 3

#define PIN_BUTTON_UP 2
#define PIN_BUTTON_DOWN 1

#define PIN_SECONDARY_OUT 0
#define PIN_PPQ_OUT 7

#define MIN_TEMPO 40
#define MAX_TEMPO 300
#define DEFAULT_TEMPO 120

#define TEMPO_SAVE_DELAY_MS 30000
#define TEMPO_EEPROM_ADDRESS 0

#define DEBOUNCE_MS 100
#define TICKS_TIL_REPEAT 10
#define TICKS_TIL_FAST_REPEAT 60

#define PPQ 24 // (MIDI, pulses per quarter note)

#define SECONDARY_PULSE_OUT_PULSES 6 // 16th notes

#define isPressed(pin) !digitalRead(pin)

MAX72S19 display(PIN_DISPLAY_CS, PIN_DISPLAY_DATA_OUT, PIN_DISPLAY_CLK);
volatile int tempo;
int eepromTempo;
volatile byte portACache;
volatile long debounceLastPressed;

volatile long tempoLastUpdated;

volatile int buttonUpTicksHeld;
volatile int buttonDownTicksHeld;

volatile bool firstHalfOfPpqTick;
bool evenTick;

volatile int pulses;

// Handle button press interrupts
// Data sheet table 9.1, PCINT0_vect is the vector for pin change interrupts 0 (port A)
ISR(PCINT0_vect) {	
	for (byte i = 0; i < 8; i++) {
		bool direction = PINA & (1 << i);
		if ((PINA & (1 << i)) != (portACache & (1 << i))) {
			portACache = PINA;
			
			// only care about down flanks
			// TODO: better readable system to handle all these
			if (direction == 1) return;
			
			switch (i) {
				case PIN_BUTTON_UP: return buttonUpPressed();
				case PIN_BUTTON_DOWN: return buttonDownPressed();
			}		
			
			break;
		}
	}
	
	portACache = PINA;
}

void setup() {	
  pinMode(PIN_SECONDARY_OUT, OUTPUT);
  pinMode(PIN_PPQ_OUT, OUTPUT);
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  portACache = PINA; 
  
  // Data sheet section 9.3.2
  // using pins in port A for interrupts, so setting bit PCIE0
  GIMSK |= _BV(PCIE0);
  // Data sheet section 9.3.5
  // PCINT0 again for port A, and using pins_arduino.h to set map the right pin
  PCMSK0 |= _BV(digitalPinToPCMSKbit(PIN_BUTTON_UP)) | _BV(digitalPinToPCMSKbit(PIN_BUTTON_DOWN));
  
  sei();
  
  display.begin();
  display.setDecodeMode(0x00);
  display.setIntensity(0xF);
  display.setScanLimit(3);
  display.clear();
  display.activate();
  delay(200);
  
  int _tempo = loadTempo();  
  updateTempo(_tempo);
}

// TODO: write some method that uses this information to set up the timer and all
long calcTimerPeriod(int bpm) {
	return round(((double)1000000 / (double)((bpm * PPQ) / (double)60)) / (double)2);
}

void internalTimerTick() {
	firstHalfOfPpqTick = !firstHalfOfPpqTick;
	digitalWrite(PIN_PPQ_OUT, firstHalfOfPpqTick);
	
	if (!firstHalfOfPpqTick) return;
	pulses++;
	if (pulses == SECONDARY_PULSE_OUT_PULSES) pulses == 0;
	if (pulses > 1) return;
	digitalWrite(PIN_SECONDARY_OUT, pulses == 0);
}

void loop() {
	evenTick = !evenTick;
	scanButtons(evenTick);
	
	if (evenTick) saveTempo();
	
	delay(50);
}

int loadTempo() {
	eepromTempo = EEPROM.read(TEMPO_EEPROM_ADDRESS)
				| (EEPROM.read(TEMPO_EEPROM_ADDRESS + 1) << 8);
				
	if (eepromTempo < MIN_TEMPO || eepromTempo > MAX_TEMPO) eepromTempo = DEFAULT_TEMPO;
	return eepromTempo;
}

void saveTempo() {
	if (tempo == eepromTempo) return;
	if (millis() - tempoLastUpdated < TEMPO_SAVE_DELAY_MS) return;
	eepromTempo = tempo;
	EEPROM.write(TEMPO_EEPROM_ADDRESS, eepromTempo & 0xFF);
	EEPROM.write(TEMPO_EEPROM_ADDRESS + 1, (eepromTempo >> 8) & 0xFF);
}

// Deals with scanning the up/down buttons in the loop
// Behavior:
// - If a button is held for TICKS_TIL_REPEAT, tempo updates start getting
//   repeated every other tick.
// - If a button is still held after TICKS_TIL_FAST_REPEAT, tempo updates
//   repeat every tick.
// - If both buttons are held down simultaneous for for at least TICKS_TIL_REPEAT, the tempo is reset to default.
// - If a button is no longer being held, its tick counter gets reset to 0.
void scanButtons(bool even) {
	bool upPressed = isPressed(PIN_BUTTON_UP);
	bool downPressed = isPressed(PIN_BUTTON_DOWN);
	
	if (!upPressed) buttonUpTicksHeld = 0;
	if (!downPressed) buttonDownTicksHeld = 0;
	
	if (!(upPressed || downPressed)) return;
	
	if (upPressed && downPressed) {
		if (++buttonUpTicksHeld >= TICKS_TIL_REPEAT &&
		    ++buttonDownTicksHeld >= TICKS_TIL_REPEAT) {
				buttonUpTicksHeld = 0;
				buttonDownTicksHeld = 0;
				updateTempo(DEFAULT_TEMPO);		
		}		
		
		return;
	}
	
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

void updateTempo(int newTempo) {
	newTempo = constrain(newTempo, MIN_TEMPO, MAX_TEMPO);
	
	if (tempo == newTempo) return;
	
	tempo = newTempo;
	
	tempoLastUpdated = millis();
	displayNumber(tempo);
	// TODO: update timer setup
}

void displayNumber(int number) {
	char str[4];
	itoa(number, str);
	pad(str, 3);
	display.print(0, str);
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
char* pad(char in[], byte len) {
	in[len] = '\0';
	byte existingLen = strlen(in);
	byte diff = len - existingLen;
	
	byte i = len - 1;
	
	do {
		in[i] = i >= diff ? in[i - diff] : ' ';
	} while (i-- > 0);
	return in;
}

void buttonUpPressed() {
	// Get rid of hardware jitter
	if (debounce()) return;
	
	// If we were already holding the button, don't have interrupts cause
	// further tempo updates, the scanButtons() loop will take care of
	// repeating normally
	if (buttonUpTicksHeld) return;
	updateTempo(tempo + 1);
}

void buttonDownPressed() {
	if (debounce()) return;
	if (buttonDownTicksHeld) return;
	updateTempo(tempo - 1);
}

bool debounce() {	
	long now = millis();
	
	if (now - debounceLastPressed > DEBOUNCE_MS) {
		debounceLastPressed = now;
		return 	false;
	}
	
	return true;
}