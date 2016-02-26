// MIDI library from https://github.com/FortySevenEffects/arduino_midi_library
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

#include <EEPROM.h>

// Constant values
#define MIDI_CHANNELS 16
// Wait this long before accepting another button press
#define BUTTON_DEBOUNCE_MS 150
// Allows shift registers to reset on power-up
#define POWERUP_MS 200
// Where in the EEPROM we store the selected channel
#define MIDI_CHANNEL_EEPROM_ADDRESS 0

// MIDI control identifiers
#define MIDIC_MODULATION 0b00000001
#define MIDIC_VOLUME     0b00000111
#define MIDIC_BALANCE    0b00001000
#define MIDIC_PAN        0b00001010

// Output pins
#define PIN_GATE 7
#define PIN_TRIGGER 8
#define PIN_FREQ 9
#define PIN_MOD1 10
#define PIN_MOD2 6
#define PIN_MOD3 5
#define PIN_MOD4 3
// 7 Segment-related
#define PIN_LATCH 19
#define PIN_CLK 4
#define PIN_DATA 18

// Input pins
// Channel select button
#define PIN_CHANNEL_SELECT_BUTTON 2

//Initialize the MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

// Hard-coded array of 7segment sequences.
// Values set up in setup()
byte DIGITS[10];

// The MIDI channel we're interested in
volatile byte midiChannel = 1;

// Used for debouncing channel select button
volatile long lastButtonPress = 0;

// Number of keys being pressed on keyboard
volatile unsigned short notesOn = 0;

volatile byte activeNote;

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

  // Ensure shift registers don't think we're writing
  digitalWrite(PIN_LATCH, LOW);

  // Retrieve last MIDI channel used from EEPROM
  midiChannel = EEPROM.read(MIDI_CHANNEL_EEPROM_ADDRESS);
  
  if (midiChannel > MIDI_CHANNELS) midiChannel = 1;
  
  // Begin listening for midi messages
  // We listen to all channels so we can change the channel
  // at runtime and filter ourselves. This means we don't
  // have to hardcode the channel.
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // Allow some time for the power-up reset circuit to do its thing,
  // resetting the shift registers before we pump data into them
  delay(POWERUP_MS);

  update7Seg(midiChannel);
}

void loop() {
  if (MIDI.read())
  {
    // Ignore messages not destined for our channel
    if (MIDI.getChannel() !=  midiChannel) return;
    processMIDIMessage();
  }   
}

void processMIDIMessage() {
  switch(MIDI.getType()) {
    case midi::NoteOn:
      notesOn++;
      activeNote = MIDI.getData1();
      sendTriggerPulse();
      updateGate();
      updateFreqOutput();
      // TODO: use velocity for something
      break;

    case midi::NoteOff:
      notesOn--;
      updateGate();
      // TODO: maybe keep an array of which keys are pressed, to determine
      // which the active note should be (it should be the last key pressed)
      break;
      
    case midi::ControlChange:
      processControlChange(MIDI.getData1(), MIDI.getData2());
      break;
      
    case midi::PitchBend:
      // TODO
      break;
  }
};

void processControlChange(byte controllerNumber, byte value) {
  byte outputPin;

  switch(controllerNumber) {
    case MIDIC_MODULATION: outputPin = PIN_MOD1; break;
    case MIDIC_VOLUME: outputPin = PIN_MOD2; break;
    case MIDIC_BALANCE: outputPin = PIN_MOD3; break;
    case MIDIC_PAN: outputPin = PIN_MOD4; break;
    // If the controller isn't listed above, we can do nothing
    default: return;
  }
  
  analogWrite(outputPin, value << 1);
}

void updateGate() {
  digitalWrite(PIN_GATE, notesOn > 0 ? HIGH : LOW);
}

void updateFreqOutput() {
  // TODO: analogWrite etc
}

void sendTriggerPulse() {
  digitalWrite(PIN_TRIGGER, HIGH);
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
  if (midiChannel > MIDI_CHANNELS) midiChannel = 1;

  update7Seg(midiChannel);
  
  EEPROM.write(MIDI_CHANNEL_EEPROM_ADDRESS, midiChannel);
  notesOn = 0;
  updateGate();
  // TODO: reset all analog outputs to the middle value (128; 0V at the output)
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
