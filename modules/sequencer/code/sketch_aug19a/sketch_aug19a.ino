#include <TimerOne.h>
#include <AH_MCP4921.h>

#define PIN_MODE_SELECT_BUTTON 16
#define PIN_GATE_MODE_SELECT_BUTTON 3
#define PIN_RUNNING 14
#define PIN_RUNSTOP_BUTTON 7

#define PIN_SELECTED_STEP A0
#define PIN_GATE_OUT 10

#define PIN_ADC_CLK 8
#define PIN_ADC_DATA_OUT 9
#define PIN_ADC_DATA_IN 15
#define PIN_ADC_SHUTDOWN 6

#define PIN_DAC_CLK 8 // Re-using
#define PIN_DAC_DATA_IN 15 // Re-using
#define PIN_DAC_CHIP_SELECT A2

#define PIN_STEP_ADDR_A A1
#define PIN_STEP_ADDR_B A3
#define PIN_STEP_ADDR_C 4
#define PIN_STEP_ENABLE 5

#define BUTTON_DEBOUNCE_TICKS 5000

#define ADC_CV_CHANNEL 0

#define ADC_RESOLUTION_BIT 12
#define DAC_RESOLUTION_BIT 12

#define BUILTIN_ADC_MAX 1024
#define ADC_MAX 4096

// Don't change this; rest of code is not set up for different value.
// It is only here to clarify the meaning of the number 8 in the code.
#define NUM_STEPS 8

#define DAC_CENTER_VALUE 2048

// Borrowing from Midi in code, as we're dealing with the same range
#define MIN_NOTE_MIDI 24 // C2
#define MAX_NOTE_MIDI 108 // C9
#define NOTE_RANGE 84

// Sequence modes
#define SEQUENCE_MODE_FORWARD              0
#define SEQUENCE_MODE_REVERSE              1
#define SEQUENCE_MODE_BACK_AND_FORTH       2
#define SEQUENCE_MODE_ALT_FORWARD          3
#define SEQUENCE_MODE_ALT_REVERSE          4
#define SEQUENCE_MODE_ALT_BACK_AND_FORTH_A 5
#define SEQUENCE_MODE_ALT_BACK_AND_FORTH_B 6
#define SEQUENCE_MODE_RANDOM               7

// Gate modes
#define GATE_MODE_HALF_STEP 0
#define GATE_MODE_FULL_STEP 1
#define GATE_MODE_REPEAT_HALF 2
#define GATE_MODE_REPEAT_FULL 3
#define GATE_MODE_SILENT 4
#define MAX_GATE_MODE_VALUE 4

// Only set during setup(), not altered after
// Divider constant used to map the ADC reading to 1 of 8 steps
byte StepSelectButtonDivider;

// Delay between sub steps in microseconds
// Possibly obsolete soon?
unsigned long int SubStepDelayUs;

// Array of DAC output values mapped to note index
short int NoteOutputValues[NOTE_RANGE + 1];

// Set up DAC library for outputting Pitch control voltage
AH_MCP4921 PitchCvDac(PIN_DAC_DATA_IN, PIN_DAC_CLK, PIN_DAC_CHIP_SELECT);

// Wether we're currently advancing steps automatically
volatile bool isRunning;

// Used for certain sequence modes, the previous value of currentStep
volatile byte previousStep;

// Currently output step index
volatile byte currentStep;

// Determines the order we traverse steps in,
// one of the SEQUENCE_MODE_x constants
volatile byte sequenceMode;

volatile byte gateMode[NUM_STEPS];

// Ranging of output notes
// TODO write more helpful commentary here
volatile byte rangeMinNote = 12;
volatile byte rangeMaxNote = 36;

// Input stabilization
// For debouncing buttons that trigger interrupts
volatile unsigned long debounceTimeRecorded = 0;
volatile unsigned long debounceTicksRecorded = 0;

// Used for stabilizing the reading from selected step.
// See getSelectedStep for more info
byte lastSelectedStepReadings[] = { 0, 0, 0, 0 };


// Timing
// BPM
volatile unsigned short tempo = 120;
// If true, the 8 steps consitute 2 beats rather than 4,
// This affects the speed, given the bpm
volatile bool stepIs16th = false;
// Number of internal ticks needed to get through half a step,
// calculated based on tempo and stepIs16th
volatile unsigned long ticksPerSubstep = 0;

// Wether the current "substep" is the first half of said step
// This will become important once we add the gate and want to have gate for half a step
volatile bool firstHalfOfStep = true;

// Ticks lapsed in this substep; once this reaches ticksPerSubstep it's reset to 0,
// and we advance the step
volatile unsigned long internalTicks = 0;

// Alternative to micros() when running,
// as we do too much work for micros to work reliably when running
volatile unsigned long debounceTicks = 0;

void setup() {
  StepSelectButtonDivider = (byte)((float)BUILTIN_ADC_MAX / (float)(NUM_STEPS));

  calculateTicksPerSubstep();
  
  Timer1.initialize(10);
  Timer1.attachInterrupt(internalTimerTick, 10);
  
  pinMode(PIN_MODE_SELECT_BUTTON, INPUT);
  pinMode(PIN_GATE_MODE_SELECT_BUTTON, INPUT);
  pinMode(PIN_RUNNING, OUTPUT);
  pinMode(PIN_RUNSTOP_BUTTON, INPUT);
  pinMode(PIN_GATE_OUT, OUTPUT);

  // ADC pins
  pinMode(PIN_ADC_CLK, OUTPUT);
  pinMode(PIN_ADC_DATA_OUT, INPUT);
  pinMode(PIN_ADC_DATA_IN, OUTPUT);
  pinMode(PIN_ADC_SHUTDOWN, OUTPUT);

  // DAC pins
  // First two are technically duplicated from ADC
  pinMode(PIN_DAC_CLK, OUTPUT);
  pinMode(PIN_DAC_DATA_IN, OUTPUT);
  pinMode(PIN_DAC_CHIP_SELECT, OUTPUT);

  // Step address pins
  pinMode(PIN_STEP_ADDR_A, OUTPUT);
  pinMode(PIN_STEP_ADDR_B, OUTPUT);
  pinMode(PIN_STEP_ADDR_C, OUTPUT);
  pinMode(PIN_STEP_ENABLE, OUTPUT);

  // Other pins
  pinMode(PIN_SELECTED_STEP, INPUT);
  
  digitalWrite(PIN_RUNNING, 0);
  digitalWrite(PIN_GATE_OUT, 0);

  // ADC init
  digitalWrite(PIN_ADC_SHUTDOWN, 1);
  digitalWrite(PIN_ADC_CLK, 0);
  digitalWrite(PIN_ADC_DATA_IN, 0);

  // DAC init
  digitalWrite(PIN_DAC_CHIP_SELECT, 1);
  digitalWrite(PIN_DAC_CLK, 0);
  digitalWrite(PIN_DAC_DATA_IN, 0);

  selectStep(0);
  sequenceMode = SEQUENCE_MODE_FORWARD;

  // Listen for button press on run/stop button
  attachInterrupt(digitalPinToInterrupt(PIN_RUNSTOP_BUTTON), runStopOnPressed, RISING);

  // Listen for button press on gate mode button
  attachInterrupt(digitalPinToInterrupt(PIN_GATE_MODE_SELECT_BUTTON), gateModeOnPressed, RISING);

  // Start not running
  isRunning = false;

  // Initialize gate mode for each step
  for (byte i = 0; i < NUM_STEPS; i++) {
    gateMode[i] = GATE_MODE_HALF_STEP;
  }

  // TODO: Step repetition init

  // Precalculate all the note values so we don't have to
  // during normal runtime.
  float noteStep = (float)4096 / (float)(NOTE_RANGE - 1);

  for (byte i = 0; i <= NOTE_RANGE; i++) {
    NoteOutputValues[i] = (short int)round(noteStep * (float)i);
  }

  PitchCvDac.setValue(2048); // 0V
  Serial.begin(250000);
}

void loop() {
  processStepButtonReading();

  byte oneIndexedStep = getSelectedStep();
  
  if (digitalRead(PIN_MODE_SELECT_BUTTON)) {
    // Mode select button held down
    if (oneIndexedStep != 0) {
      setSequenceMode(oneIndexedStep - 1);
    }
  } else if (!isRunning) {
    // Not running, and no other buttons pressed,
    // make this step active
    if (oneIndexedStep != 0) {
      selectStep(oneIndexedStep - 1);
    }
  }

  // TODO: if not running, continually update DAC

  delayMicroseconds(100);
}

// [Begin Timing region]
void internalTimerTick() {
  debounceTicks++;
  
  if (!isRunning) {
    return;    
  }

  internalTicks++;
  if (internalTicks > ticksPerSubstep) {
    internalTicks = 0;
    advanceSubStep(); 
  }
}

void advanceSubStep() {
  firstHalfOfStep = !firstHalfOfStep;

  if (firstHalfOfStep) {
    advanceSequence();
    unsigned short int reading = readAdc(ADC_CV_CHANNEL);
    unsigned short int note = mapToNote(reading);
    //Serial.println(note, DEC);
    PitchCvDac.setValue(NoteOutputValues[mapToNote(reading)]);
  }
  
  switch (gateMode[currentStep]) {
    case GATE_MODE_HALF_STEP:
      toggleGate(firstHalfOfStep);
      break;
      
    case GATE_MODE_FULL_STEP:
      toggleGate(true);
      break;

    case GATE_MODE_REPEAT_HALF:
      toggleGate(firstHalfOfStep);
      break;

    case GATE_MODE_REPEAT_FULL:
      toggleGate(true);
      break;

    case GATE_MODE_SILENT:
      toggleGate(false);
      break;
  }
}

void calculateTicksPerSubstep() {
  byte multiplier = stepIs16th ? 1 : 2;
  ticksPerSubstep = round(((double)1000 / (((double)tempo * (double)8) / (double)60)) * (double)(100 * multiplier));
}
// [End Timing region]

// Processes the period ADC reading for which step button is pressed.
// This is used for "debouncing" false non-0 readings when a button
// has just been released.
// Works by caching the last 4 readings.
void processStepButtonReading() {
  // This is 1-indexed, 0 = no step pressed
  byte reading = (analogRead(PIN_SELECTED_STEP) + (StepSelectButtonDivider / 2) ) / StepSelectButtonDivider;
  lastSelectedStepReadings[3] = lastSelectedStepReadings[2];
  lastSelectedStepReadings[2] = lastSelectedStepReadings[1];
  lastSelectedStepReadings[1] = lastSelectedStepReadings[0];
  lastSelectedStepReadings[0] = reading;    
}

// Gets the currently pressed button index, corresponding to the step it's under.
// The return value is 1-indexed, as the 0 value is reserved for when no button is pressed.
byte getSelectedStep() {
  byte *arr = lastSelectedStepReadings;

  // The issue with this DAC input is only when letting go of the button,
  // the read value will briefly be > 0; it doesn't happen the other way around
  // therefore if the most recent value is the highest of the recent readings,
  // we can safely determine that this is the button actually pressed.
  // For values lower we want to let it stabilize for a few readings instead.
  if (arr[0] >= arr[1] && arr[0] >= arr[2] && arr[0] >= arr[3]) {
    return arr[0];
  }

  return 0;
}

// Read a value from the external ADC, given one of the two channels
// Implements a bit of SPI manually
short unsigned int readAdc(byte channel) {
  short int adcValue = 0;
  // Leftmost bit = start bit
  // Then, use single ended mode
  // Then, select channel 0
  // Then, instruct to send the least significant bit first
  byte commandBits = B1100;

  if (channel != 0) {
    commandBits |= B0010;
  }  

  // Turn ADC on
  digitalWrite(PIN_ADC_SHUTDOWN, 0);

  // Send configuration
  for (byte i = 3; i > 0; i--) {
    // Grab the current bit, most significant first
    // If the value is > 0, it will write a 1, so doesn't matter if it's > 1
    digitalWrite(PIN_ADC_DATA_IN, commandBits & (1 << i));

    sendPulse(PIN_ADC_CLK);
  }

  // Pass the null bit between sending config and receiving data
  sendPulse(PIN_ADC_CLK);

  // Read from ADC
  for (short int i = ADC_RESOLUTION_BIT; i >= 0; i--) {
    adcValue += digitalRead(PIN_ADC_DATA_OUT) << i;
    sendPulse(PIN_ADC_CLK);    
  }

  // Turn ADC off
  digitalWrite(PIN_ADC_SHUTDOWN, 1);

  return adcValue;
}

// Sends a short "clock" pulse on a given pin.
// Takes 20 us to complete.
void sendPulse(int pin) {
  digitalWrite(pin, 1);
  delayMicroseconds(10);
  digitalWrite(pin, 0);
  delayMicroseconds(10);
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

// Advances to the next step, using getNextStepIndexInSequece to determine what that is.
// Stores the old step in previousStep.
void advanceSequence() {
  byte oldStep = currentStep;
  byte nextStep = getNextStepIndexInSequence();
  
  previousStep = oldStep;
  selectStep(nextStep);
}

void setSequenceMode(byte newSequenceMode) {
  sequenceMode = newSequenceMode;

  if (sequenceMode == SEQUENCE_MODE_RANDOM) {
    randomSeed(millis());
  }
}

// Based on mode and current step, figures out the next step index
byte getNextStepIndexInSequence() {
  byte oldStep = currentStep;
  byte nextStep = 0;

  switch (sequenceMode) {
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

void toggleGate(bool on) {
  digitalWrite(PIN_GATE_OUT, on);
}

// Activates the new step by index
// Writes the index via 3 bit number,
// hooked up to 3-to-8 decoder for LEDs, as well as multiplexer(s)
void selectStep(byte step) {
  currentStep = step;
  byte a = currentStep & B001;
  byte b = currentStep & B010;
  byte c = currentStep & B100;

  // Disable 3-to-8 decoder while writing to all lines
  // Prevents flickering of different values before the full number is written
  digitalWrite(PIN_STEP_ENABLE, 0);
  
  digitalWrite(PIN_STEP_ADDR_A, a);
  digitalWrite(PIN_STEP_ADDR_B, b);
  digitalWrite(PIN_STEP_ADDR_C, c);

  // Re-enable decoder
  digitalWrite(PIN_STEP_ENABLE, 1);
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
  if (debounceButton()) {
    return;
  }
 
  isRunning = !isRunning;
  digitalWrite(PIN_RUNNING, isRunning);

  if (!isRunning) {
    toggleGate(false);
  }
}

void gateModeOnPressed() {
  if (debounceButton()) {
    return;
  }

  byte stepToAlter;
  if (!isRunning) {
    stepToAlter = currentStep;
  } else {
    byte stepToAlterOneIndexed = getSelectedStep();

    // If none selected there's nothing we can do with that button press,
    // abort.
    if (stepToAlterOneIndexed == 0) {
      return;
    }

    stepToAlter = stepToAlterOneIndexed -1;
  }

  if (gateMode[stepToAlter] == MAX_GATE_MODE_VALUE) {
    gateMode[stepToAlter] = 0;
  } else {
    gateMode[stepToAlter]++;
  }

  Serial.print("Gate mode for step ");
  Serial.print(stepToAlter, DEC);
  Serial.print(": ");
  Serial.print(gateMode[stepToAlter], DEC);
  Serial.print("\n");
}

