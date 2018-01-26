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

#define MIN_SEQUENCE_MODE 0
#define MAX_SEQUENCE_MODE 8

// Gate modes
#define GATE_MODE_HALF_STEP 0
#define GATE_MODE_FULL_STEP 1
#define GATE_MODE_REPEAT_HALF 2
#define GATE_MODE_REPEAT_FULL 3
#define GATE_MODE_SILENT 4
#define MAX_GATE_MODE_VALUE 4

#define MIN_STEP_REPEAT 0
#define MAX_STEP_REPEAT 8

#define MAX_SEQUENCE_LENGTH 16
#define SEQUENCE_TERMINATOR 0xFF
#define SEQUENCE_RANDOM_STEP 0xEE

// Shorthand
#define SEQ_RS SEQUENCE_RANDOM_STEP

#include "settings.h"
#include <Arduino.h>
#include <stdint.h>

enum RunModes {
    // Not moving on on tick(), doesn't respond to chainTrigger()
    NOT_RUNNING, 
    // Moves on every tick(), switches to WAITING on sequence end if chained
    RUNNING,
    // Will switch to RUNNING on chainTrigger()
    WAITING
};

typedef void (*BoolChangedHandler)(bool);
typedef void (*ByteChangedHandler)(uint8_t);
typedef void (*EventHandler)(void);

// Sequence deals with everything to do with actual sequencing, that is,
// figuring out which step is active, which is next, managing the gate
class Sequence {
  public:
    // Initializes values
    static void init();

    // Called every pulse (as in PPQ), will advance sequence when needed
    static void tick();

    // Called on a positive flank of the chain input,
    // if sequencer is in waiting mode, this sets it to running
    static void chainTrigger();

    // Toggle running, stop if running, start if not running
    static void toggleRunMode();

    static bool getChained();
    static void setChained(bool);

    // Accessor to see whether the sequence is currently started
    static bool isRunning();

    // Set the sequence mode to one of the SEQUENCE_MODE_ constants
    static uint8_t setSequenceMode(uint8_t);

    // Manually selects the active step
    static void selectStep(uint8_t);

    // Retrieve the currently selected step
    static uint8_t getSelectedStep();

    // Manually set the gate signal. This will get overridden by what the
    // sequence dictates when it's running
    static void setGate(bool);

    // Sets the time divider, that is, the musical length of 1 step
    // For instance the value 8 would be 1 eight note
    static void setTimeDivider(uint8_t);

    static uint8_t cycleTimeDivider(bool);

    // Change gate mode per step, directly, or by cycling
    static uint8_t setGateModeForStep(uint8_t step, uint8_t gateMode);
    static uint8_t cycleGateModeForStep(uint8_t step);

    // Change step repetition count, directly, or by cycling
    static uint8_t setStepRepeatForStep(uint8_t step, uint8_t repetitions);
    static uint8_t cycleStepRepeatForStep(uint8_t step);

    // Writes Sequence's local settings to teh given settings struct
    static void collectSettings(Settings *settingsToSave);

    // Given settings struct, changes local settings to the provided ones
    static void loadFromSettings(Settings *settings);

    // Event handlers
    static void onRunningIndicatorChange(BoolChangedHandler);
    // Distinct from onRunningIndicatorChange
    static void onRunningChange(BoolChangedHandler);
    static void onGateChange(BoolChangedHandler);
    static void onTriggerChange(BoolChangedHandler);
    static void onSelectedStepChange(ByteChangedHandler);
    static void onSequenceEnd(EventHandler);

  private:
    static const uint8_t _sequences[][MAX_SEQUENCE_LENGTH + 1];
    static volatile uint8_t _selectedSequence[MAX_SEQUENCE_LENGTH + 1];
    static volatile uint8_t _indexInSequence;
    static volatile uint8_t _currentStep;
    static volatile uint8_t _currentStepRepetition;
    static volatile uint8_t _sequenceMode;
    static volatile uint8_t _gateMode[NUM_STEPS];
    static volatile uint8_t _stepRepeat[NUM_STEPS];
    static volatile uint8_t _timeDivider;
    static volatile uint32_t _pulsesPerSubstep;
    static volatile bool _firstHalfOfStep;
    static volatile uint32_t _internalTicks;
    static volatile RunModes _runMode;
    static volatile bool _running;
    static volatile bool _runningIndicator;
    static volatile bool _gate;
    static volatile bool _trigger;
    static volatile bool _chained;
    static BoolChangedHandler _onRunningIndicatorChangedHandler;
    static BoolChangedHandler _onRunningChangedHandler;
    static BoolChangedHandler _onGateChangedHandler;
    static BoolChangedHandler _onTriggerChangedHandler;
    static ByteChangedHandler _onSelectedStepChangedHandler;
    static EventHandler _onSequenceEndedHandler;
    static void _setRunningIndicator(bool);
    static void _setRunning(bool);
    static void _selectStep(uint8_t);
    static void _setTrigger(bool);
    static void _advanceSubStep();
    static void _advanceSequence();
    static bool _isOnLastStep();
    static void _initSequence(uint8_t);
    static uint8_t _generateRandomStep(uint8_t);
    static void _resetSequencePosition();
};

#endif // MODULE_SEQUENCE_H
