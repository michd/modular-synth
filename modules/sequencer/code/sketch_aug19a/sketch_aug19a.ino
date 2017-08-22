#include <AH_MCP4921.h>

#define PIN_MODE_SELECT 16
#define PIN_RUNNING 14
#define PIN_RUNSTOP 7
#define PIN_SELECTED_STEP A0

#define PIN_ADC_CLK 8
#define PIN_ADC_DATA_OUT 9
#define PIN_ADC_DATA_IN 15
#define PIN_ADC_SHUTDOWN 6

#define PIN_DAC_CLK 8 // Re-using
#define PIN_DAC_DATA_IN 15 // Re-using
#define PIN_DAC_CHIP_SELECT A2

#define PIN_STEP_ADDR_A 2
#define PIN_STEP_ADDR_B 3
#define PIN_STEP_ADDR_C 4
#define PIN_STEP_ENABLE 5

#define PIN_RANDOM_INPUT A1

#define ADC_CV_CHANNEL 0

#define ADC_RESOLUTION_BIT 12
#define DAC_RESOLUTION_BIT 12

#define BUILTIN_ADC_MAX 1024

// Don't change this; rest of code is not set up for different value.
// It is only here to clarify the meaning of the number 8 in the code.
#define NUM_STEPS 8

#define STEP_TIME_MS 2000
#define STEP_SUB_DIVISIONS 8

// Sequence modes
#define SEQUENCE_MODE_FORWARD              0
#define SEQUENCE_MODE_REVERSE              1
#define SEQUENCE_MODE_BACK_AND_FORTH       2
#define SEQUENCE_MODE_ALT_FORWARD          3
#define SEQUENCE_MODE_ALT_REVERSE          4
#define SEQUENCE_MODE_ALT_BACK_AND_FORTH_A 5
#define SEQUENCE_MODE_ALT_BACK_AND_FORTH_B 6
#define SEQUENCE_MODE_RANDOM               7

AH_MCP4921 pitchCvDac(PIN_DAC_DATA_IN, PIN_DAC_CLK, PIN_DAC_CHIP_SELECT);

volatile bool running;
volatile byte previousStep; // Used for certain sequence modes
volatile byte currentStep;
volatile byte currentSubStep;
volatile byte sequenceMode;

byte StepSelectButtonDivider;
unsigned long int SubStepDelayUs;

void setup() {
  StepSelectButtonDivider = (byte)((float)BUILTIN_ADC_MAX / (float)(NUM_STEPS + 1));
  SubStepDelayUs = (STEP_TIME_MS * 1000) / STEP_SUB_DIVISIONS;
  
  pinMode(PIN_MODE_SELECT, INPUT);
  pinMode(PIN_RUNNING, OUTPUT);
  pinMode(PIN_RUNSTOP, INPUT);

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
  pinMode(PIN_RANDOM_INPUT, INPUT);
  
  digitalWrite(PIN_RUNNING, 0);

  // ADC init
  digitalWrite(PIN_ADC_SHUTDOWN, 1);
  digitalWrite(PIN_ADC_CLK, 0);
  digitalWrite(PIN_ADC_DATA_IN, 0);

  // DAC init
  digitalWrite(PIN_DAC_CHIP_SELECT, 1);
  digitalWrite(PIN_DAC_CLK, 0);
  digitalWrite(PIN_DAC_DATA_IN, 0);

  currentSubStep = 0;
  selectStep(0);
  sequenceMode = SEQUENCE_MODE_FORWARD;

  // Listen for button press on run/stop button
  attachInterrupt(digitalPinToInterrupt(PIN_RUNSTOP), runStopOnPressed, RISING);

  // Start not running
  running = false;  

  Serial.begin(9600);
}

void loop() {
  // TODO: use a timer instead of delay to run this
  // TODO: stabilise the ADC input from the sequence buttons
  // TODO: debounce buttons

  // Mode select button held down
  if (digitalRead(PIN_MODE_SELECT)) {
    setSequenceMode(readSelectedStep());
  }
  
  if (running) {    
    currentSubStep++;
    
    if (currentSubStep == STEP_SUB_DIVISIONS) {
      currentSubStep = 0;
      advanceSequence();
      unsigned short int reading = readAdc(ADC_CV_CHANNEL);
      Serial.println(reading, DEC);  
      pitchCvDac.setValue(reading);
    }    
     
    delayMicroseconds(SubStepDelayUs);
    
    //Serial.println(readSelectedStep(), DEC);
  }
}

// This is 1-indexed, 0 = no step pressed
byte readSelectedStep() {
  return (analogRead(PIN_SELECTED_STEP) + (StepSelectButtonDivider / 2) ) / StepSelectButtonDivider;
}

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

void sendPulse(int pin) {
  digitalWrite(pin, 1);
  delayMicroseconds(10);
  digitalWrite(pin, 0);
  delayMicroseconds(10);
}

void advanceSequence() {
  byte oldStep = currentStep;
  byte nextStep = getNextStepIndexInSequence();
  
  previousStep = oldStep;
  selectStep(nextStep);
}

void setSequenceMode(byte buttonIndex) {
  if (buttonIndex == 0) {
    return;
  }

  sequenceMode = buttonIndex - 1;
}

// Based on mode and current step, figures out the next step
byte getNextStepIndexInSequence() {
  byte oldStep = currentStep;
  byte nextStep = 0;

  switch (sequenceMode) {
    case SEQUENCE_MODE_FORWARD:
      nextStep = oldStep + 1;
      if (nextStep >= NUM_STEPS) {
        nextStep = 0;
      }
      break;
      
    case SEQUENCE_MODE_REVERSE:
      if (oldStep > 0) {
        nextStep = oldStep - 1;
      } else {
        nextStep = NUM_STEPS - 1;
      }
      break;
      
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

    case SEQUENCE_MODE_ALT_FORWARD:
      nextStep = oldStep + 2;
      if (nextStep == NUM_STEPS) {
        nextStep = 1;
      } else if (nextStep > NUM_STEPS) {
        nextStep = 0;
      }
      break;

    case SEQUENCE_MODE_ALT_REVERSE:
      if (oldStep > 1) {
        nextStep = oldStep - 2;
      } else if (oldStep == 1) {
        nextStep = NUM_STEPS - 2;
      } else if (oldStep == 0) {
        nextStep = NUM_STEPS - 1; 
      }
      break;

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

    case SEQUENCE_MODE_RANDOM:
      randomSeed(analogRead(PIN_RANDOM_INPUT));
      do {
        nextStep = random(NUM_STEPS);
      } while (nextStep == oldStep);
      break;
  }

  return nextStep;
}

void selectStep(byte step) {
  currentStep = step;
  byte a = currentStep & B001;
  byte b = currentStep & B010;
  byte c = currentStep & B100;
  
  digitalWrite(PIN_STEP_ENABLE, 0);
  digitalWrite(PIN_STEP_ADDR_A, a);
  digitalWrite(PIN_STEP_ADDR_B, b);
  digitalWrite(PIN_STEP_ADDR_C, c);
  digitalWrite(PIN_STEP_ENABLE, 1);
}

void runStopOnPressed() {
  running = !running;
  currentSubStep = 0;
  digitalWrite(PIN_RUNNING, running);
}
