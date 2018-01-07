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

#define SCALE_CHROMATIC 0
#define SCALE_C_MAJOR 1

#define MAX_SCALE 1

#define SCALE_TERMINATOR 0xFF

#include <Arduino.h>

class NoteMapper {
  public:
    static void init();
    // Maps a ADC reading to a note base on current range and such
    static byte mapToNote(unsigned short int);

    static unsigned int getNoteOutput(byte);

    // Builds a string representation of a note, for display
    static char * getNoteText(byte);

    // String representation of a scale, for display
    static char * getScaleText(byte);

    // Sets the min note up or down, and returns the new min note
    static byte cycleMinNote(bool);

    // Sets the max note up or down, and returns the new max note
    static byte cycleMaxNote(bool);
  
  private:
    static byte* _getScale(byte);
    static byte _getClosest(byte, byte*);
    static unsigned int _noteOutputValues[];
    static volatile byte _selectedScale;
    static volatile byte _rangeMinNote;
    static volatile byte _rangeMaxNote;
    
};

#endif // NOTEMAPPER_H
