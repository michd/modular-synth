#include <TimerOne.h>

#include "sequence.h"
#include "io.h"
#include "notemapper.h"

#define DEFAULT_TEMPO 80

#define BUTTON_DEBOUNCE_TICKS 10

long lastTriggered = 0;

volatile bool initialized = false;

volatile byte lastNoteRead;

// Input stabilization
// For debouncing buttons that trigger interrupts
volatile unsigned long debounceTimeRecorded = 0;
volatile unsigned long debounceTicksRecorded = 0;

// Alternative to micros() when running,
// as we do too much work for micros to work reliably when running
volatile unsigned long debounceTicks = 0;

void setup() {
  IO::init();
  IO::onGateButtonPressed(gateModeOnPressed);
  IO::onRepeatButtonPressed(repeatOnPressed);
  IO::onRunStopButtonPressed(runStopOnPressed);
  IO::onExternalClockTick(externalClockTick);

  Sequence::init();
  Sequence::onRunningChange(sequenceOnRunningChanged);
  Sequence::onSequenceModeChange(sequenceOnSequenceModeChanged);
  Sequence::onGateChange(sequenceOnGateChanged);
  Sequence::onTriggerChange(sequenceOnTriggerChanged);
  Sequence::onSelectedStepChange(sequenceOnSelectedStepChanged);

  NoteMapper::init();

  IO::setStep(0);

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
  byte oldNote = lastNoteRead;
  unsigned short int note = NoteMapper::mapToNote(adcReading);

  lastNoteRead = note;
  IO::setPitch(NoteMapper::getNoteOutput(note));

  if (!Sequence::isRunning() && oldNote != note) {
    IO::writeDisplay(NoteMapper::getNoteText(note));
  }
}

void externalClockTick() {
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
  uint8_t wordA = (IO::readPortExpCache() & 0xFF);

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

  char text[] = "GM.0.0";
  text[3] = '1' + stepToAlter;
  text[5] = '1' + newGateMode;
  IO::writeDisplay(text);
}

void repeatOnPressed() {
  // TODO: evaluate if still needed
  if (debounceButton()) return;

  byte stepToAlter = getRelevantOneIndexedStep();
  if (stepToAlter == 0) return;
  stepToAlter--;
  byte newStepRepeat = Sequence::cycleStepRepeatForStep(stepToAlter);
  char text[] = "SR.0.0";
  text[3] = '1' + stepToAlter;
  text[5] = '0' + newStepRepeat;
  IO::writeDisplay(text);
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
  char text[] = "SEQ.0";
  text[4] = '0' + sequenceMode + 1;
  IO::writeDisplay(text);
}

