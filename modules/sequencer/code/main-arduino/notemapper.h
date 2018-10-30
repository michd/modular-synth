#ifndef NOTEMAPPER_H
#define NOTEMAPPER_H

#ifndef ADC_MAX
#define ADC_MAX 4096
#endif // ADC_MAX

#ifndef DAC_MAX
#define DAC_MAX 4096
#endif // DAC_MAX

#define MIN_NOTE_MIDI 24 // C2
#define MAX_NOTE_MIDI 108 // C9
#define NOTE_RANGE 84

#define DEFAULT_MIN_NOTE 12
#define DEFAULT_MAX_NOTE 36

#define SCALE_CHROMATIC 0
#define SCALE_C_MAJOR 1
#define SCALE_PENTATONIC 2
#define SCALE_BLUES 3
#define SCALE_HEXATONIC 4

#define MAX_SCALE 4

#define SCALE_TERMINATOR 0xFF

#include "settings.h"
#include <Arduino.h>
#include <String.h>
#include <stdint.h>

class NoteMapper {
  public:
    static void init();
    // Maps a ADC reading to a note base on current range and such
    static uint8_t mapToNote(uint16_t);

    static uint16_t getNoteOutput(uint8_t);

    // Builds a String representation of a note, for display
    static String getNoteText(uint8_t);

    // String representation of a scale, for display
    static String getScaleText(uint8_t);

    // Sets the min note up or down, and returns the new min note
    static uint8_t cycleMinNote(bool);

    // Sets the max note up or down, and returns the new max note
    static uint8_t cycleMaxNote(bool);

    // Cycles to the next defined scale and returns the scale identifier
    static uint8_t cycleScale();

    // Writes notemapper's settings to the given settings struct
    static void collectSettings(Settings *settingsToSave);

    // Given settings struct, changes local settings to the provided ones
    static void loadFromSettings(Settings *settings);
  
  private:
    static uint8_t* _getScale(uint8_t);
    static uint8_t _getClosest(uint8_t, uint8_t*);
    static uint16_t _noteOutputValues[];
    static volatile uint8_t _selectedScale;
    static volatile uint8_t _rangeMinNote;
    static volatile uint8_t _rangeMaxNote;    
};

#endif // NOTEMAPPER_H
