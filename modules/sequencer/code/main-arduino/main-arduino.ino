#include <TimerOne.h>

#include "sequence.h"
#include "io.h"

#define DEFAULT_TEMPO 130

#define BUTTON_DEBOUNCE_TICKS 10

// Borrowing from Midi in code, as we're dealing with the same range
#define MIN_NOTE_MIDI 24 // C2
#define MAX_NOTE_MIDI 108 // C9
#define NOTE_RANGE 84

long lastTriggered = 0;

// Array of DAC output values mapped to note index
unsigned int NoteOutputValues[NOTE_RANGE + 1];

volatile bool initialized = false;

// Ranging of output notes
// TODO write more helpful commentary here
volatile byte rangeMinNote = 12;
volatile byte rangeMaxNote = 36;

// Input stabilization
// For debouncing buttons that trigger interrupts
volatile unsigned long debounceTimeRecorded = 0;
volatile unsigned long debounceTicksRecorded = 0;

// Alternative to micros() when running,
// as we do too much work for micros to work reliably when running
volatile unsigned long debounceTicks = 0;

void setup() {
  // Tempo is in beats per minute
  // MIDI PPQ is pulses per beat
  // And timer period is in microseconds, of which there are 1000000 per second
  // 60 being seconds per minute
  // Summarized, the period of the timer is calculated to generate MIDI_PPQ pulses per beat for the given bpm
  Timer1.initialize(round((double)1000000 / (double)((DEFAULT_TEMPO * PPQ) / (double)60)));
  Timer1.attachInterrupt(internalTimerTick);

  IO::init();
  IO::onGateButtonPressed(gateModeOnPressed);
  IO::onRepeatButtonPressed(repeatOnPressed);
  IO::onRunStopButtonPressed(runStopOnPressed);

  Sequence::init();
  Sequence::onRunningChange(sequenceOnRunningChanged);
  Sequence::onSequenceModeChange(sequenceOnSequenceModeChanged);
  Sequence::onGateChange(sequenceOnGateChanged);
  Sequence::onTriggerChange(sequenceOnTriggerChanged);
  Sequence::onSelectedStepChange(sequenceOnSelectedStepChanged);

  IO::setStep(0);

  // Precalculate all the note values so we don't have to
  // during normal runtime.
  float noteStep = (float)4096 / (float)(NOTE_RANGE - 1);

  for (byte i = 0; i <= NOTE_RANGE; i++) {
    NoteOutputValues[i] = (unsigned int)round(noteStep * (float)i);
  }

  char initText[] = "Init";
  IO::writeDisplay(initText);

  initialized = true;
}

void loop() {
  byte oneIndexedStep = getSelectedStep();

  if (IO::getPortExpPin(PORTEXP_PIN_MODE_SELECT_BUTTON) == 0) {
    // Mode select button held down
    if (oneIndexedStep != 0) {
      Sequence::setSequenceMode(oneIndexedStep - 1);
    }
  } else if (!Sequence::isRunning()) {
    // Not running, and no other buttons pressed,
    // make this step active
    if (oneIndexedStep != 0) {
      Sequence::selectStep(oneIndexedStep - 1);
      Sequence::setGate(true);

      IO::readAdc(mapNoteAndWriteDac);
    } else {
      Sequence::setGate(false);
    }
  }

  delay(1);
}

void mapNoteAndWriteDac(unsigned int adcReading) {
  unsigned short int note = mapToNote(adcReading);
  IO::setPitch(NoteOutputValues[note]);
}

void internalTimerTick() {
  if (!initialized) return;

  debounceTicks++;
  IO::readPortExp();
  Sequence::tick();
}

// Gets the currently pressed button index, corresponding to the step it's under.
// The return value is 1-indexed, as the 0 value is reserved for when no button is pressed.
byte getSelectedStep() {
  // The pins are pull-up, so 1 = not pressed.
  // Therefore, we're bitwise inverting first.
  uint8_t wordA = ~(IO::readPortExpCache() & 0xFF);

  if (wordA == 0) return 0;

  // I don't know if this would be less efficient in a for loop.
  // Worth playing with to test performance.
  if (wordA & 0b10000000) return 8;
  if (wordA & 0b01000000) return 7;
  if (wordA & 0b00100000) return 6;
  if (wordA & 0b00010000) return 5;
  if (wordA & 0b00001000) return 4;
  if (wordA & 0b00000100) return 3;
  if (wordA & 0b00000010) return 2;
  if (wordA & 0b00000001) return 1;
}

// Maps an ADC reading to a note index, given the configured note range
// and other scale settings.
// Currently hardcoded to only return "white keys" - TODO update comment when that changes
byte mapToNote(short int inputValue) {
  byte note = (byte)(((float)inputValue / (float)ADC_MAX) * (rangeMaxNote - rangeMinNote + 1) + rangeMinNote);

  byte baseNote = note % 12;
  int blackKeys[] = {1, 3, 6, 8, 10};

  bool isBlackKey = false;
  for (byte i = 0; i < 5; i++) {
    if (baseNote == blackKeys[i]) {
      isBlackKey = true;
      break;
    }
  }

  if (isBlackKey) {
    return note + 1;
  }

  return note;
}

// Button handlers

bool debounceButton() {
  unsigned long currentTicks = debounceTicks;

  if (currentTicks - debounceTicksRecorded < BUTTON_DEBOUNCE_TICKS) {
    return true;
  }

  debounceTicksRecorded = currentTicks;
  return false;
}

// Run/stop pressed
void runStopOnPressed() {
  if (debounceButton()) return;
  Sequence::toggleRunning();
}

void gateModeOnPressed() {
  // TODO: evaluate if still needed
  if (debounceButton()) return;

  byte stepToAlter = getRelevantOneIndexedStep();
  if (stepToAlter == 0) return;
  stepToAlter--;

  byte newGateMode = Sequence::cycleGateModeForStep(stepToAlter);
  // TODO: display new gate mode
}

void repeatOnPressed() {
  // TODO: evaluate if still needed
  if (debounceButton()) return;

  byte stepToAlter = getRelevantOneIndexedStep();
  if (stepToAlter == 0) return;
  stepToAlter--;
  Sequence::cycleStepRepeatForStep(stepToAlter);
  //TODO: display new step repeat
}

// Retrieves the step to alter some property of, based on whether we're running
byte getRelevantOneIndexedStep() {
  if (!Sequence::isRunning()) {
    return Sequence::getSelectedStep() + 1;
  } else {
    return getSelectedStep();
  }
}

void sequenceOnRunningChanged(bool running) {
  IO::setRunning(running);
}

void sequenceOnGateChanged(bool gateOn) {
  IO::setGate(gateOn);
}

void sequenceOnTriggerChanged(bool triggerOn) {
  IO::setTrigger(triggerOn);
}

void sequenceOnSelectedStepChanged(byte selectedStep) {
  IO::setStep(selectedStep);
  IO::readAdc(mapNoteAndWriteDac);
}

void sequenceOnSequenceModeChanged(byte sequenceMode) {
  char text[] = "SEQ0";
  text[3] = '0' + sequenceMode;
  IO::writeDisplay(text);
}

