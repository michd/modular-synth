#ifndef SETTINGS_H
#define SETTINGS_H

#ifndef NUM_STEPS
#define NUM_STEPS 8
#endif

#include <stdint.h>

// Container for all the settings that can be changed in Sequence and NoteMapper
struct Settings {
  // Gate mode, one for each step, see Sequence GATE_MODE_x constants
  uint8_t gateModes[NUM_STEPS];

  // Step repetitions, one for each step
  uint8_t stepRepeat[NUM_STEPS];

  // Musical scale, one of the NoteMapper SCALE_x constants
  // Used for mapping to desired notes from pot values
  uint8_t scale;

  // Notemapper minimum note, sets the minumum note that can be reached with
  // the step pots
  uint8_t minNote;

  // Notemapper maximum note, sets the maximum note that can be reached with
  // the step pots
  uint8_t maxNote;

  // Timing divider (1/x) - should only be 1,2,4,8, or 16
  // This means one step lasts for this long musically 1/4 would be 1 beat
  uint8_t timeDivider;

  // Sequencer sequence mode, one of the 8 SEQUENCE_MODE constants
  // Refers to the order in which the steps are played; there are 8 modes, 0-7
  uint8_t sequenceMode;
};  

class SettingsManager {
  public:
    // Settings loaded upon powerup
    static Settings defaultSettings;
    // Load settings from a given slot in eeprom (0-7 and return as the struct)
    static Settings load(uint8_t);
    // Save given settings to a given slot in eeprom
    static void save(Settings, uint8_t);
};

#endif // SETTINGS_H
