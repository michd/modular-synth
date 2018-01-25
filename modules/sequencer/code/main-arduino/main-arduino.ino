#include "sequence.h"
#include "io.h"
#include "notemapper.h"
#include "settings.h"
#include <stdint.h>

// Note: Using String for displaying our text as opposed to plain char
// arrays adds a lot of convenience, but adds a significant amount to
// program size. If program space becomes tight, this is a good area
// to optimize.
#include <String.h>

#define RESET_BUTTON_HOLD_TIME 1500
#define PORTEXP_READ_CYCLE_TICKS 100

// Uncomment to enable serial logging
//#define DEBUGLOGGING

// TODO: wire up Sequence end to generate chain out pulse

volatile bool initialized = false;

volatile uint8_t lastNoteRead;

volatile bool holdingReset = false;

volatile uint16_t resetTicksHeld = 0;

volatile uint8_t readCycleTicks = 0;

void setup() {
  IO::init();
  IO::onSequenceModeButtonPressed(sequenceModeOnPressed);
  IO::onGateButtonPressed(gateModeOnPressed);
  IO::onRepeatButtonPressed(repeatOnPressed);
  IO::onRunModeButtonPressed(Sequence::toggleRunMode);
  IO::onScaleButtonPressed(scaleOnPressed);
  IO::onResetButtonPressed(resetOnPressed);
  IO::onLoadButtonPressed(loadOnPressed);
  IO::onSaveButtonPressed(saveOnPressed);

  IO::onMinNoteArrowButtonPressed(minNoteArrowPressed);
  IO::onMaxNoteArrowButtonPressed(maxNoteArrowPressed);
  IO::onTimeDivisionArrowButtonPressed(timeDivisionArrowPressed);

  IO::onChainInputChanged(chainInputChanged);

  IO::onExternalClockTick(externalClockTick);

  Sequence::init();
  Sequence::onRunningIndicatorChange(IO::setRunningIndicator);
  Sequence::onSequenceModeChange(sequenceOnSequenceModeChanged);
  Sequence::onGateChange(IO::setGate);
  Sequence::onTriggerChange(IO::setTrigger);
  Sequence::onSelectedStepChange(sequenceOnSelectedStepChanged);
  Sequence::onSequenceEnd(sequenceOnSequenceEnd);

  Sequence::setChained(!IO::getPortExpPin(PORTEXP_PIN_CHAIN_INPUT));

  NoteMapper::init();

  IO::setStep(0);

  fullReset();

  initialized = true;
}

void loop() {
  uint8_t oneIndexedStep = IO::getSelectedStep();

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
  uint8_t note = NoteMapper::mapToNote(adcReading);

  IO::setPitch(NoteMapper::getNoteOutput(note));
  
  if (!Sequence::isRunning()) {
    IO::writeDisplay(NoteMapper::getNoteText(note));
    IO::writeLeds(INDICATOR_STEP_NOTE);
  }
}

void externalClockTick() {
  if (!initialized) return;
  if (IO::getChainOut()) IO::setChainOut(false);
  IO::readSelectedStep();
  Sequence::tick();
}

void fullReset() {
  Sequence::loadFromSettings(&(SettingsManager::defaultSettings));
  NoteMapper::loadFromSettings(&(SettingsManager::defaultSettings));
  IO::writeDisplay("Init");
  IO::writeLeds(INDICATOR_NONE);
}

// Button handlers

// Scale button pressed
void scaleOnPressed() {
  uint8_t newScale = NoteMapper::cycleScale();
  IO::writeDisplay(NoteMapper::getScaleText(newScale));
  IO::writeLeds(INDICATOR_NONE);
}

void sequenceModeOnPressed() {
  uint8_t oneIndexedStep = IO::getSelectedStep();

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
    for (uint8_t i = 0; i < NUM_STEPS; i++) {
      settingsToRevertTo.gateModes[i] = defSettings.gateModes[i];
    }

    resultText += "Ga";
    fullReset = false;
  } 

  // Reset step repeats
  if (!IO::getPortExpPin(PORTEXP_PIN_REPEAT_SELECT_BUTTON)) {    
    for (uint8_t i = 0; i < NUM_STEPS; i++) {
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

    uint8_t selectedParam = IO::getSelectedParam();

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
    IO::writeLeds(INDICATOR_NONE);
  }
}

void loadOnPressed() {
  // Get settings slot from pressed sequence button
  // If none is pressed, defaults to the first
  // getSelectedStep is 1-indexed and returns 0 for none pressed
  // 0 is a valid slot of its own.
  uint8_t slot = IO::getSelectedStep();
  Settings settings = SettingsManager::load(slot);
  Sequence::loadFromSettings(&settings);
  NoteMapper::loadFromSettings(&settings);
  IO::writeDisplay("Loa." + String(slot));
  IO::writeLeds(INDICATOR_NONE);
}

void saveOnPressed() {
  // Get settings slot from pressed sequence button
  // If none is pressed, defaults to the first
  // getSelectedStep is 1-indexed and returns 0 for none pressed
  // 0 is a valid slot of its own
  uint8_t slot = IO::getSelectedStep();

  Settings settingsToSave;
  Sequence::collectSettings(&settingsToSave);
  NoteMapper::collectSettings(&settingsToSave);  
  SettingsManager::save(settingsToSave, slot);
  IO::writeDisplay("Sav." + String(slot));
  IO::writeLeds(INDICATOR_NONE);
}

void gateModeOnPressed() {
  uint8_t stepToAlter = IO::getSelectedStep();
  if (stepToAlter == 0) return;
  stepToAlter--;

  uint8_t newGateMode = Sequence::cycleGateModeForStep(stepToAlter);

  IO::writeDisplay(
    "GM." + String(stepToAlter + 1) + "." + String(newGateMode + 1));
  IO::writeLeds(INDICATOR_NONE);
}

void repeatOnPressed() { 
  uint8_t stepToAlter = IO::getSelectedStep();
  if (stepToAlter == 0) return;
  stepToAlter--;
  uint8_t newStepRepeat = Sequence::cycleStepRepeatForStep(stepToAlter);
  IO::writeDisplay(
    "SR." + String(stepToAlter + 1) + "." + String(newStepRepeat));
  IO::writeLeds(INDICATOR_NONE);
}

void minNoteArrowPressed(bool upArrow) {
  uint8_t newMinNote = NoteMapper::cycleMinNote(upArrow);
  IO::writeDisplay(NoteMapper::getNoteText(newMinNote));
  IO::writeLeds(INDICATOR_MIN_NOTE);
}

void maxNoteArrowPressed(bool upArrow) {
  uint8_t newMaxNote = NoteMapper::cycleMaxNote(upArrow);
  IO::writeDisplay(NoteMapper::getNoteText(newMaxNote));  
  IO::writeLeds(INDICATOR_MAX_NOTE);
}

void timeDivisionArrowPressed(bool upArrow) {
  uint8_t newDivider = Sequence::cycleTimeDivider(upArrow);

  IO::writeDisplay(
    "1/" + ((newDivider < 10) ? String(" ") : String("")) + String(newDivider));
  IO::writeLeds(INDICATOR_NONE);
}

void chainInputChanged(bool value) {
  // Rising flank = trigger
  if (value == 1) {
    Sequence::chainTrigger();
  }

  // Active low, if pulled to gnd we're chained.
  Sequence::setChained(!value);
}

void sequenceOnSelectedStepChanged(uint8_t selectedStep) {
  IO::setStep(selectedStep);
  IO::readAdc(mapNoteAndWriteDac);
}

void sequenceOnSequenceModeChanged(uint8_t sequenceMode) {
  IO::writeDisplay("SEQ." + String(sequenceMode + 1));
  IO::writeLeds(INDICATOR_NONE);
}

void sequenceOnSequenceEnd() {
  IO::setChainOut(true);
}
