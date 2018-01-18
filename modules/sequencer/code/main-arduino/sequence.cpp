#include "sequence.h"

// Note: this entire class is used in a static way, it is not to be instantiated.

volatile byte Sequence::_previousStep = 0;
volatile byte Sequence::_currentStep = 0;
volatile byte Sequence::_currentStepRepetition = 0;
volatile byte Sequence::_sequenceMode = SEQUENCE_MODE_FORWARD;
volatile byte Sequence::_gateMode[NUM_STEPS];
volatile byte Sequence::_stepRepeat[NUM_STEPS];
volatile byte Sequence::_timeDivider = DEFAULT_TIME_DIVIDER;
volatile unsigned long Sequence::_pulsesPerSubstep = (PPQ / 2) / (DEFAULT_TIME_DIVIDER / 4);
volatile bool Sequence::_firstHalfOfStep = true;
volatile unsigned long Sequence::_internalTicks = 0;
volatile bool Sequence::_running = false;
volatile bool Sequence::_gate = false;
volatile bool Sequence::_trigger = false;
BoolChangedHandler Sequence::_onRunningChangedHandler;
BoolChangedHandler Sequence::_onGateChangedHandler;
BoolChangedHandler Sequence::_onTriggerChangedHandler;
ByteChangedHandler Sequence::_onSelectedStepChangedHandler;
ByteChangedHandler Sequence::_onSequenceModeChangedHandler;

void Sequence::init() {
  for (byte i = 0; i < NUM_STEPS; i++) {
    _gateMode[i] = GATE_MODE_HALF_STEP;
    _stepRepeat[i] = 1;
  }
}

void Sequence::tick() {
  if (!_running) return;

  unsigned long ticks = _internalTicks;
  ticks++;

  if (ticks >= _pulsesPerSubstep) {
    ticks = 0;
    _advanceSubStep();
  }

  _internalTicks = ticks;
}

void Sequence::start() {
  if (_running) return;

  _running = true;
  (*_onRunningChangedHandler)(true);
}

void Sequence::stop() {
  if (!_running) return;

  _running = false;
  (*_onRunningChangedHandler)(false);

  setGate(false);
  _setTrigger(false);
}

void Sequence::toggleRunning() {
  if (_running) {
    stop();
  } else {
    start();
  }
}

bool Sequence::isRunning() {
  return _running;
}

void Sequence::setSequenceMode(byte newSequenceMode) {
  if (_sequenceMode == newSequenceMode) {
    // Trigger change handler anyway to update display
    (*_onSequenceModeChangedHandler)(newSequenceMode);
    return;
  }

  _sequenceMode = newSequenceMode;

  if (_sequenceMode == SEQUENCE_MODE_RANDOM) {
    ::randomSeed(::millis());
  }

  (*_onSequenceModeChangedHandler)(newSequenceMode);
}

void Sequence::selectStep(byte newSelectedStep) {
  _selectStep(constrain(newSelectedStep, 0, NUM_STEPS - 1));
}

byte Sequence::getSelectedStep() {
  return _currentStep;
}

void Sequence::setGate(bool on) {
  if (_gate == on) return;

  _gate = on;
  (*_onGateChangedHandler)(on);

  if (on) _setTrigger(true);
}

void Sequence::setTimeDivider(byte newDivider) {
  _timeDivider = constrain(newDivider, 1, 16);
  _pulsesPerSubstep = (short int)((double)(PPQ / 2) / ((double)newDivider / (double)4));
}

byte Sequence::cycleTimeDivider(bool higher) {
  byte newTimeDivider;

  if (higher) {
    newTimeDivider = _timeDivider << 1;

    if (newTimeDivider > MAX_TIME_DIVIDER) {
      newTimeDivider = MAX_TIME_DIVIDER;
    }
  } else {
    newTimeDivider = _timeDivider >> 1;

    if (newTimeDivider < MIN_TIME_DIVIDER) {
      newTimeDivider = MIN_TIME_DIVIDER;
    }
  }

  setTimeDivider(newTimeDivider);
  return _timeDivider;
}

byte Sequence::setGateModeForStep(byte step, byte gateMode) {
  step = constrain(step, 0, NUM_STEPS - 1);
  gateMode = constrain(gateMode, 0, MAX_GATE_MODE_VALUE);

  _gateMode[step] = gateMode;
  return gateMode;
}

byte Sequence::cycleGateModeForStep(byte step) {
  step = constrain(step, 0, NUM_STEPS - 1);
  byte gateMode = _gateMode[step];

  gateMode++;
  if (gateMode > MAX_GATE_MODE_VALUE) gateMode = 0;
  _gateMode[step] = gateMode;
  return gateMode;
}

byte Sequence::setStepRepeatForStep(byte step, byte repetitions) {
  step = constrain(step, 0, NUM_STEPS - 1);
  repetitions = constrain(repetitions, MIN_STEP_REPEAT, MAX_STEP_REPEAT);

  _stepRepeat[step] = repetitions;
  return repetitions;
}

byte Sequence::cycleStepRepeatForStep(byte step) {
  step = constrain(step, 0, NUM_STEPS - 1);
  byte stepRepetitions = _stepRepeat[step];

  stepRepetitions++;
  if (stepRepetitions > MAX_STEP_REPEAT) stepRepetitions = MIN_STEP_REPEAT;
  _stepRepeat[step] = stepRepetitions;
  return stepRepetitions;
}

void Sequence::collectSettings(Settings *settingsToSave) {
  settingsToSave->timeDivider = _timeDivider;
  settingsToSave->sequenceMode = _sequenceMode;

  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    settingsToSave->gateModes[i] = _gateMode[i];
    settingsToSave->stepRepeat[i] = _stepRepeat[i];
  }
}

void Sequence::loadFromSettings(Settings *settings) {
  setTimeDivider(constrain(settings->timeDivider, MIN_TIME_DIVIDER, MAX_TIME_DIVIDER));
  _sequenceMode = constrain(settings->sequenceMode, SEQUENCE_MODE_FORWARD, SEQUENCE_MODE_RANDOM);

  // TODO further clean up _timeDivider, essentially ensuring it only has one 1 in the byte

  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    _gateMode[i] = constrain(settings->gateModes[i], GATE_MODE_HALF_STEP, MAX_GATE_MODE_VALUE);
    _stepRepeat[i] = constrain(settings->stepRepeat[i], MIN_STEP_REPEAT, MAX_STEP_REPEAT);
  }
}

void Sequence::onRunningChange(BoolChangedHandler handler) {
  _onRunningChangedHandler = handler;
}

void Sequence::onGateChange(BoolChangedHandler handler) {
  _onGateChangedHandler = handler;
}

void Sequence::onTriggerChange(BoolChangedHandler handler) {
  _onTriggerChangedHandler = handler;
}

void Sequence::onSelectedStepChange(ByteChangedHandler handler) {
  _onSelectedStepChangedHandler = handler;
}

void Sequence::onSequenceModeChange(ByteChangedHandler handler) {
  _onSequenceModeChangedHandler = handler;
}

void Sequence::_selectStep(byte newSelectedStep) {
  if (_currentStep == newSelectedStep) return;
  _currentStep = newSelectedStep;
  (*_onSelectedStepChangedHandler)(newSelectedStep);
}

// TODO: hardcode sequences, instead of procedural
// TODO: random sequence should be randomly generated 16 step sequence,
// generated at time of setting mode to random
byte Sequence::_getNextStepIndexInSequence() {
  byte oldStep = _currentStep;
  byte previousStep = _previousStep;

  byte nextStep = 0;

  switch (_sequenceMode) {
    // Moving forward; simple 0..7, returning to 0 when past the last step
    // 0,1,2,3,4,5,6,7
    case SEQUENCE_MODE_FORWARD:
      nextStep = oldStep + 1;
      if (nextStep >= NUM_STEPS) {
        nextStep = 0;
      }
      break;

    // Reverse: 7..0, back to 7 when past 0
    // 7,6,5,4,3,2,1,0
    case SEQUENCE_MODE_REVERSE:
      if (oldStep > 0) {
        nextStep = oldStep - 1;
      } else {
        nextStep = NUM_STEPS - 1;
      }
      break;

    // Forward, then backward
    // 0,1,2,3,4,5,6,7,6,5,4,3,2,1
    case SEQUENCE_MODE_BACK_AND_FORTH:
      if (previousStep < oldStep) {
        nextStep = oldStep + 1;
        if (nextStep >= NUM_STEPS) {
          nextStep = oldStep - 1;
        }
      } else {
        if (oldStep > 0) {
          nextStep = oldStep - 1;
        } else {
          nextStep = oldStep + 1;
        }
      }
      break;

    // Even steps, then odd steps:
    // 0,2,4,6,1,3,5,7
    case SEQUENCE_MODE_ALT_FORWARD:
      nextStep = oldStep + 2;
      if (nextStep == NUM_STEPS) {
        nextStep = 1;
      } else if (nextStep > NUM_STEPS) {
        nextStep = 0;
      }
      break;

    // The above, reversed
    // 7,5,3,1,6,4,2,0
    case SEQUENCE_MODE_ALT_REVERSE:
      if (oldStep > 1) {
        nextStep = oldStep - 2;
      } else if (oldStep == 1) {
        nextStep = NUM_STEPS - 2;
      } else if (oldStep == 0) {
        nextStep = NUM_STEPS - 1;
      }
      break;

    // The above two in order
    // 0,2,4,6,1,3,5,7,6,4,2,0,7,5,3,1
    case SEQUENCE_MODE_ALT_BACK_AND_FORTH_A:
      if (oldStep == 0) {
        if (previousStep > 1) {
          nextStep = NUM_STEPS - 1;
        } else {
          nextStep = oldStep + 2;
        }
      } else if (oldStep == 1) {
        if (previousStep == NUM_STEPS - 2) {
          nextStep = oldStep + 2;
        } else {
          nextStep = 0;
        }
      } else if (oldStep == NUM_STEPS - 2) {
        if (previousStep > oldStep) {
          nextStep = oldStep - 2;
        } else {
          nextStep = 1;
        }
      } else if (oldStep == NUM_STEPS - 1) {
        if (previousStep == 0) {
          nextStep = oldStep - 2;
        } else {
          nextStep = NUM_STEPS - 2;
        }
      } else if (previousStep < oldStep) {
        nextStep = oldStep + 2;
      } else {
        nextStep = oldStep - 2;
      }
      break;

    // Even forward, odd backward
    // 0,2,4,6,7,5,3,1
    case SEQUENCE_MODE_ALT_BACK_AND_FORTH_B:
      if (oldStep == 0) {
        nextStep = oldStep + 2;
      } else if (oldStep == NUM_STEPS - 2) {
        nextStep = NUM_STEPS - 1;
      } else if (oldStep == NUM_STEPS -1) {
        nextStep = oldStep - 2;
      } else if (oldStep == 1) {
        nextStep = 0;
      } else if (previousStep < oldStep) {
        nextStep = oldStep + 2;
      } else {
        nextStep = oldStep - 2;
      }
      break;

    // Random step each time, but ensures not the same step is chosen twice in a row
    case SEQUENCE_MODE_RANDOM:
      do {
        nextStep = random(NUM_STEPS);
      } while (nextStep == oldStep);
      break;
  }

  return nextStep;
}

void Sequence::_setTrigger(bool on) {
  if (_trigger == on) return;

  _trigger = on;
  (*_onTriggerChangedHandler)(on);
}

void Sequence::_advanceSubStep() {
  _firstHalfOfStep = !_firstHalfOfStep;
  bool firstHalf = _firstHalfOfStep;
  bool gateWasOn = _gate;
  byte repeatsForThisStep = _stepRepeat[Sequence::_currentStep];

  if (firstHalf) {
    if (_currentStepRepetition >= repeatsForThisStep) {
      _advanceSequence();
      _currentStepRepetition = 0;
    }

    while (_stepRepeat[_currentStep] == 0) {
      _advanceSequence();
      _currentStepRepetition = 0;
    }

    _currentStepRepetition++;

    (*_onSelectedStepChangedHandler)(_currentStep);
  }

  switch (_gateMode[_currentStep]) {
    case GATE_MODE_HALF_STEP:
      setGate(
        _firstHalfOfStep && _currentStepRepetition == 1);
      break;

    case GATE_MODE_FULL_STEP:
      setGate(_currentStepRepetition == 1);
      break;

    case GATE_MODE_REPEAT_HALF:
      setGate(_firstHalfOfStep);
      break;

    case GATE_MODE_REPEAT_FULL:
      setGate(true);
      break;

    case GATE_MODE_SILENT:
      setGate(false);
      break;
  }

  if ((_gate && firstHalf && _currentStepRepetition == 1) || (_gate && !gateWasOn && firstHalf)) {
    _setTrigger(true);
  } else if (!firstHalf) {
    _setTrigger(false);
  }
}

void Sequence::_advanceSequence() {
  byte oldStep = _currentStep;
  byte nextStep = _getNextStepIndexInSequence();

  _previousStep = oldStep;
  _selectStep(nextStep);
}
