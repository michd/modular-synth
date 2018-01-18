#include "sequence.h"
#include "io.h"
#include "notemapper.h"
#include "settings.h"

// Note: Using String for displaying our text as opposed to plain char
// arrays adds a lot of convenience, but adds a significant amount to
// program size. If program space becomes tight, this is a good area
// to optimize.
#include <String.h>

#define RESET_BUTTON_HOLD_TIME 1500
#define PORTEXP_READ_CYCLE_TICKS 100

volatile bool initialized = false;

volatile byte lastNoteRead;

volatile bool holdingReset = false;

volatile unsigned int resetTicksHeld = 0;

volatile byte readCycleTicks = 0;

void setup() {
  IO::init();
  IO::onSequenceModeButtonPressed(sequenceModeOnPressed);
  IO::onGateButtonPressed(gateModeOnPressed);
  IO::onRepeatButtonPressed(repeatOnPressed);
  IO::onRunStopButtonPressed(Sequence::toggleRunning);
  IO::onScaleButtonPressed(scaleOnPressed);
  IO::onResetButtonPressed(resetOnPressed);
  IO::onLoadButtonPressed(loadOnPressed);
  IO::onSaveButtonPressed(saveOnPressed);

  IO::onMinNoteArrowButtonPressed(minNoteArrowPressed);
  IO::onMaxNoteArrowButtonPressed(maxNoteArrowPressed);
  IO::onTimeDivisionArrowButtonPressed(timeDivisionArrowPressed);

  IO::onExternalClockTick(externalClockTick);

  Sequence::init();
  Sequence::onRunningChange(IO::setRunning);
  Sequence::onSequenceModeChange(sequenceOnSequenceModeChanged);
  Sequence::onGateChange(IO::setGate);
  Sequence::onTriggerChange(IO::setTrigger);
  Sequence::onSelectedStepChange(sequenceOnSelectedStepChanged);

  NoteMapper::init();

  IO::setStep(0);

  fullReset();

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

  // If we were holding the reset button, and it is still held,
  // increase ticks held, and measure if we've held for long enough.
  // If held for long enough, reset counter and held flag, and execute full
  // reset
  if (holdingReset && !IO::getPortExpPin(PORTEXP_PIN_RESET_BUTTON)) {
    resetTicksHeld++;

    if (resetTicksHeld == RESET_BUTTON_HOLD_TIME) {
      resetTicksHeld = 0;
      holdingReset = false;
      fullReset();
    }
  }

  readCycleTicks++;

  if (readCycleTicks == PORTEXP_READ_CYCLE_TICKS) {
    readCycleTicks = 0;
    IO::readPortExp();
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
  IO::readSelectedStep();
  Sequence::tick();
}

void fullReset() {
  Sequence::loadFromSettings(&(SettingsManager::defaultSettings));
  NoteMapper::loadFromSettings(&(SettingsManager::defaultSettings));
  IO::writeDisplay("Init");
}

// Button handlers

// Run/stop pressed
void runStopOnPressed() {
  Sequence::toggleRunning();
}

// Scale button pressed
void scaleOnPressed() {
  byte newScale = NoteMapper::cycleScale();
  IO::writeDisplay(NoteMapper::getScaleText(newScale));
}

void sequenceModeOnPressed() {
  byte oneIndexedStep = IO::getSelectedStep();

  if (oneIndexedStep != 0) {
    Sequence::setSequenceMode(oneIndexedStep - 1);
  }
}

// Reset all settings to default, or only one of them
// If reset is pressed while no relevant other button is pressed, all settings
// are reset to default, and "Init" is displayed.
// If reset is pressed while one or more relevant other buttons are pressed,
// only those settings get reset to defaults, and a message re.XX is shown,
// where XX attempts to indicate what was reset.
// Note: the XX displayed will be for the last matching button, so if more
// than one button was held to be reset, all those settings are reset,
// but only the bottom one is displayed on-screen.
void resetOnPressed() {
  String resultText = "re.";

  Settings settingsToRevertTo;
  Sequence::collectSettings(&settingsToRevertTo);
  NoteMapper::collectSettings(&settingsToRevertTo);

  Settings &defSettings = SettingsManager::defaultSettings;

  bool fullReset = true;

  // Reset gate modes
  if (!IO::getPortExpPin(PORTEXP_PIN_GATE_MODE_SELECT_BUTTON)) {
    for (byte i = 0; i < NUM_STEPS; i++) {
      settingsToRevertTo.gateModes[i] = defSettings.gateModes[i];
    }

    resultText += "Ga";
    fullReset = false;
  } 

  // Reset step repeats
  if (!IO::getPortExpPin(PORTEXP_PIN_REPEAT_SELECT_BUTTON)) {    
    for (byte i = 0; i < NUM_STEPS; i++) {
      settingsToRevertTo.stepRepeat[i] = defSettings.stepRepeat[i];
    }

    resultText += "Sr";
    fullReset = false;
  } 

  // Reset sequence mode
  if (!IO::getPortExpPin(PORTEXP_PIN_SEQUENCE_MODE_SELECT_BUTTON)) {    
    settingsToRevertTo.sequenceMode = defSettings.sequenceMode;
    resultText += "SM";
    fullReset = false;
  } 

  // Reset scale
  if (!IO::getPortExpPin(PORTEXP_PIN_SCALE_BUTTON)) {
    settingsToRevertTo.scale = defSettings.scale;
    resultText += "Sc";
    fullReset = false;
  } 

  // Reset setting controlled by the arrow buttons,
  // depending on how the param select switch is set
  if (!IO::getPortExpPin(PORTEXP_PIN_UP_ARROW) 
    || !IO::getPortExpPin(PORTEXP_PIN_DOWN_ARROW)) {

    byte selectedParam = IO::getSelectedParam();

    switch (selectedParam) {
      case PARAM_MIN_NOTE:
        // Reset min note
        settingsToRevertTo.minNote = defSettings.minNote;
        resultText += "Mi";
        break;

      case PARAM_MAX_NOTE:
        // Reset max note
        settingsToRevertTo.maxNote = defSettings.maxNote;
        resultText += "Ma";
        break;

      case PARAM_TIME_DIVIDER:      
      default:
        // Reset time divider
        settingsToRevertTo.timeDivider = defSettings.timeDivider;
        resultText += "Td";
        break;
    }

    fullReset = false;
  } 

  // If we're not pressing anything else,
  // ensure we hold reset for a while before doing the full reset.
  holdingReset = fullReset;
  if (fullReset) resetTicksHeld = 0;

  if (!fullReset) {
    Sequence::loadFromSettings(&settingsToRevertTo);
    NoteMapper::loadFromSettings(&settingsToRevertTo);
    IO::writeDisplay(resultText);  
  }
}

void loadOnPressed() {
  // Get settings slot from pressed sequence button
  // If none is pressed, defaults to the first
  // getSelectedStep is 1-indexed and returns 0 for none pressed
  // 0 is a valid slot of its own.
  byte slot = IO::getSelectedStep();
  Settings settings = SettingsManager::load(slot);
  Sequence::loadFromSettings(&settings);
  NoteMapper::loadFromSettings(&settings);
  IO::writeDisplay("Loa." + String(slot));
}

void saveOnPressed() {
  // Get settings slot from pressed sequence button
  // If none is pressed, defaults to the first
  // getSelectedStep is 1-indexed and returns 0 for none pressed
  // 0 is a valid slot of its own
  byte slot = IO::getSelectedStep();

  Settings settingsToSave;
  Sequence::collectSettings(&settingsToSave);
  NoteMapper::collectSettings(&settingsToSave);  
  SettingsManager::save(settingsToSave, slot);
  IO::writeDisplay("Sav." + String(slot));
}

void gateModeOnPressed() {
  byte stepToAlter = IO::getSelectedStep();
  if (stepToAlter == 0) return;
  stepToAlter--;

  byte newGateMode = Sequence::cycleGateModeForStep(stepToAlter);

  IO::writeDisplay(
    "GM." + String(stepToAlter + 1) + "." + String(newGateMode + 1));
}

void repeatOnPressed() { 
  byte stepToAlter = IO::getSelectedStep();
  if (stepToAlter == 0) return;
  stepToAlter--;
  byte newStepRepeat = Sequence::cycleStepRepeatForStep(stepToAlter);
  IO::writeDisplay(
    "SR." + String(stepToAlter + 1) + "." + String(newStepRepeat));
}

void minNoteArrowPressed(bool upArrow) {
  byte newMinNote = NoteMapper::cycleMinNote(upArrow);
  // TODO: way to also indicate what the note being displayed is for
  IO::writeDisplay(NoteMapper::getNoteText(newMinNote));
}

void maxNoteArrowPressed(bool upArrow) {
  byte newMaxNote = NoteMapper::cycleMaxNote(upArrow);
  IO::writeDisplay(NoteMapper::getNoteText(newMaxNote));  
}

void timeDivisionArrowPressed(bool upArrow) {
  byte newDivider = Sequence::cycleTimeDivider(upArrow);

  IO::writeDisplay(
    "1/" + ((newDivider < 10) ? String(" ") : String("")) + String(newDivider));
}

void sequenceOnSelectedStepChanged(byte selectedStep) {
  IO::setStep(selectedStep);
  IO::readAdc(mapNoteAndWriteDac);
}

void sequenceOnSequenceModeChanged(byte sequenceMode) {
  IO::writeDisplay("SEQ." + String(sequenceMode + 1));
}
