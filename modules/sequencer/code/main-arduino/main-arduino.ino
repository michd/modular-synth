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
  IO::onSequenceModeButtonPressed(sequenceModeOnPressed);
  IO::onGateButtonPressed(gateModeOnPressed);
  IO::onRepeatButtonPressed(repeatOnPressed);
  IO::onRunStopButtonPressed(runStopOnPressed);
  IO::onMinNoteArrowButtonPressed(minNoteArrowPressed);
  IO::onMaxNoteArrowButtonPressed(maxNoteArrowPressed);
  IO::onTimeDivisionArrowButtonPressed(timeDivisionArrowPressed);

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
  byte oneIndexedStep = IO::getSelectedStep();

  if (!Sequence::isRunning()) {
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
  // TODO: perhaps the routine reads should not be based on
  // external clock since it's variable?
  debounceTicks++;
  IO::readSelectedStep();
  Sequence::tick();
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

void sequenceModeOnPressed() {
  // TODO: evaluate if still needed
  if (debounceButton()) return;

  byte oneIndexedStep = IO::getSelectedStep();

  if (oneIndexedStep != 0) {
    Sequence::setSequenceMode(oneIndexedStep - 1);
  }
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

void minNoteArrowPressed(bool upArrow) {
  // TODO: evaluate if still needed
  if (debounceButton()) return;

  byte newMinNote = NoteMapper::cycleMinNote(upArrow);
  // TODO: way to also indicate what the note being displayed is for
  IO::writeDisplay(NoteMapper::getNoteText(newMinNote));
}

void maxNoteArrowPressed(bool upArrow) {
  // TODO: evaluate if still needed
  if (debounceButton()) return;

  byte newMaxNote = NoteMapper::cycleMaxNote(upArrow);
  IO::writeDisplay(NoteMapper::getNoteText(newMaxNote));  
}

void timeDivisionArrowPressed(bool upArrow) {
  // TODO: evaluate if still needed
  if (debounceButton()) return;

  byte newDivider = Sequence::cycleTimeDivider(upArrow);

  static char dispText[] =  "1/  ";

  if (newDivider >= 10) {
    dispText[2] = '0' + (char)(newDivider / 10);
    dispText[3] = '0' + (char)(newDivider % 10);
  } else {
    dispText[2] = ' ';
    dispText[3] = '0' + newDivider;
  }

  IO::writeDisplay(dispText);
}

// Retrieves the step to alter some property of, based on whether we're running
byte getRelevantOneIndexedStep() {
  if (!Sequence::isRunning()) {
    return Sequence::getSelectedStep() + 1;
  } else {
    return IO::getSelectedStep();
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

