#include "settings.h"
#include <EEPROM.h>

// Byte written before the area taken by the actual settings
// If this byte is present at the correct address, the following
// address spaces contain a patch
#define SETTING_INDEX_HAS_VALUE_INDICATOR 0b10101010

Settings SettingsManager::defaultSettings = {
  gateModes: { 0, 0, 0, 0, 0, 0, 0, 0 },
  stepRepeat: { 1, 1, 1, 1, 1, 1, 1, 1 },
  scale: 0,
  minNote: 12,
  maxNote: 36,
  timeDivider: 16,
  sequenceMode: 0
};

// Load settings from EEPROM, given a slot index (0-NUM_STEPS)
Settings SettingsManager::load(uint8_t index) {
  // Don't try to retrieve settings that can't exist.
  // Instead of constraining to an existing one and getting unexpected results,
  // return default settings instead.
  // Note: The slots are 1-indexed when using step buttons; if none is specified
  // slot 0 is used. This is good for comparing patches and not accidentally
  // overwriting patch 1.
  if (index > NUM_STEPS) return defaultSettings;

  // Located stating address, based on size of Settings struct + 1 for
  // "contains patch" indicator value
  unsigned int address = index * (sizeof(Settings) + 1);
  
  // If our magic value wasn't found, there are no settings here,
  // return default settings instead
  if (EEPROM.read(address) != SETTING_INDEX_HAS_VALUE_INDICATOR) {
    return defaultSettings;
  }

  Settings loadedSettings;

  // Read the gate mode batch
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    loadedSettings.gateModes[i] = EEPROM.read(++address);
  }

  // Read the step repeat bytes
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    loadedSettings.stepRepeat[i] = EEPROM.read(++address);
  }

  // Read the remaining, simply settings
  loadedSettings.scale = EEPROM.read(++address);
  loadedSettings.minNote = EEPROM.read(++address);
  loadedSettings.maxNote = EEPROM.read(++address);
  loadedSettings.timeDivider = EEPROM.read(++address);
  loadedSettings.sequenceMode = EEPROM.read(++address);

  return loadedSettings;
}

void SettingsManager::save(Settings settings, uint8_t index) {
  // Don't attempt to save if index is out of bounds.
  // Note: The slots are 1-indexed when using step buttons; if none is specified
  // slot 0 is used. This is good for comparing patches and not accidentally
  // overwriting patch 1.
  if (index > NUM_STEPS) return;

  unsigned int address = index * (sizeof(Settings) + 1);

  // Write our indicator, indicating that a valid set of settings comes after
  EEPROM.write(address, SETTING_INDEX_HAS_VALUE_INDICATOR);

  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    EEPROM.write(++address, settings.gateModes[i]);
  }

  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    EEPROM.write(++address, settings.stepRepeat[i]);
  }

  EEPROM.write(++address, settings.scale);
  EEPROM.write(++address, settings.minNote);
  EEPROM.write(++address, settings.maxNote);
  EEPROM.write(++address, settings.timeDivider);
  EEPROM.write(++address, settings.sequenceMode);
}
