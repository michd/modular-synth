#include "io.h"
#include <Arduino.h>

volatile bool IO::_spiBusy = false;
volatile Tasks IO::_taskQueue[MAX_TASK_QUEUE_LENGTH];
volatile uint8_t IO::_taskQueueLength = 0;
volatile unsigned int IO::_queuedDacValue = 0;
volatile uint8_t IO::_cachedSelectedStep = 0;
String IO::_queuedDisplayValue;
uint16_t IO::_queuedLedsValue = 0x0000;
bool IO::_arrowButtonHandlerSetup = false;
volatile bool IO::_chainOutputCache = false;

AdcReadHandler IO::_adcReadHandler;

ButtonPressedHandler IO::_sequenceModeButtonPressedHandler;
ButtonPressedHandler IO::_gateButtonPressedHandler;
ButtonPressedHandler IO::_repeatButtonPressedHandler;
ButtonPressedHandler IO::_runModeButtonPressedHandler;
ButtonPressedHandler IO::_scaleButtonPressedHandler;
ButtonPressedHandler IO::_resetButtonPressedHandler;
ButtonPressedHandler IO::_loadButtonPressedHandler;
ButtonPressedHandler IO::_saveButtonPressedHandler;

ArrowButtonPressedHandler IO::_minNoteArrowButtonPressedHandler;
ArrowButtonPressedHandler IO::_maxNoteArrowButtonPressedHandler;
ArrowButtonPressedHandler IO::_timeDivisionArrowButtonPressedHandler;

PinChangedHandler IO::_chainInputChangedHandler;

TickHandler IO::_externalClockTickHandler;

MAX72S21 IO::_display(PIN_SPI_CS_DISP);
MCP23S17 IO::_portExp(PORT_EXPANDER_CHANNEL, PIN_SPI_CS_PORTEXP);
MCP3202 IO::_adc(PIN_SPI_CS_ADC);
MCP492X IO::_dac(PIN_SPI_CS_DAC);

void IO::init() {
  ::pinMode(PIN_RUNNING_INDICATOR, OUTPUT);
  ::pinMode(PIN_GATE_OUT, OUTPUT);
  ::pinMode(PIN_TRIGGER_OUT, OUTPUT);
  ::pinMode(PIN_STEP_ADDR_A, OUTPUT);
  ::pinMode(PIN_STEP_ADDR_B, OUTPUT);
  ::pinMode(PIN_STEP_ADDR_C, OUTPUT);
  ::pinMode(PIN_STEP_ENABLE, OUTPUT);

  pinMode(PIN_PORTEXP_INTERRUPT, INPUT_PULLUP);

  ::digitalWrite(PIN_RUNNING_INDICATOR, 0);
  ::digitalWrite(PIN_GATE_OUT, 0);

  // [1] Configure port expander
  _portExp.begin();
  // Configure pins on port expander
  // Port A = buttons per step
  uint8_t portAmodes = 0b00011111; // Change as needed
  //                       ||||||- PORTEXP_PIN_UP_ARROW                  [*]
  //                       |||||- PORTEXP_PIN_DOWN_ARROW                 [*]
  //                       ||||- PORTEXP_PIN_PARAM_SELECT_A
  //                       |||- PORTEXP_PIN_PARAM_SELECT_B
  //                       ||- PORTEXP_PIN_CHAIN_INPUT                   [*]
  //                       |- PORTEXP_PIN_CHAIN_OUTPUT
  // Port B = various buttons
  uint8_t portBmodes = 0b11111111; // Change as needed
  //                     ||||||||- PORTEXP_PIN_GATE_MODE_SELECT_BUTTON   [*]
  //                     |||||||- PORTEXP_PIN_REPEAT_SELECT_BUTTON       [*]
  //                     ||||||- PORTEXP_PIN_SEQUENCE_MODE_SELECT_BUTTON [*]
  //                     |||||- PORTEXP_PIN_RUN_STOP_BUTTON              [*]
  //                     ||||- PORTEXP_PIN_SCALE_BUTTON                  [*]
  //                     |||- PORTEXP_PIN_RESET_BUTTON                   [*]
  //                     ||- PORTEXP_PIN_LOAD_BUTTON                     [*]
  //                     |- PORTEXP_PIN_SAVE_BUTTON                      [*]
  // [*] - interrupt attached
  uint16_t combinedModes = (portBmodes << 8) | portAmodes;
  _portExp.pinMode(combinedModes);
  // Set all inputs to pull-up (buttons normal-open to gnd)
  // This means we can re-use the portmodes word
  _portExp.pullupMode(combinedModes);
  
  _portExp.inputInvert(0x0000);
  // Cache initial values
  _portExp.digitalRead();

  // Assign no-op handlers to prevent potentially calling handlers that were
  // not assigned (causing undefined behavior or segfaults)
  _minNoteArrowButtonPressedHandler = _noopArrowButtonPressedHandler;
  _maxNoteArrowButtonPressedHandler = _noopArrowButtonPressedHandler;
  _timeDivisionArrowButtonPressedHandler = _noopArrowButtonPressedHandler;

  // Set up interrupts with the port expander
  ::attachInterrupt(
    digitalPinToInterrupt(PIN_PORTEXP_INTERRUPT),
    _processPortExpanderInterrupt,
    FALLING);

  // Set up interrupt for external clock
  ::attachInterrupt(
    digitalPinToInterrupt(PIN_EXTERNAL_CLOCK),
    _processExternalClockTick,
    RISING);

  // Set up other devices
  _adc.begin();
  _dac.begin();
  _display.begin();

  // Configure display
  _display.setDecodeMode(0x00);
  _display.setIntensity(0x5);
  _display.setScanLimit(4);
  _display.clear();
  _display.activate();

  _dac.analogWrite(DAC_CENTER_VALUE); // 0V

#ifdef DEBUGLOGGING  
  Serial.begin(250000);
#endif
}

void IO::setGate(bool on) {
  ::digitalWrite(PIN_GATE_OUT, on);
  // Trigger should never remain on when gate is off
  if (!on) ::digitalWrite(PIN_TRIGGER_OUT, false);
}

void IO::setTrigger(bool on) { ::digitalWrite(PIN_TRIGGER_OUT, on); }

void IO::setRunningIndicator(bool on) {
  ::digitalWrite(PIN_RUNNING_INDICATOR, on);
}

void IO::setStep(uint8_t step) {
  uint8_t a = step & B001;
  uint8_t b = step & B010;
  uint8_t c = step & B100;

  // Disable 3-to-8 decoded while writing to all lines
  // Prevents flickering of different values on the LEDS
  // This is one of the things that would be more efficient if writing directly
  // to the microcontroller's port instead of using arduino's digitalWrite
  ::digitalWrite(PIN_STEP_ENABLE, 0);
  ::digitalWrite(PIN_STEP_ADDR_A, a);
  ::digitalWrite(PIN_STEP_ADDR_B, b);
  ::digitalWrite(PIN_STEP_ADDR_C, c);

  // Re-enable decoder
  ::digitalWrite(PIN_STEP_ENABLE, 1);
}

void IO::setPitch(uint16_t newPitch) {
  _queuedDacValue = newPitch;
  _queueTask(WRITE_DAC);
}

void IO::readAdc(AdcReadHandler handler) {
  _adcReadHandler = handler;
  _queueTask(READ_ADC);
}

void IO::readPortExp() {
  _queueTask(READ_PORTEXP);
}

void IO::readSelectedStep() {
  _queueTask(READ_SELECTED_STEP);
}

uint8_t IO::getSelectedStep() {
  return _cachedSelectedStep;
}

uint16_t IO::readPortExpCache() {
  return _portExp.digitalReadCache();
}

bool IO::getPortExpPin(uint8_t pin) {
  return _portExp.digitalReadCache(pin);
}

uint8_t IO::getSelectedParam() {
  bool paramA = _portExp.digitalReadCache(PORTEXP_PIN_PARAM_SELECT_A);
  bool paramB = _portExp.digitalReadCache(PORTEXP_PIN_PARAM_SELECT_B);

  return (paramA << 1) | paramB;
}

void IO::writeDisplay(String text) {
  _queuedDisplayValue = text;
  _queueTask(WRITE_DISPLAY);
}

void IO::writeLeds(LEDs ledConfig) {
  switch (ledConfig) {    
    case INDICATOR_STEP_NOTE: _queuedLedsValue = LED_INDICATOR_STEP_NOTE; break;
    case INDICATOR_MIN_NOTE: _queuedLedsValue = LED_INDICATOR_MIN_NOTE; break;
    case INDICATOR_MAX_NOTE: _queuedLedsValue = LED_INDICATOR_MAX_NOTE; break;
    case INDICATOR_NONE:
    default:
      _queuedLedsValue = LED_INDICATOR_NONE;
      break;
  }

  _queueTask(WRITE_LEDS);
}

void IO::setChainOut(bool on) {
  _portExp.digitalWrite(PORTEXP_PIN_CHAIN_OUTPUT, on);
  // Not using port expander class cache here, as that cache returns
  // only input cache, not the output cache, and checking against
  // modes to return from output cache where relevant would add
  // unnecessary overhead to the very frequence common input lookups.
  // Hence, this single output is cached here.
  _chainOutputCache = on;
}

bool IO::getChainOut() {
  return _chainOutputCache;
}

void IO::onSequenceModeButtonPressed(ButtonPressedHandler handler) {
  _sequenceModeButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_SEQUENCE_MODE_SELECT_BUTTON,
    _internalHandleSequenceModeButtonPressed,
    FALLING);
}

void IO::onGateButtonPressed(ButtonPressedHandler handler) {
  _gateButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_GATE_MODE_SELECT_BUTTON,
    _internalHandleGateButtonPressed,
    FALLING);
}

void IO::onRepeatButtonPressed(ButtonPressedHandler handler) {
  _repeatButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_REPEAT_SELECT_BUTTON,
    _internalHandleRepeatButtonPressed,
    FALLING);
}

void IO::onRunModeButtonPressed(ButtonPressedHandler handler) {
  _runModeButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_RUN_MODE_BUTTON,
    _internalHandleRunModeButtonPressed,
    FALLING);
}

void IO::onScaleButtonPressed(ButtonPressedHandler handler) {
  _scaleButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_SCALE_BUTTON,
    _internalHandleScaleButtonPressed,
    FALLING);
}

void IO::onResetButtonPressed(ButtonPressedHandler handler) {
  _resetButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_RESET_BUTTON,
    _internalHandleResetButtonPressed,
    FALLING);
}

void IO::onLoadButtonPressed(ButtonPressedHandler handler) {
  _loadButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_LOAD_BUTTON,
    _internalHandleLoadButtonPressed,
    FALLING);
}

void IO::onSaveButtonPressed(ButtonPressedHandler handler) {
  _saveButtonPressedHandler = handler;

  _portExp.attachInterrupt(
    PORTEXP_PIN_SAVE_BUTTON,
    _internalHandleSaveButtonPressed,
    FALLING);
}

void IO::onMinNoteArrowButtonPressed(ArrowButtonPressedHandler handler) {
  _minNoteArrowButtonPressedHandler = handler;
  if (!_arrowButtonHandlerSetup) _setupArrowButtonHandler();
}

void IO::onMaxNoteArrowButtonPressed(ArrowButtonPressedHandler handler) {
  _maxNoteArrowButtonPressedHandler = handler;
  if (!_arrowButtonHandlerSetup) _setupArrowButtonHandler();
}

void IO::onTimeDivisionArrowButtonPressed(ArrowButtonPressedHandler handler) {
  _timeDivisionArrowButtonPressedHandler = handler;
  if (!_arrowButtonHandlerSetup) _setupArrowButtonHandler();
}

void IO::onChainInputChanged(PinChangedHandler handler) {
  _chainInputChangedHandler = handler;
  _portExp.attachInterrupt(
    PORTEXP_PIN_CHAIN_INPUT,
    _internalHandleChainInputChanged,
    CHANGE);
}

void IO::onExternalClockTick(TickHandler handler) {
  _externalClockTickHandler = handler;
}

void IO::_processPortExpanderInterrupt() {
  _queueTask(PROCESS_PORTEXP_INTERRUPT);
}

void IO::_processExternalClockTick() {
  _externalClockTickHandler();
}

// Adds a new task to the queue, or executes it instantly if the queue is empty
void IO::_queueTask(Tasks task) {
  // If queue is empty and spi isn't busy, execute it right away
  if (_taskQueueLength == 0 && !_spiBusy) {
    return _executeTask(task);
  }

  // Don't double-queue
  if (_taskQueueContainsTask(task)) return;

  bool highPriority = (task == PROCESS_PORTEXP_INTERRUPT);
  bool lowPriority = (task == READ_PORTEXP || task == READ_SELECTED_STEP);
  bool containsLowPriority = _taskQueue[_taskQueueLength - 1] == READ_PORTEXP;

  // High priority tasks should always go at position 0
  if (highPriority) {
    _taskQueueInsertAt(task, 0);
    return;
  }

  // If there is a low priority task, insert the new task before it
  if (containsLowPriority) {
    _taskQueueInsertAt(task, _taskQueueLength - 1);
    return;
  }

  // Otherwise, just add the task at the end of the queue
  if (_taskQueueLength < MAX_TASK_QUEUE_LENGTH) {
    _taskQueueInsertAt(task, _taskQueueLength);
    return;
  }

  // Otherwise, don't add the task at all, as the queue is full.
  // Note: this should never actually happen.
}

// Inserts a task in the queue, moving other tasks down to make room
void IO::_taskQueueInsertAt(Tasks task, uint8_t index) {
  constrain(index, 0, MAX_TASK_QUEUE_LENGTH - 1);
  uint8_t firstBlank = index;

  // Locate earliest blank task in queue to find where to start shifting tasks
  while (firstBlank < MAX_TASK_QUEUE_LENGTH) {
    if (_taskQueue[firstBlank] == BLANK) break;
    firstBlank++;
  }

  // Move all tasks down one
  for (uint8_t j = firstBlank; j > index; j--) {
    if (j < MAX_TASK_QUEUE_LENGTH) {
      _taskQueue[j] = _taskQueue[j - 1];
    }
  }

  // Inser the new task at the given index
  _taskQueue[index] = task;

  // If the queue wasn't full to begin with, increment length
  if (_taskQueueLength < MAX_TASK_QUEUE_LENGTH) _taskQueueLength++;
}

bool IO::_taskQueueContainsTask(Tasks task) {
  for (uint8_t i = 0; i < _taskQueueLength; i++) {
    if (_taskQueue[i] == task) return true;
  }

  return false;
}

// Take the first task from the queue, shift the remainders up, and execute
// the task.
void IO::_processQueuedTask() {
  Tasks taskToRun = _taskQueue[0];

  // Shift everything up after removing the prioritized task
  for (uint8_t i = 0; i < _taskQueueLength - 1; i++) {
    _taskQueue[i] = _taskQueue[i + 1];
  }

  _taskQueueLength--;

  // Ensure vacated positions are filled with "blank" tasks
  for (uint8_t i = _taskQueueLength; i < MAX_TASK_QUEUE_LENGTH; i++) {
    _taskQueue[i] = BLANK;
  }

  // Run the extracted task
  _executeTask(taskToRun);
}

// Callback for when we finish any task; if there are more, move on to the next.
void IO::_taskFinished() {
  if (_taskQueueLength > 0) {
    _processQueuedTask();
  }
}

// Figure out which function to run based on the enum value of the task
void IO::_executeTask(Tasks task) {
  switch (task) {
    case READ_ADC: return _taskReadAdc();
    case READ_SELECTED_STEP: return _taskReadSelectedStep();
    case WRITE_DAC: return _taskWriteDac();
    case READ_PORTEXP: return _taskReadPortExp();
    case PROCESS_PORTEXP_INTERRUPT: return _taskProcessPortExpInterrupt();
    case WRITE_DISPLAY: return _taskWriteDisplay();
    case WRITE_LEDS: return _taskWriteLeds();
    default: return;
  }
}

// Read from the ADC, and invoke the callback function with the value read
void IO::_taskReadAdc() {
  // Paranoid check
  if (_spiBusy) return;

  _spiBusy = true;
  uint16_t adcValue = _adc.analogRead(ADC_CV_CHANNEL);
  _spiBusy = false;

  _adcReadHandler(adcValue);

  _taskFinished();
}

// Read from the ADC and figure out selected step
void IO::_taskReadSelectedStep() {
  if (_spiBusy) return;

  _spiBusy = true;
  uint16_t adcValue = _adc.analogRead(ADC_STEP_CHANNEL);
  _spiBusy = false;

  uint8_t selectedStep = 0;

  while (adcValue > ((selectedStep * _stepSize) + _halfStepSize)) {
    selectedStep++;
  }

  if (selectedStep > NUM_STEPS) selectedStep = NUM_STEPS;

  _cachedSelectedStep = selectedStep;

  _taskFinished();
}

// Write a value to the DAC; value is stored in _queuedDacValue
void IO::_taskWriteDac() {
  // Paranoid check
  if (_spiBusy) return;

  _spiBusy = true;
  _dac.analogWrite(_queuedDacValue);
  _spiBusy = false;

  _taskFinished();
}

// Update the cached values of the port expanded, so we can read from cache
// whenever later
void IO::_taskReadPortExp() {
  // Paranoid check
  if (_spiBusy) return;

  _spiBusy = true;
  _portExp.digitalRead();
  _spiBusy = false;

  _taskFinished();
}

// Process the incoming interrupt from port expander (a button was pressed)
void IO::_taskProcessPortExpInterrupt() {
  // Paranoid check
  if (_spiBusy) return;

  _spiBusy = true;
  _portExp.processInterrupt();

  // Somewhat redunant but good to have here just in case
  _spiBusy = false;

  _taskFinished();
}

// Write some text to the 7 segment display
void IO::_taskWriteDisplay() {
  // Paranoid check
  if (_spiBusy) return;

  _spiBusy = true;
  _display.print(0, _queuedDisplayValue);
  _spiBusy = false;

  _taskFinished();
}

// Write some indication LEDs, through same chip as 7 segment display
void IO::_taskWriteLeds() {
  // Paranoid check
  if (_spiBusy) return;

  _spiBusy = true;
  _display.setRow(_queuedLedsValue >> 8, _queuedLedsValue & 0xFF);
  _spiBusy = false;

  _taskFinished();
}

void IO::_internalHandleSequenceModeButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _sequenceModeButtonPressedHandler();
}

void IO::_internalHandleGateButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _gateButtonPressedHandler();
}

void IO::_internalHandleRepeatButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _repeatButtonPressedHandler();
}

void IO::_internalHandleRunModeButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _runModeButtonPressedHandler();
}

void IO::_internalHandleScaleButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _scaleButtonPressedHandler(); 
}

void IO::_internalHandleResetButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _resetButtonPressedHandler();  
}

void IO::_internalHandleLoadButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _loadButtonPressedHandler();  
}

void IO::_internalHandleSaveButtonPressed() {
  // spiBusy was set to true in _taskProcessPortExpInterrupt, and we want
  // to mark that done before invoking whatever handler we've got
  _spiBusy = false;
  _saveButtonPressedHandler();  
}

void IO::_setupArrowButtonHandler() {
  if (_arrowButtonHandlerSetup) return;

  _portExp.attachInterrupt(
    PORTEXP_PIN_UP_ARROW,
    _internalHandleUpArrowButtonPressed,
    FALLING);

  _portExp.attachInterrupt(
    PORTEXP_PIN_DOWN_ARROW,
    _internalHandleDownArrowButtonPressed,
    FALLING);

  _arrowButtonHandlerSetup = true;
}

void IO::_internalHandleUpArrowButtonPressed() {
  _handleArrowButtonPressed(true);
}

void IO::_internalHandleDownArrowButtonPressed() {
  _handleArrowButtonPressed(false);
}

void IO::_handleArrowButtonPressed(bool upArrow) {
  uint8_t combinedParams = getSelectedParam();

  switch (combinedParams) {
    case PARAM_MIN_NOTE:
      return _minNoteArrowButtonPressedHandler(upArrow);

    case PARAM_MAX_NOTE:
      return _maxNoteArrowButtonPressedHandler(upArrow);

    case PARAM_TIME_DIVIDER:    
      return _timeDivisionArrowButtonPressedHandler(upArrow);

    // Note: there is no 0b00 (both pins grounded) case as an on-off-on switch
    // cannot turn both sides on at once
    // 0 is on here as the middle pin of the switch is ground, and the pins
    // discussed are in pull-up mode. When they're floating, they're high.
  }
}

void IO::_internalHandleChainInputChanged() {
  _chainInputChangedHandler(getPortExpPin(PORTEXP_PIN_CHAIN_INPUT));
}

void IO::_noopArrowButtonPressedHandler (bool _) {}
