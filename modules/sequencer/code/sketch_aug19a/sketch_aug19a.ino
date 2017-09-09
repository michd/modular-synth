#include <TimerOne.h>
#include <SPI.h>
#include "libs/MCP492X.h"
#include "libs/MCP23S17.h"
#include "libs/MCP3202.h"

#define MIDI_PPQ 24 // 24 pulses per quarter note
#define DEFAULT_TEMPO 130

#define MAX_TIME_DIVIDER 32
#define DEFAULT_TIME_DIVIDER 8
#define MIN_TIME_DIVIDER 1

// Chip select pins for the various SPI devices being used
#define PIN_SPI_CS_ADC 6
#define PIN_SPI_CS_DAC A2
#define PIN_SPI_CS_PORTEXP A0

#define PIN_MODE_SELECT_BUTTON 9
#define PIN_GATE_MODE_SELECT_BUTTON 3
#define PIN_REPEAT_SELECT_BUTTON 2
#define PIN_RUNSTOP_BUTTON 7

#define PIN_RUNNING 8
#define PIN_GATE_OUT 10

#define PIN_STEP_ADDR_A A1
#define PIN_STEP_ADDR_B A3
#define PIN_STEP_ADDR_C 4
#define PIN_STEP_ENABLE 5

#define BUTTON_DEBOUNCE_TICKS 10

#define ADC_CV_CHANNEL 0

#define ADC_RESOLUTION_BIT 12
#define DAC_RESOLUTION_BIT 12

#define ADC_MAX 4096

#define PORT_EXPANDER_CHANNEL 0

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

#define MAX_STEP_REPEAT 8

/*
 * MCP23S17 pin number mapping:
 * A0 -  1 | B0 -  9
 * A1 -  2 | B1 - 10
 * A2 -  3 | B2 - 11
 * A3 -  4 | B3 - 12
 * A4 -  5 | B4 - 13
 * A5 -  6 | B5 - 14
 * A6 -  7 | B6 - 15
 * A7 -  8 | B7 - 16
 * 
 * When writing a word: order is MSB B7-B0 A7-A0 LSB
 */

/* SPI pins on pro micro for reference:
 * MOSI: 16
 * MISO: 14
 * SCK:  15
 */

// Array of DAC output values mapped to note index
short int NoteOutputValues[NOTE_RANGE + 1];

// Set up MCP23S17 library for port expander
MCP23S17 PortExp(PORT_EXPANDER_CHANNEL, PIN_SPI_CS_PORTEXP);

// ADC library for reading pitch input voltages
MCP3202 PitchAdc(PIN_SPI_CS_ADC);

// Set up DAC library for outputting Pitch control voltage
MCP492X PitchCvDac(PIN_SPI_CS_DAC);

// Wether we're currently advancing steps automatically
volatile bool isRunning;

// Used for certain sequence modes, the previous value of currentStep
volatile byte previousStep;

// Currently output step index
volatile byte currentStep;

volatile byte currentStepRepetition = 0;

// Determines the order we traverse steps in,
// one of the SEQUENCE_MODE_x constants
volatile byte sequenceMode;

volatile byte gateMode[NUM_STEPS];
volatile byte stepRepeat[NUM_STEPS];

// Ranging of output notes
// TODO write more helpful commentary here
volatile byte rangeMinNote = 12;
volatile byte rangeMaxNote = 36;

// Input stabilization
// For debouncing buttons that trigger interrupts
volatile unsigned long debounceTimeRecorded = 0;
volatile unsigned long debounceTicksRecorded = 0;




// Timing
// BPM
// TODO: external tempo source
// volatile unsigned short tempo = DEFAULT_TEMPO;

volatile short int timeDivider = DEFAULT_TIME_DIVIDER; // eight notes

// Number of midi pulses per sub step, knowing that each step is dvided in 2
volatile unsigned long pulsesPerSubstep = (MIDI_PPQ / 2) / (DEFAULT_TIME_DIVIDER / 4);

// Wether the current "substep" is the first half of said step
// Important for the gate when we want to have gate for half a step
volatile bool firstHalfOfStep = true;

// Ticks lapsed in this substep; once this reaches ticksPerSubstep it's reset to 0,
// and we advance the step
volatile unsigned long internalTicks = 0;

// Alternative to micros() when running,
// as we do too much work for micros to work reliably when running
volatile unsigned long debounceTicks = 0;

void setup() {
  // Tempo is in beats per minute
  // MIDI PPQ is pulses per beat
  // And timer period is in microseconds, of which there are 1000000 per second
  // 60 being seconds per minute
  // Summarized, the period of the timer is calculated to generate MIDI_PPQ pulses per beat for the given bpm  
  Timer1.initialize(round((double)1000000 / (double)((DEFAULT_TEMPO * MIDI_PPQ) / (double)60)));
  Timer1.attachInterrupt(internalTimerTick);
  
  pinMode(PIN_MODE_SELECT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_GATE_MODE_SELECT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_REPEAT_SELECT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RUNNING, OUTPUT);
  pinMode(PIN_RUNSTOP_BUTTON, INPUT_PULLUP);
  pinMode(PIN_GATE_OUT, OUTPUT);

  // Step address pins
  pinMode(PIN_STEP_ADDR_A, OUTPUT);
  pinMode(PIN_STEP_ADDR_B, OUTPUT);
  pinMode(PIN_STEP_ADDR_C, OUTPUT);
  pinMode(PIN_STEP_ENABLE, OUTPUT);

  digitalWrite(PIN_RUNNING, 0);
  digitalWrite(PIN_GATE_OUT, 0);

  PortExp.begin();
  // Configure pins on port expander
  // Port A = buttons per step
  uint8_t portAmodes = 0xFF; // All pins on port A are input
  // Port B: to be assigned to stuff, nothing atm - TODO
  uint8_t portBmodes = 0b00000000; // Change as needed
  unsigned int combinedModes = (portBmodes << 8) | portAmodes;
  
  PortExp.pinMode(combinedModes);
  
  // Set all inputs to pull-up (buttons normal-open to gnd)
  // This means we can re-use the portmodes word
  PortExp.pullupMode(combinedModes);

  // Inverts all inputs so we don't have to do conversion (mental overhead)
  // Can re-use portmodes again. A 1 value will mean "button pressed".
  PortExp.inputInvert(0);

  // Init pitch adc lib
  PitchAdc.begin();

  // Init Pitch DAC lib
  PitchCvDac.begin();

  selectStep(0);
  sequenceMode = SEQUENCE_MODE_FORWARD;

  // Listen for button press on run/stop button
  attachInterrupt(digitalPinToInterrupt(PIN_RUNSTOP_BUTTON), runStopOnPressed, FALLING);

  // Listen for button press on gate mode button
  attachInterrupt(digitalPinToInterrupt(PIN_GATE_MODE_SELECT_BUTTON), gateModeOnPressed, FALLING);

  // Listen for button press on repeat count button
  attachInterrupt(digitalPinToInterrupt(PIN_REPEAT_SELECT_BUTTON), repeatOnPressed, FALLING);

  // Start not running
  isRunning = false;

  // Initialize gate mode and step repeat for each step
  for (byte i = 0; i < NUM_STEPS; i++) {
    gateMode[i] = GATE_MODE_HALF_STEP;
    stepRepeat[i] = 1;    
  }

  // Precalculate all the note values so we don't have to
  // during normal runtime.
  float noteStep = (float)4096 / (float)(NOTE_RANGE - 1);

  for (byte i = 0; i <= NOTE_RANGE; i++) {
    NoteOutputValues[i] = (short int)round(noteStep * (float)i);
  }

  PitchCvDac.analogWrite(2048); // 0V
  Serial.begin(250000);
}

void loop() {
  byte oneIndexedStep = getSelectedStep();
  
  if (digitalRead(PIN_MODE_SELECT_BUTTON) == 0) {
    // Mode select button held down
    if (oneIndexedStep != 0) {
      Serial.print("Setting sequence mode to ");
      Serial.print(oneIndexedStep - 1, DEC);
      Serial.print("\n");
      setSequenceMode(oneIndexedStep - 1);
    }
  } else if (!isRunning) {
    // Not running, and no other buttons pressed,
    // make this step active
    if (oneIndexedStep != 0) {
      selectStep(oneIndexedStep - 1);
      toggleGate(true);
      unsigned short int reading = PitchAdc.analogRead(ADC_CV_CHANNEL);
      unsigned short int note = mapToNote(reading);
      PitchCvDac.analogWrite(NoteOutputValues[mapToNote(reading)]);
    } else {
      toggleGate(false);
    }
  }
  
  delay(1);
}

// newDivider should only ever be a power of 2
// It's not enforced here for efficience, but if it isn't, it'll likely
// mess stuff up somewhere.
void setTimeDivider(short int newDivider) {
  pulsesPerSubstep = (short int)((double)(MIDI_PPQ / 2) / ((double)newDivider / (double)4));
}

void changeTimeDivider(bool higher) {
  short int newTimeDivider;
  
  // TODO: once/if there are up/down buttons, don't cycle but limit to max/min
  if (higher) {
    newTimeDivider = timeDivider << 1;

    if (newTimeDivider > MAX_TIME_DIVIDER) {
      newTimeDivider = 1;
    }
  } else {
    newTimeDivider = timeDivider >> 1;

    if (newTimeDivider < MIN_TIME_DIVIDER) {
      newTimeDivider = MAX_TIME_DIVIDER;
    }
  }

  setTimeDivider(newTimeDivider);
}

// [Begin Timing region]
void internalTimerTick() {
  debounceTicks++;

  PortExp.digitalRead();

  if (!isRunning) {
    return;    
  }

  internalTicks++;
  if (internalTicks >= pulsesPerSubstep) {
    internalTicks = 0;
    advanceSubStep(); 
  }
}

void advanceSubStep() {
  firstHalfOfStep = !firstHalfOfStep;
  byte repeatsForThisStep = stepRepeat[currentStep];

  if (firstHalfOfStep) {
    if (currentStepRepetition >= repeatsForThisStep) {
      advanceSequence();
      currentStepRepetition = 0;
    }

    // Skip steps with repeats set to 0
    while (stepRepeat[currentStep] == 0) {
      advanceSequence();
      currentStepRepetition = 0;
    }

    currentStepRepetition++;
    
    unsigned short int reading = PitchAdc.analogRead(ADC_CV_CHANNEL);
    unsigned short int note = mapToNote(reading);
    //Serial.println(note, DEC);
    PitchCvDac.analogWrite(NoteOutputValues[mapToNote(reading)]);
  }
  
  switch (gateMode[currentStep]) {
    case GATE_MODE_HALF_STEP:
      toggleGate(firstHalfOfStep && currentStepRepetition == 1);
      break;
      
    case GATE_MODE_FULL_STEP:
      toggleGate(currentStepRepetition == 1);
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

// [End Timing region]

// Gets the currently pressed button index, corresponding to the step it's under.
// The return value is 1-indexed, as the 0 value is reserved for when no button is pressed.
byte getSelectedStep() {
  // The pins are pull-up, so 1 = not pressed.
  // Therefore, we're bitwise inverting first.
  uint8_t wordA = ~(PortExp.digitalReadCache()) & 0xFF;
  //byte wordA = 0;
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

void repeatOnPressed() {
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

  if (stepRepeat[stepToAlter] == MAX_STEP_REPEAT) {
    stepRepeat[stepToAlter] = 0;
  } else {
    stepRepeat[stepToAlter]++;
  }

  //Serial.print("Repeat count for step ");
  //Serial.print(stepToAlter, DEC);
  //Serial.print(": ");
  //Serial.print(stepRepeat[stepToAlter], DEC);
  //Serial.print("\n");
}

