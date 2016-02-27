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
// Analog outputs are made symmetrical around 0, this value
// sits in the center of the PWM output range of 0-255, setting
// The output to 0V.
#define ANALOG_CENTER_VALUE 128

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
volatile byte midiChannel = 0;

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

  // Begin listening for midi messages
  // We listen to all channels so we can change the channel
  // at runtime and filter ourselves. This means we don't
  // have to hardcode the channel.
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // Allow some time for the power-up reset circuit to do its thing,
  // resetting the shift registers before we pump data into them
  delay(POWERUP_MS);
  
  // Retrieve last MIDI channel used from EEPROM
  setMIDIChannel(EEPROM.read(MIDI_CHANNEL_EEPROM_ADDRESS));
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
      // Maybe in a later version.
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
      unsigned int pitchValue = MIDI.getData2() << 7 + MIDI.getData1();
      analogWrite(PIN_MOD4, pitchValue / 64);
      break;
  }
};

void processControlChange(byte controllerNumber, byte value) {
  byte outputPin;

  switch(controllerNumber) {
    case MIDIC_MODULATION: outputPin = PIN_MOD1; break;
    case MIDIC_VOLUME: outputPin = PIN_MOD2; break;
    case MIDIC_BALANCE: outputPin = PIN_MOD3; break;
    // If the controller isn't listed above, we can do nothing
    default: return;
  }
  
  // The value here comes from a MIDI value and will be
  // in the range of 0-127(0b01111111), shifting it to the left
  // effectively just multiplies it by two, getting 0-255 range
  // with the same precision.
  analogWrite(outputPin, value << 1);
}

void updateGate() {
  digitalWrite(PIN_GATE, notesOn > 0 ? HIGH : LOW);
}

void updateFreqOutput() {
  // TODO: Map notes to analogWrite values to achieve 1V/octave
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

  setMIDIChannel(midiChannel + 1);
}

void setMIDIChannel(byte newChannel) {
  if (newChannel > MIDI_CHANNELS) newChannel = 1;
  
  if (midiChannel == newChannel) return;
  
  midiChannel = newChannel;
  updateDisplay();
  
  notesOn = 0;
  updateGate();
  analogWrite(PIN_MOD1, ANALOG_CENTER_VALUE);
  analogWrite(PIN_MOD2, ANALOG_CENTER_VALUE);
  analogWrite(PIN_MOD3, ANALOG_CENTER_VALUE);
  analogWrite(PIN_MOD4, ANALOG_CENTER_VALUE);
  analogWrite(PIN_FREQ, ANALOG_CENTER_VALUE);
  
  EEPROM.write(MIDI_CHANNEL_EEPROM_ADDRESS, midiChannel);
}

void updateDisplay() {
  byte *digits = getDigitBytes(midiChannel);

  digitalWrite(PIN_LATCH, HIGH);

  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[1]);
  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[0]);

  // Done writing data
  digitalWrite(PIN_LATCH, LOW);

  // Slightly hacky fix, but doing this makes the registers actually output
  // the newly shifted data. Maybe that's how it's meant to work.
  digitalWrite(PIN_LATCH, HIGH);
  digitalWrite(PIN_LATCH, LOW);
}

// Assumes number < 100. In this program we're only dealing with 1-16.
byte * getDigitBytes(byte number) {
  static byte digits[2];
  byte ones = number % 10;
  byte tens = (number - ones) / 10;

  digits[0] = DIGITS[tens];
  digits[1] = DIGITS[ones];
  return digits;
}
