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

// Used to cancel out a piece of information from a MIDI status
#define MIDI_STATUS_ACTION_BITMASK  0b11110000
#define MIDI_STATUS_CHANNEL_BITMASK 0b00001111

// MIDI status bytes (when the channel info is stripped off)
#define MIDIS_NOTE_OFF        0b10000000
#define MIDIS_NOTE_ON         0b10010000
#define MIDIS_POLY_AFTERTOUCH 0b10100000
#define MIDIS_CONTROL_CHANGE  0b10110000
#define MIDIS_PROGRAM_CHANGE  0b11000000
#define MIDIS_CHAN_AFTERTOUCH 0b11010000
#define MIDIS_PITCH_BEND      0b11100000
// TODO?: Timing clock?

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

// Hard-coded array of 7segment sequences.
// Values set up in setup()
byte DIGITS[10];

// The MIDI channel we're interested in
volatile byte midiChannel = 0;

// Used for debouncing channel select button
volatile long lastButtonPress = 0;

// Number of keys being pressed on keyboard
volatile unsigned short notesOn = 0;

volatile byte activeNote;

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

  if (midiChannel >= MIDI_CHANNELS) midiChannel = 0;

  // Allow some time for the power-up reset circuit to do its thing,
  // resetting the shift registers before we pump data into them
  delay(POWERUP_MS);

  // MIDI channels are 0-indexed, increment one for human-friendly display
  update7Seg(midiChannel + 1);
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
  // Separate out just channel, discarding action
  byte statusChannel = data[0] & MIDI_STATUS_CHANNEL_BITMASK;
  byte statusAction;

  // Message is not for the channel we're interested in, abort
  // further processing
  if (statusChannel != midiChannel) return;

  // Separate out just the action, discarding channel
  statusAction = data[0] & MIDI_STATUS_ACTION_BITMASK;

  switch(statusAction) {
     case MIDIS_NOTE_ON:
       notesOn++;
       activeNote = data[1];
       sendTriggerPulse();
       updateGate();
       updateFreqOutput();
       // TODO: use velocity for something
       break;

    case MIDIS_NOTE_OFF:
      notesOn--;
      updateGate();
      // TODO: maybe keep an array of which keys are pressed, to determine
      // which the active note should be (it should be the last key pressed)
      break;

     case MIDIS_CONTROL_CHANGE:
       // TODO: keep an array of output controls.
       // Ignore any controls beyond the assigned ones
       break;

     case MIDIS_PITCH_BEND:
       // TODO: special case controller change I guess
       break;

     case MIDIS_POLY_AFTERTOUCH:
     case MIDIS_CHAN_AFTERTOUCH:
     case MIDIS_PROGRAM_CHANGE:
     default:
       // Not implemented
       break;
  }
}

void updateGate() {
  digitalWrite(PIN_GATE, notesOn > 0 ? HIGH : LOW);
}

void updateFreqOutput() {
  // TODO: analogWrite etc
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

  incrementMIDIChannel();
}

void incrementMIDIChannel() {
  midiChannel++;
  if (midiChannel >= MIDI_CHANNELS) midiChannel = 0;

  update7Seg(midiChannel + 1);
  
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
