#ifndef MODULE_IO_H
#define MODULE_IO_H

// Chip select pins for the various SPI devices being used
#define PIN_SPI_CS_ADC 6
#define PIN_SPI_CS_DAC A2
#define PIN_SPI_CS_PORTEXP A0
#define PIN_SPI_CS_DISP 9

#define PIN_EXTERNAL_CLOCK 7

#define PIN_RUNNING 8
#define PIN_GATE_OUT 10
#define PIN_TRIGGER_OUT 2

#define PIN_STEP_ADDR_A A1
#define PIN_STEP_ADDR_B A3
#define PIN_STEP_ADDR_C 4
#define PIN_STEP_ENABLE 5

#define PIN_PORTEXP_INTERRUPT 3

#define PORTEXP_PIN_GATE_MODE_SELECT_BUTTON 9
#define PORTEXP_PIN_REPEAT_SELECT_BUTTON 10
#define PORTEXP_PIN_SEQUENCE_MODE_SELECT_BUTTON 11
#define PORTEXP_PIN_RUN_STOP_BUTTON 12
#define PORTEXP_PIN_SCALE_BUTTON 13
#define PORTEXP_PIN_UP_ARROW 1
#define PORTEXP_PIN_DOWN_ARROW 2
#define PORTEXP_PIN_PARAM_SELECT_A 3
#define PORTEXP_PIN_PARAM_SELECT_B 4

#define PORT_EXPANDER_CHANNEL 0

#define ADC_CV_CHANNEL 0
#define ADC_STEP_CHANNEL 1

#define ADC_RESOLUTION_BIT 12
#define DAC_RESOLUTION_BIT 12

#define DAC_CENTER_VALUE 2048
#define ADC_MAX 4096
#define DAC_MAX 4096

#define NUM_STEPS 8

// There are only 6 possible tasks, and none may be duplicated in the queue
#define MAX_TASK_QUEUE_LENGTH 6
#define DISPLAY_STRING_LENGTH 5 // 1 of the null terminator

#include <Arduino.h>

#include "MAX72S19.h" // Display driver
#include "MCP23S17.h" // Port expander
#include "MCP3202.h"  // ADC
#include "MCP492X.h"  // DAC

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

enum Tasks {
    BLANK,
    READ_ADC,
    READ_SELECTED_STEP,
    WRITE_DAC,
    READ_PORTEXP,
    PROCESS_PORTEXP_INTERRUPT,
    WRITE_DISPLAY
  };

typedef void (*AdcReadHandler)(unsigned int);
typedef void (*ButtonPressedHandler)(void);
typedef void (*ArrowButtonPressedHandler)(bool);
typedef void (*TickHandler)(void);

class IO {
  public:
    static void init();

    static void setGate(bool);

    static void setTrigger(bool);

    static void setRunning(bool);

    // Drive multiplexers and decoder for selecting the current step
    static void setStep(byte);

    // write to DAC (SPI)
    static void setPitch(unsigned int);

    // Read from the ADC, processing the result with a handler function
    static void readAdc(AdcReadHandler);

    // Routinely read port expander values
    static void readPortExp();

    // Read the cached data from port expander
    static word readPortExpCache();

    static bool getPortExpPin(byte);

    // Read and derive the pressed step from the ADC
    // The step buttons are set up as a little keyboard, resistors between them
    // We read an ADC value to figure out which one's pressed, if any
    static void readSelectedStep();

    static byte getSelectedStep();

    // Note: very limiting but should cover needs pretty well for now
    // Write 4 characters to the display
    static void writeDisplay(char*);

    // Assign button press handlers
    static void onSequenceModeButtonPressed(ButtonPressedHandler);
    static void onGateButtonPressed(ButtonPressedHandler);
    static void onRepeatButtonPressed(ButtonPressedHandler);
    static void onRunStopButtonPressed(ButtonPressedHandler);
    static void onScaleButtonPressed(ButtonPressedHandler);
    static void onMinNoteArrowButtonPressed(ArrowButtonPressedHandler);
    static void onMaxNoteArrowButtonPressed(ArrowButtonPressedHandler);
    static void onTimeDivisionArrowButtonPressed(ArrowButtonPressedHandler);

    // External clock tick handler
    static void onExternalClockTick(TickHandler);

  private:
    static void _queueTask(Tasks);
    static void _processQueuedTask();
    static void _taskFinished();
    static void _taskQueueInsertAt(Tasks, byte);
    static bool _taskQueueContainsTask(Tasks);
    static void _executeTask(Tasks);
    static void _taskReadAdc();
    static void _taskReadSelectedStep();
    static void _taskWriteDac();
    static void _taskReadPortExp();
    static void _taskProcessPortExpInterrupt();
    static void _taskWriteDisplay();

    static void _processPortExpanderInterrupt();
    static void _processRunStopPressedInterrupt();
    static void _processExternalClockTick();

    static void _internalHandleSequenceModeButtonPressed();
    static void _internalHandleGateButtonPressed();
    static void _internalHandleRepeatButtonPressed();
    static void _internalHandleRunStopButtonPressed();
    static void _internalHandleScaleButtonPressed();

    static void _setupArrowButtonHandler();
    static void _internalHandleUpArrowButtonPressed();
    static void _internalHandleDownArrowButtonPressed();
    static void _handleArrowButtonPressed(bool);

    static void _noopArrowButtonPressedHandler(bool);

    volatile static bool _spiBusy;
    volatile static Tasks _taskQueue[MAX_TASK_QUEUE_LENGTH];
    volatile static byte _taskQueueLength;
    volatile static unsigned int _queuedDacValue;
    volatile static byte _cachedSelectedStep;
    static const unsigned int _stepSize = (ADC_MAX - 8) / (NUM_STEPS + 1);
    static const unsigned int _halfStepSize = _stepSize / 2;
    static char *_queuedDisplayValue;
    static AdcReadHandler _adcReadHandler;
    static bool _arrowButtonHandlerSetup;

    static ButtonPressedHandler _sequenceModeButtonPressedHandler;
    static ButtonPressedHandler _gateButtonPressedHandler;
    static ButtonPressedHandler _repeatButtonPressedHandler;
    static ButtonPressedHandler _runStopButtonPressedHandler;
    static ButtonPressedHandler _scaleButtonPressedHandler;

    static ArrowButtonPressedHandler _minNoteArrowButtonPressedHandler;
    static ArrowButtonPressedHandler _maxNoteArrowButtonPressedHandler;
    static ArrowButtonPressedHandler _timeDivisionArrowButtonPressedHandler;

    static TickHandler _externalClockTickHandler;

    static MAX72S19 _display;
    static MCP23S17 _portExp;
    static MCP3202 _adc;
    static MCP492X _dac;
};

#endif // MODULE_IO_H
