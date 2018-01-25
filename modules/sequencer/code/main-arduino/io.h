#ifndef MODULE_IO_H
#define MODULE_IO_H

// Chip select pins for the various SPI devices being used
#define PIN_SPI_CS_ADC 6
#define PIN_SPI_CS_DAC A2
#define PIN_SPI_CS_PORTEXP A0
#define PIN_SPI_CS_DISP 9

#define PIN_EXTERNAL_CLOCK 7

#define PIN_RUNNING_INDICATOR 8
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
#define PORTEXP_PIN_RUN_MODE_BUTTON 12
#define PORTEXP_PIN_SCALE_BUTTON 13
#define PORTEXP_PIN_RESET_BUTTON 14
#define PORTEXP_PIN_LOAD_BUTTON 15
#define PORTEXP_PIN_SAVE_BUTTON 16
#define PORTEXP_PIN_UP_ARROW 1
#define PORTEXP_PIN_DOWN_ARROW 2
#define PORTEXP_PIN_PARAM_SELECT_A 3
#define PORTEXP_PIN_PARAM_SELECT_B 4
#define PORTEXP_PIN_CHAIN_INPUT 5
#define PORTEXP_PIN_CHAIN_OUTPUT 6

#define PORTEXP_NUM_PINS 16

#define PORT_EXPANDER_CHANNEL 0

#define DEBOUNCE_TIME_MS 100

#define PARAM_MIN_NOTE 0b01
#define PARAM_MAX_NOTE 0b11
#define PARAM_TIME_DIVIDER 0b10

// Least significant byte describes bits to be written to a row
// Most significat bits describe which row to use
// Split up accordingly
#define LED_INDICATOR_NONE      0x400 // None of those LEDs on
#define LED_INDICATOR_STEP_NOTE 0x440
#define LED_INDICATOR_MAX_NOTE  0x420
#define LED_INDICATOR_MIN_NOTE  0x410

#define ADC_CV_CHANNEL 0
#define ADC_STEP_CHANNEL 1

#define ADC_RESOLUTION_BIT 12
#define DAC_RESOLUTION_BIT 12

#define DAC_CENTER_VALUE 2048
#define ADC_MAX 4096
#define DAC_MAX 4096

#define NUM_STEPS 8

// There are only 7 possible tasks, and none may be duplicated in the queue
#define MAX_TASK_QUEUE_LENGTH 7
#define DISPLAY_STRING_LENGTH 5 // 1 for the null terminator

#include <Arduino.h>
#include <String.h>
#include <stdint.h>

#include "MAX72S21.h" // Display driver
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
    WRITE_DISPLAY,
    WRITE_LEDS
  };

enum LEDs {
    INDICATOR_NONE,
    INDICATOR_STEP_NOTE,
    INDICATOR_MIN_NOTE,
    INDICATOR_MAX_NOTE
  };


typedef void (*AdcReadHandler)(unsigned int);
typedef void (*ButtonPressedHandler)(void);
typedef void (*ArrowButtonPressedHandler)(bool);
typedef void (*TickHandler)(void);
typedef void (*PinChangedHandler)(bool);

class IO {
  public:    
    static void init();

    static void setGate(bool);

    static void setTrigger(bool);

    static void setRunningIndicator(bool);

    // Drive multiplexers and decoder for selecting the current step
    static void setStep(uint8_t);

    // write to DAC (SPI)
    static void setPitch(uint16_t);

    // Read from the ADC, processing the result with a handler function
    static void readAdc(AdcReadHandler);

    // Routinely read port expander values
    static void readPortExp();

    // Read the cached data from port expander
    static uint16_t readPortExpCache();

    static bool getPortExpPin(uint8_t);

    static uint8_t getSelectedParam();

    // Read and derive the pressed step from the ADC
    // The step buttons are set up as a little keyboard, resistors between them
    // We read an ADC value to figure out which one's pressed, if any
    static void readSelectedStep();

    static uint8_t getSelectedStep();

    // Note: very limiting but should cover needs pretty well for now
    // Write 4 characters to the display
    static void writeDisplay(String);

    // Writes one of a configuration of LEDs
    // This is an enum which covers needs for now (light one out of a set of 3)
    // but could be updated to take a uint16_t instead to cover LED_INDICATOR_*
    // constant formats directly
    static void writeLeds(LEDs);

    static void setChainOut(bool);
    static bool getChainOut();

    // Assign button press handlers
    static void onSequenceModeButtonPressed(ButtonPressedHandler);
    static void onGateButtonPressed(ButtonPressedHandler);
    static void onRepeatButtonPressed(ButtonPressedHandler);
    static void onRunModeButtonPressed(ButtonPressedHandler);
    static void onScaleButtonPressed(ButtonPressedHandler);
    static void onResetButtonPressed(ButtonPressedHandler);
    static void onLoadButtonPressed(ButtonPressedHandler);
    static void onSaveButtonPressed(ButtonPressedHandler);
    static void onMinNoteArrowButtonPressed(ArrowButtonPressedHandler);
    static void onMaxNoteArrowButtonPressed(ArrowButtonPressedHandler);
    static void onTimeDivisionArrowButtonPressed(ArrowButtonPressedHandler);
    static void onChainInputChanged(PinChangedHandler);

    // External clock tick handler
    static void onExternalClockTick(TickHandler);

  private:
    static void _queueTask(Tasks);
    static void _processQueuedTask();
    static void _taskFinished();
    static void _taskQueueInsertAt(Tasks, uint8_t);
    static bool _taskQueueContainsTask(Tasks);
    static void _executeTask(Tasks);
    static void _taskReadAdc();
    static void _taskReadSelectedStep();
    static void _taskWriteDac();
    static void _taskReadPortExp();
    static void _taskProcessPortExpInterrupt();
    static void _taskWriteDisplay();
    static void _taskWriteLeds();

    static void _processPortExpanderInterrupt();
    static void _processRunStopPressedInterrupt();
    static void _processExternalClockTick();
    static bool _debounceButton(uint8_t);

    static void _internalHandleSequenceModeButtonPressed();
    static void _internalHandleGateButtonPressed();
    static void _internalHandleRepeatButtonPressed();
    static void _internalHandleRunModeButtonPressed();
    static void _internalHandleScaleButtonPressed();
    static void _internalHandleResetButtonPressed();
    static void _internalHandleLoadButtonPressed();
    static void _internalHandleSaveButtonPressed();
    

    static void _setupArrowButtonHandler();
    static void _internalHandleUpArrowButtonPressed();
    static void _internalHandleDownArrowButtonPressed();
    static void _handleArrowButtonPressed(bool);

    static void _internalHandleChainInputChanged();

    static void _noopArrowButtonPressedHandler(bool);

    volatile static bool _spiBusy;
    volatile static Tasks _taskQueue[MAX_TASK_QUEUE_LENGTH];
    volatile static uint8_t _taskQueueLength;
    volatile static unsigned int _queuedDacValue;
    volatile static uint8_t _cachedSelectedStep;
    static const unsigned int _stepSize = (ADC_MAX - 8) / (NUM_STEPS + 1);
    static const unsigned int _halfStepSize = _stepSize / 2;
    static String _queuedDisplayValue;
    static uint16_t _queuedLedsValue;
    static AdcReadHandler _adcReadHandler;
    static bool _arrowButtonHandlerSetup;
    volatile static bool _chainOutputCache;
    volatile static uint32_t _debounceLastPressed[PORTEXP_NUM_PINS];

    static ButtonPressedHandler _sequenceModeButtonPressedHandler;
    static ButtonPressedHandler _gateButtonPressedHandler;
    static ButtonPressedHandler _repeatButtonPressedHandler;
    static ButtonPressedHandler _runModeButtonPressedHandler;
    static ButtonPressedHandler _scaleButtonPressedHandler;
    static ButtonPressedHandler _resetButtonPressedHandler;
    static ButtonPressedHandler _loadButtonPressedHandler;
    static ButtonPressedHandler _saveButtonPressedHandler;

    static ArrowButtonPressedHandler _minNoteArrowButtonPressedHandler;
    static ArrowButtonPressedHandler _maxNoteArrowButtonPressedHandler;
    static ArrowButtonPressedHandler _timeDivisionArrowButtonPressedHandler;

    static PinChangedHandler _chainInputChangedHandler;
    
    static TickHandler _externalClockTickHandler;

    static MAX72S21 _display;
    static MCP23S17 _portExp;
    static MCP3202 _adc;
    static MCP492X _dac;
};

#endif // MODULE_IO_H
