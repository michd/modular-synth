#include "sequence.h"

// Note: this entire class is used in a static way, it is not to be instantiated.

volatile uint8_t Sequence::_currentStep = 0;
volatile uint8_t Sequence::_currentStepRepetition = 0;
volatile uint8_t Sequence::_sequenceMode = SEQUENCE_MODE_FORWARD;
volatile uint8_t Sequence::_gateMode[NUM_STEPS];
volatile uint8_t Sequence::_stepRepeat[NUM_STEPS];
volatile uint8_t Sequence::_timeDivider = DEFAULT_TIME_DIVIDER;
volatile uint8_t Sequence::_indexInSequence = 0;
volatile uint32_t Sequence::_pulsesPerSubstep = (PPQ / 2) / (DEFAULT_TIME_DIVIDER / 4);
volatile bool Sequence::_firstHalfOfStep = true;
volatile uint32_t Sequence::_internalTicks = 0;
volatile RunModes Sequence::_runMode = NOT_RUNNING;
volatile bool Sequence::_running = false;
volatile bool Sequence::_runningIndicator = false;
volatile bool Sequence::_gate = false;
volatile bool Sequence::_trigger = false;
volatile bool Sequence::_chained = false;
BoolChangedHandler Sequence::_onRunningIndicatorChangedHandler;
BoolChangedHandler Sequence::_onRunningChangedHandler;
BoolChangedHandler Sequence::_onGateChangedHandler;
BoolChangedHandler Sequence::_onTriggerChangedHandler;
ByteChangedHandler Sequence::_onSelectedStepChangedHandler;
EventHandler Sequence::_onSequenceEndedHandler;

// Initialize hardcoded sequences
const uint8_t Sequence::_sequences[][MAX_SEQUENCE_LENGTH + 1] = {
  // 1: Simple forward, length 8
  { 0, 1, 2, 3, 4, 5, 6, 7, SEQUENCE_TERMINATOR },
  // 2: Reverse, length 8
  { 7, 6, 5, 4, 3, 2, 1, 0, SEQUENCE_TERMINATOR },
  // 3: Forward, then revers, length 16
  { 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0, SEQUENCE_TERMINATOR },
  // 4: Even steps, then odd steps, length 8
  { 0, 2, 4, 6, 1, 3, 5, 7, SEQUENCE_TERMINATOR },
  // 5: The above, reversed, length 8
  { 7, 5, 3, 1, 6, 4, 2, 0, SEQUENCE_TERMINATOR },
  // 6: The above two in order kinda, length 8
  { 0, 2, 4, 6, 1, 3, 5, 7, 6, 4, 2, 0, 7, 5, 3, 1, SEQUENCE_TERMINATOR },
  // 7: Even step forwared, odd steps backwards, length 8
  { 0, 2, 4, 6, 7, 5, 3, 1, SEQUENCE_TERMINATOR },
  // 8: Random steps, generated when random mode is selected, length 16
  { SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS,
    SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS, SEQ_RS,
    SEQUENCE_TERMINATOR }
};

volatile uint8_t Sequence::_selectedSequence[MAX_SEQUENCE_LENGTH + 1]; 

void Sequence::init() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    _gateMode[i] = GATE_MODE_HALF_STEP;
    _stepRepeat[i] = 1;
  }

  _initSequence(SEQUENCE_MODE_FORWARD);
}

void Sequence::tick() {
  // If waiting, just blink the indicator LED
  if (_runMode == WAITING) {
    // Blink at 1/16 time essentially, in waiting mode
    static const uint32_t blinkPulsesPerSubstep = (PPQ / 2) / (4 / 4);
    // blinkPulses is only relevant to tick() but needs maintaining between calls
    // hence it's static
    static uint32_t blinkPulses;

    blinkPulses++;

    if (blinkPulses >= blinkPulsesPerSubstep) {
      blinkPulses = 0;
      _setRunningIndicator(!_runningIndicator);
    }
  }

  if (!_running) return;

  uint32_t ticks = _internalTicks;
  ticks++;

  if (ticks >= _pulsesPerSubstep) {
    ticks = 0;
    _advanceSubStep();
  }

  _internalTicks = ticks;
}

// Only care about this if we were waiting, if so, switch to running mode
void Sequence::chainTrigger() {
  if (_runMode == WAITING) {
    _runMode = RUNNING;
    _setRunning(true);
    _setRunningIndicator(true);
  }
}

void Sequence::toggleRunMode() {
  // Based on current state, set next run mode
  switch (_runMode) {
    case NOT_RUNNING:
      _runMode = _chained ? WAITING : RUNNING;
      break;

    case RUNNING:
      _runMode = NOT_RUNNING;
      break;

    case WAITING:
      _runMode = RUNNING;
      break;
  }

  _resetSequencePosition();

  _setRunning(_runMode == RUNNING);

  // If we're not running (anymore) ensure gate and trigger are low
  if (!_running) {    
    setGate(false);
    _setTrigger(false);
  }

  // Update indicator output
  // Will ensure it blinks in tick() when waiting
  _setRunningIndicator(_runMode == RUNNING);
}

bool Sequence::getChained() { return _chained; }

void Sequence::setChained(bool chained) {
  _chained = chained;
}

bool Sequence::isRunning() {
  return _running;
}

uint8_t Sequence::setSequenceMode(uint8_t newSequenceMode) {
  if (_sequenceMode == newSequenceMode) {
    if (_sequenceMode == SEQUENCE_MODE_RANDOM) _initSequence(_sequenceMode);
    return _sequenceMode;
  }

  _sequenceMode = newSequenceMode;
  _initSequence(_sequenceMode);
  return _sequenceMode;
}

void Sequence::selectStep(uint8_t newSelectedStep) {
  _selectStep(constrain(newSelectedStep, 0, NUM_STEPS - 1));
}

uint8_t Sequence::getSelectedStep() {
  return _currentStep;
}

void Sequence::setGate(bool on) {
  if (_gate == on) return;

  _gate = on;
  (*_onGateChangedHandler)(on);

  if (on) _setTrigger(true);
}

void Sequence::setTimeDivider(uint8_t newDivider) {
  _timeDivider = constrain(newDivider, 1, 16);
  _pulsesPerSubstep = (uint16_t)((double)(PPQ / 2) / ((double)newDivider / (double)4));
}

uint8_t Sequence::cycleTimeDivider(bool higher) {
  uint8_t newTimeDivider;

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

uint8_t Sequence::setGateModeForStep(uint8_t step, uint8_t gateMode) {
  step = constrain(step, 0, NUM_STEPS - 1);
  gateMode = constrain(gateMode, 0, MAX_GATE_MODE_VALUE);

  _gateMode[step] = gateMode;
  return gateMode;
}

uint8_t Sequence::cycleGateModeForStep(uint8_t step) {
  step = constrain(step, 0, NUM_STEPS - 1);
  uint8_t gateMode = _gateMode[step];

  gateMode++;
  if (gateMode > MAX_GATE_MODE_VALUE) gateMode = 0;
  _gateMode[step] = gateMode;
  return gateMode;
}

uint8_t Sequence::setStepRepeatForStep(uint8_t step, uint8_t repetitions) {
  step = constrain(step, 0, NUM_STEPS - 1);
  repetitions = constrain(repetitions, MIN_STEP_REPEAT, MAX_STEP_REPEAT);

  _stepRepeat[step] = repetitions;
  return repetitions;
}

uint8_t Sequence::cycleStepRepeatForStep(uint8_t step) {
  step = constrain(step, 0, NUM_STEPS - 1);
  uint8_t stepRepetitions = _stepRepeat[step];

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
  uint8_t timeDivider = settings->timeDivider;
  
  // Tidy up timeDivider to ensure it only has a single ON bit
  for (uint8_t tdBit = 0; tdBit < 8; tdBit++) {
    uint8_t bitMask = 1 << (7 - tdBit);
    if (timeDivider & bitMask) {
      timeDivider &= bitMask;
      break;
    }
  }

  setTimeDivider(constrain(settings->timeDivider, MIN_TIME_DIVIDER, MAX_TIME_DIVIDER));
  _sequenceMode = constrain(settings->sequenceMode, SEQUENCE_MODE_FORWARD, SEQUENCE_MODE_RANDOM);
  _initSequence(_sequenceMode);

  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    _gateMode[i] = constrain(settings->gateModes[i], GATE_MODE_HALF_STEP, MAX_GATE_MODE_VALUE);
    _stepRepeat[i] = constrain(settings->stepRepeat[i], MIN_STEP_REPEAT, MAX_STEP_REPEAT);
  }
}

void Sequence::onRunningIndicatorChange(BoolChangedHandler handler) {
  _onRunningIndicatorChangedHandler = handler;
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

void Sequence::onSequenceEnd(EventHandler handler) {
  _onSequenceEndedHandler = handler;
}

void Sequence::_setRunningIndicator(bool on) {
  if (_runningIndicator == on) return;
  _runningIndicator = on;
  (*_onRunningIndicatorChangedHandler)(on);
}

void Sequence::_setRunning(bool on) {
  if (_running == on) return;
  _running = on;
  (*_onRunningChangedHandler)(on);
}

void Sequence::_selectStep(uint8_t newSelectedStep) {
  if (_currentStep == newSelectedStep) return;
  _currentStep = newSelectedStep;
  (*_onSelectedStepChangedHandler)(newSelectedStep);
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
  uint8_t repeatsForThisStep = _stepRepeat[_currentStep];

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
  } else if (_chained && _isOnLastStep()) {
    // TODO: it will need verifying if this is the right place to
    // trigger this event. I think it is.
    // I believe this will trigger it on the substep  just prior to wrapping
    // around to the beginning, leading to the next sequencer seamlessly picking
    // up where this one left off 
    (*_onSequenceEndedHandler)();
    // We're chained, so when sequence is complete, switch to waiting mode
    _runMode = WAITING;
    _setRunning(false);
    _setRunningIndicator(false);
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
  if (_selectedSequence[++_indexInSequence] == SEQUENCE_TERMINATOR) {
    _indexInSequence = 0;
  }

  _selectStep(_selectedSequence[_indexInSequence]);
}

bool Sequence::_isOnLastStep() {
  if (_selectedSequence[_indexInSequence + 1] == SEQUENCE_TERMINATOR) {
    return _stepRepeat[_currentStep] == _currentStepRepetition;
  }

  uint8_t index = _indexInSequence + 1;

  while (_selectedSequence[index] != SEQUENCE_TERMINATOR) {
    if (_stepRepeat[_selectedSequence[index]] != 0) return false;
    index++;
  }

  return true;
}

void Sequence::_initSequence(uint8_t sequenceMode) {
  sequenceMode = constrain(sequenceMode, MIN_SEQUENCE_MODE, MAX_SEQUENCE_MODE);

  if (sequenceMode == SEQUENCE_MODE_RANDOM) {
    ::randomSeed(::millis());
  }

  uint8_t *sequence = _sequences[sequenceMode];

  uint8_t previousStep = SEQUENCE_TERMINATOR;
  uint8_t i;
  bool reachedEnd = false;

  // Copies over the values of the selected sequence,
  // and fills remaining space with sequence terminator
  for (i = 0; i <= MAX_SEQUENCE_LENGTH; i++) {
    if (reachedEnd) {
      _selectedSequence[i] = SEQUENCE_TERMINATOR;
      continue;
    }

    if (sequence[i] == SEQUENCE_RANDOM_STEP) {
      _selectedSequence[i] = _generateRandomStep(previousStep);
      previousStep = _selectedSequence[i];
    } else {
      _selectedSequence[i] = sequence[i];
    }

    if (_selectedSequence[i] == SEQUENCE_TERMINATOR) reachedEnd = true;
  }

  _selectedSequence[i] = SEQUENCE_TERMINATOR;
  _resetSequencePosition();
}

uint8_t Sequence::_generateRandomStep(uint8_t previousStep) {
  uint8_t nextStep;

  do {
    nextStep = random(NUM_STEPS);
  } while (nextStep == previousStep);

  return nextStep;
}

void Sequence::_resetSequencePosition() {
  _indexInSequence = MAX_SEQUENCE_LENGTH - 1;
  _selectStep(_selectedSequence[0]);
}
