#include "io.h"
#include <Arduino.h>

volatile bool IO::_spiBusy = false;
volatile Tasks IO::_taskQueue[MAX_TASK_QUEUE_LENGTH];
volatile byte IO::_taskQueueLength = 0;
volatile unsigned int IO::_queuedDacValue = 0;
char *IO::_queuedDisplayValue;
AdcReadHandler IO::_adcReadHandler;

ButtonPressedHandler IO::_gateButtonPressedHandler;
ButtonPressedHandler IO::_repeatButtonPressedHandler;
ButtonPressedHandler IO::_runStopButtonPressedHandler;

MAX72S19 IO::_display(PIN_SPI_CS_DISP);
MCP23S17 IO::_portExp(PORT_EXPANDER_CHANNEL, PIN_SPI_CS_PORTEXP);
MCP3202 IO::_adc(PIN_SPI_CS_ADC);
MCP492X IO::_dac(PIN_SPI_CS_DAC);

void IO::init() {
  ::pinMode(PIN_RUNSTOP_BUTTON, INPUT_PULLUP);
  ::pinMode(PIN_RUNNING, OUTPUT);
  ::pinMode(PIN_GATE_OUT, OUTPUT);
  ::pinMode(PIN_TRIGGER_OUT, OUTPUT);
  ::pinMode(PIN_STEP_ADDR_A, OUTPUT);
  ::pinMode(PIN_STEP_ADDR_B, OUTPUT);
  ::pinMode(PIN_STEP_ADDR_C, OUTPUT);
  ::pinMode(PIN_STEP_ENABLE, OUTPUT);

  pinMode(PIN_PORTEXP_INTERRUPT, INPUT_PULLUP);

  ::digitalWrite(PIN_RUNNING, 0);
  ::digitalWrite(PIN_GATE_OUT, 0);

  // [1] Configure port expander
  _portExp.begin();
  // Configure pins on port expander
  // Port A = buttons per step
  byte portAmodes = 0xFF; // All pins on port A are input
  // Port B = various buttons
  byte portBmodes = 0b00000111; // Change as needed
  //                          |||- PORTEXP_PIN_GATE_MODE_SELECT_BUTTON [*]
  //                          ||- PORTEXP_PIN_REPEAT_SELECT_BUTTON     [*]
  //                          |- PORTEXP_PIN_MODE_SELECT_BUTTON
  // [*] - interrupt attached
  word combinedModes = (portBmodes << 8) | portAmodes;
  _portExp.pinMode(combinedModes);
  // Set all inputs to pull-up (buttons normal-open to gnd)
  // This means we can re-use the portmodes word
  _portExp.pullupMode(combinedModes);
  
  _portExp.inputInvert(0xFF);
  // Cache initial values
  _portExp.digitalRead();

  // Set up interrupts with the port expander
  ::attachInterrupt(
    digitalPinToInterrupt(PIN_PORTEXP_INTERRUPT),
    _processPortExpanderInterrupt,
    FALLING);

  // Set up interrupt for runstop button (TODO, move to port expander)
  ::attachInterrupt(
    digitalPinToInterrupt(PIN_RUNSTOP_BUTTON),
    _processRunStopPressedInterrupt,
    FALLING);

  // Set up other devices
  _adc.begin();
  _dac.begin();
  _display.begin();

  // Configure display
  _display.setDecodeMode(0x00);
  _display.setIntensity(0xF);
  _display.setScanLimit(3);
  _display.clear();
  _display.activate();

  _dac.analogWrite(DAC_CENTER_VALUE); // 0V
  Serial.begin(250000);
}

void IO::setGate(bool on) {
  ::digitalWrite(PIN_GATE_OUT, on);
  // Trigger should never remain on when gate is off
  if (!on) ::digitalWrite(PIN_TRIGGER_OUT, false);
}

void IO::setTrigger(bool on) { ::digitalWrite(PIN_TRIGGER_OUT, on); }
void IO::setRunning(bool on) { ::digitalWrite(PIN_RUNNING, on); }

void IO::setStep(byte step) {
  byte a = step & B001;
  byte b = step & B010;
  byte c = step & B100;

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

void IO::setPitch(unsigned int newPitch) {
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

word IO::readPortExpCache() {
  return _portExp.digitalReadCache();
}

bool IO::getPortExpPin(byte pin) {
  return _portExp.digitalReadCache(pin);
}

void IO::writeDisplay(char *characters) {
  _queuedDisplayValue = characters;
  _queueTask(WRITE_DISPLAY);
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

void IO::onRunStopButtonPressed(ButtonPressedHandler handler) {
  _runStopButtonPressedHandler = handler;
}

void IO::_processRunStopPressedInterrupt() {
  _runStopButtonPressedHandler();
}

void IO::_processPortExpanderInterrupt() {
  _queueTask(PROCESS_PORTEXP_INTERRUPT);
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
  bool lowPriority = (task == READ_PORTEXP);
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
void IO::_taskQueueInsertAt(Tasks task, byte index) {
  constrain(index, 0, MAX_TASK_QUEUE_LENGTH - 1);
  byte firstBlank = index;

  // Locate earliest blank task in queue to find where to start shifting tasks
  while (firstBlank < MAX_TASK_QUEUE_LENGTH) {
    if (_taskQueue[firstBlank] == BLANK) break;
    firstBlank++;
  }

  // Move all tasks down one
  for (byte j = firstBlank; j > index; j--) {
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
  for (byte i = 0; i < _taskQueueLength; i++) {
    if (_taskQueue[i] == task) return true;
  }

  return false;
}

// Take the first task from the queue, shift the remainders up, and execute
// the task.
void IO::_processQueuedTask() {
  Tasks taskToRun = _taskQueue[0];

  // Shift everything up after removing the prioritized task
  for (byte i = 0; i < _taskQueueLength - 1; i++) {
    _taskQueue[i] = _taskQueue[i + 1];
  }

  _taskQueueLength--;

  // Ensure vacated positions are filled with "blank" tasks
  for (byte i = _taskQueueLength; i < MAX_TASK_QUEUE_LENGTH; i++) {
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
    case WRITE_DAC: return _taskWriteDac();
    case READ_PORTEXP: return _taskReadPortExp();
    case PROCESS_PORTEXP_INTERRUPT: return _taskProcessPortExpInterrupt();
    case WRITE_DISPLAY: return _taskWriteDisplay();
    default: return;
  }
}

// Read from the ADC, and invoke the callback function with the value read
void IO::_taskReadAdc() {
  // Paranoid check
  if (_spiBusy) return;

  _spiBusy = true;
  unsigned int adcValue = _adc.analogRead(ADC_CV_CHANNEL);
  _spiBusy = false;

  _adcReadHandler(adcValue);

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
