#ifndef MODULE_SEQUENCE_H
#define MODULE_SEQUENCE_H

#define PPQ 24 // Pulses per quarter note, like in MIDI
#define MIN_TIME_DIVIDER 1
#define MAX_TIME_DIVIDER 16
#define DEFAULT_TIME_DIVIDER 16

// Don't change this; rest of code is not set up for different value.
// It is only here to clarify the meaning of the number 8 in the code.
#ifndef NUM_STEPS
#define NUM_STEPS 8
#endif

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

#define MIN_STEP_REPEAT 0
#define MAX_STEP_REPEAT 8

#include "settings.h"
#include <Arduino.h>

typedef void (*BoolChangedHandler)(bool);
typedef void (*ByteChangedHandler)(byte);

// Sequence deals with everything to do with actual sequencing, that is,
// figuring out which step is active, which is next, managing the gate
class Sequence {


  public:
    // Initializes values
    static void init();

    // Called every pulse (as in PPQ), will advance sequence when needed
    static void tick();

    // Start running; will make sure `tick()` actually does stuff.
    static void start();

    // Toggle running, stop if running, start if not running
    static void toggleRunning();

    // Stop running; disables `tick()` from actually doing anything.
    static void stop();

    // Accessor to see whether the sequence is currently started
    static bool isRunning();

    // Set the sequence mode to one of the SEQUENCE_MODE_ constants
    static void setSequenceMode(byte);

    // Manually selects the active step
    static void selectStep(byte);

    // Retrieve the currently selected step
    static byte getSelectedStep();

    // Manually set the gate signal. This will get overridden by what the
    // sequence dictates when it's running
    static void setGate(bool);

    // Sets the time divider, that is, the musical length of 1 step
    // For instance the value 8 would be 1 eight note
    static void setTimeDivider(byte);

    static byte cycleTimeDivider(bool);

    // Change gate mode per step, directly, or by cycling
    static byte setGateModeForStep(byte step, byte gateMode);
    static byte cycleGateModeForStep(byte step);

    // Change step repetition count, directly, or by cycling
    static byte setStepRepeatForStep(byte step, byte repetitions);
    static byte cycleStepRepeatForStep(byte step);

    // Writes Sequence's local settings to teh given settings struct
    static void collectSettings(Settings *settingsToSave);

    // Given settings struct, changes local settings to the provided ones
    static void loadFromSettings(Settings *settings);

    // Event handlers
    static void onRunningChange(BoolChangedHandler);
    static void onGateChange(BoolChangedHandler);
    static void onTriggerChange(BoolChangedHandler);
    static void onSelectedStepChange(ByteChangedHandler);
    static void onSequenceModeChange(ByteChangedHandler);

  private:
    static volatile byte _previousStep;
    static volatile byte _currentStep;
    static volatile byte _currentStepRepetition;
    static volatile byte _sequenceMode;
    static volatile byte _gateMode[NUM_STEPS];
    static volatile byte _stepRepeat[NUM_STEPS];
    static volatile byte _timeDivider;
    static volatile unsigned long _pulsesPerSubstep;
    static volatile bool _firstHalfOfStep;
    static volatile unsigned long _internalTicks;
    static volatile bool _running;
    static volatile bool _gate;
    static volatile bool _trigger;
    static BoolChangedHandler _onRunningChangedHandler;
    static BoolChangedHandler _onGateChangedHandler;
    static BoolChangedHandler _onTriggerChangedHandler;
    static ByteChangedHandler _onSelectedStepChangedHandler;
    static ByteChangedHandler _onSequenceModeChangedHandler;
    static void _selectStep(byte);
    static byte _getNextStepIndexInSequence();
    static void _setTrigger(bool);
    static void _advanceSubStep();
    static void _advanceSequence();
};

#endif // MODULE_SEQUENCE_H
