// TODO: listening to note on/off events based on channel
// TODO: outputting PWM for frequency control voltage
// TODO: listening to for control param changes, output 4 of them via PWM
#include <SoftwareSerial.h>
#include <EEPROM.h>

// Constant values
// MIDI standard, don't anticipate these will ever change
#define MIN_MIDI_BYTES 3
#define MIDI_SERIAL_BAUD_RATE 31250
#define MIDI_CHANNELS 16
// Wait this long before accepting another button press
#define BUTTON_DEBOUNCE_MS 150
// Allows shift registers to reset on power-up
#define POWERUP_MS 200
// Where in the EEPROM we store the selected channel
#define MIDI_CHANNEL_EEPROM_ADDRESS 0

// Output pins
#define PIN_GATE 7
#define PIN_TRIGGER 8
// 7 Segment-related
#define PIN_LATCH 3
#define PIN_CLK 4
#define PIN_DATA 5
// Not actually used to send anything
#define PIN_SOFTWARE_TX 14

// Input pins
// MIDI serial in
#define PIN_SOFTWARE_RX 15
// Channel select button
#define PIN_CHANNEL_SELECT_BUTTON 2

// Deprecated, TODO: base on channel
#define STATUS_NOTE_ON 0x90
#define STATUS_NOTE_OFF 0x80

// Hard-coded array of 7segment sequences.
// Values set up in setup()
byte DIGITS[10];

// The MIDI channel we're interested in
volatile byte midiChannel = 1;

// Used for debouncing channel select button
volatile long lastButtonPress = 0;

// Number of keys being pressed on keyboard
unsigned short notesOn = 0;

SoftwareSerial sSerial(PIN_SOFTWARE_RX, PIN_SOFTWARE_TX);

void setup() {
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_GATE, OUTPUT);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_CHANNEL_SELECT_BUTTON, INPUT);
  
  // Set up 7 segment digit bytes
  // Segments:  ABCDEFGdp
  DIGITS[0] = 0b11111100;
  DIGITS[1] = 0b01100000;
  DIGITS[2] = 0b11011010;
  DIGITS[3] = 0b11110010;
  DIGITS[4] = 0b01100110;
  DIGITS[5] = 0b10110110;
  DIGITS[6] = 0b10111110;
  DIGITS[7] = 0b11100000;
  DIGITS[8] = 0b11111110;
  DIGITS[9] = 0b11110110;
  
  attachInterrupt(
    digitalPinToInterrupt(PIN_CHANNEL_SELECT_BUTTON),
    channelSelectButtonOnPressed,
    FALLING); 
  
  // Prepare to receive MIDI data
  sSerial.begin(MIDI_SERIAL_BAUD_RATE);
  
  // Ensure shift registers don't think we're writing
  digitalWrite(PIN_LATCH, LOW);

  // Retrieve last MIDI channel used from EEPROM
  midiChannel = EEPROM.read(MIDI_CHANNEL_EEPROM_ADDRESS);
  if (midiChannel < 1 || midiChannel > MIDI_CHANNELS) {
    midiChannel = 1;
  }
  
  // Allow some time for the power-up reset circuit to do its thing,
  // resetting the shift registers before we pump data into them
  delay(POWERUP_MS);
  update7Seg(midiChannel);
}

void loop() {
  do {
    if (!sSerial.available()) continue;

    byte data[3];
    
    data[0] = sSerial.read();
    data[1] = sSerial.read();
    data[2] = sSerial.read();
    
    processMIDIMessage(data);
  }
  while(sSerial.available() >= MIN_MIDI_BYTES);
}

void processMIDIMessage(byte data[3]) {
  if (data[0] == STATUS_NOTE_ON) {
    notesOn++;
    sendTriggerPulse();
  } else if (data[0] == STATUS_NOTE_OFF) {
    notesOn--;
    updateGate();
  }
  
  updateGate();
}

void updateGate() {
  digitalWrite(PIN_GATE, notesOn > 0 ? HIGH : LOW);
}

void sendTriggerPulse() {
   digitalWrite(PIN_TRIGGER, HIGH);
   // TODO: this can cause some messages to be missed.
   // Checking for data should be done on an interrupt basis (timed)
  delay(1);
  digitalWrite(PIN_TRIGGER, LOW);
}

void channelSelectButtonOnPressed() {
  // Debounce
  long currentTime = millis();
  if (currentTime - lastButtonPress < BUTTON_DEBOUNCE_MS) return;
  lastButtonPress = currentTime;

  increment();
}

void increment() {
  midiChannel++;
  if (midiChannel > MIDI_CHANNELS) midiChannel = 1;

  update7Seg(midiChannel);
  
  EEPROM.write(MIDI_CHANNEL_EEPROM_ADDRESS, midiChannel);

  notesOn = 0;
  updateGate();
  // TODO: update any variables pertaining to infering
  // channel from messages
}

void update7Seg(int number) {
  byte *digits = getDigitBytes(number);

  digitalWrite(PIN_LATCH, HIGH);

  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[1]);
  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[0]);

  // Done writing data
  digitalWrite(PIN_LATCH, LOW);

  // Slightly hacky lacking, but doing this makes the registers actually output
  // the newly shifted data. Maybe that's how it's meant to work.
  digitalWrite(PIN_LATCH, HIGH);
  digitalWrite(PIN_LATCH, LOW);
}

byte * getDigitBytes(int number) {
  static byte digits[2];
  // TODO: the local var types here could be optimized
  int ones = number % 10;
  int tens = (number - ones) / 10;

  if (tens > 9) tens = 0;

  digits[0] = DIGITS[tens];
  digits[1] = DIGITS[ones];
  return digits;
}
