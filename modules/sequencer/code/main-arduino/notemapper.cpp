#include "notemapper.h"

uint16_t NoteMapper::_noteOutputValues[NOTE_RANGE];
volatile uint8_t NoteMapper::_selectedScale = SCALE_CHROMATIC;
volatile uint8_t NoteMapper::_rangeMinNote = DEFAULT_MIN_NOTE;
volatile uint8_t NoteMapper::_rangeMaxNote = DEFAULT_MAX_NOTE;

void NoteMapper::init() {
  double noteStep = (double)DAC_MAX / (double)(NOTE_RANGE - 1);

  for (uint8_t i = 0; i < NOTE_RANGE; i++) {
    _noteOutputValues[i] = (uint16_t)round(noteStep * (double)i);
  }
}

uint8_t NoteMapper::mapToNote(uint16_t input) {
  uint8_t note = (uint8_t)(
      ((double)input / (double)ADC_MAX) * 
      (_rangeMaxNote - _rangeMinNote + 1) + _rangeMinNote
    );

  uint8_t baseNote = note % 12;
  uint8_t mappedBaseNote = _getClosest(baseNote, _getScale(_selectedScale));

  return note - baseNote + mappedBaseNote; 
}

uint16_t NoteMapper::getNoteOutput(uint8_t note) {
  note = constrain(note, 0, NOTE_RANGE -1);
  return _noteOutputValues[note];
}

String NoteMapper::getNoteText(uint8_t note) {
  String octaveText = String(((note + MIN_NOTE_MIDI) / 12));
  uint8_t baseNote = note % 12;

  switch (baseNote) {
    case  0: return "C  " + octaveText;
    case  1: return "Db " + octaveText;
    case  2: return "D  " + octaveText;
    case  3: return "Eb " + octaveText;
    case  4: return "E  " + octaveText;
    case  5: return "F  " + octaveText;
    case  6: return "Gb " + octaveText;
    case  7: return "G  " + octaveText;
    case  8: return "Ab " + octaveText;
    case  9: return "A  " + octaveText;
    case 10: return "Bb " + octaveText;
    case 11: return "B  " + octaveText;
    default: return "----";
  }
}

String NoteMapper::getScaleText(uint8_t scale) {
  switch (scale) {
    case SCALE_C_MAJOR: return "CMaj";
    case SCALE_PENTATONIC: return "PENT";
    case SCALE_BLUES: return "BLUE";
    case SCALE_HEXATONIC: return "HEXA";
    case SCALE_CHROMATIC:
    default:
       return "Chro";
  }
}

uint8_t NoteMapper::cycleMinNote(bool up) {
  if (up) {
    if (_rangeMinNote < NOTE_RANGE) _rangeMinNote++;
  } else {
    if (_rangeMinNote > 0) _rangeMinNote--;
  }

  return _rangeMinNote;
}

uint8_t NoteMapper::cycleMaxNote(bool up) {
  if (up) {
    if (_rangeMaxNote < NOTE_RANGE) _rangeMaxNote++;
  } else {
    if (_rangeMaxNote > 0) _rangeMaxNote--;
  }

  return _rangeMaxNote; 
}

uint8_t NoteMapper::cycleScale() {
  uint8_t nextScale = _selectedScale + 1;
  if (nextScale > MAX_SCALE) nextScale = 0;
  _selectedScale = nextScale;
  return nextScale;
}

void NoteMapper::collectSettings(Settings *settingsToSave) {
  settingsToSave->scale = _selectedScale;
  settingsToSave->minNote = _rangeMinNote;
  settingsToSave->maxNote = _rangeMaxNote;
}

void NoteMapper::loadFromSettings(Settings *settings) {
  _selectedScale = constrain(settings->scale, 0, MAX_SCALE);
  _rangeMinNote = constrain(settings->minNote, 0, NOTE_RANGE);
  _rangeMaxNote = constrain(settings->maxNote, _rangeMinNote, NOTE_RANGE);
}

uint8_t* NoteMapper::_getScale(uint8_t scaleIndex) {
  static uint8_t chromaticScale[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, SCALE_TERMINATOR };
  static uint8_t cMajorScale[] = { 0, 2, 4, 5, 7, 9, 11, SCALE_TERMINATOR };
  static uint8_t pentatonicScale[] = { 1, 3, 6, 8, 10, SCALE_TERMINATOR };
  static uint8_t bluesScale[] = { 1, 3, 6, 8, 9, 10, SCALE_TERMINATOR };
  static uint8_t hexatonicScale[] = { 0, 2, 4, 6, 8, 10, SCALE_TERMINATOR };

  switch (scaleIndex) {
    case SCALE_CHROMATIC: return chromaticScale;
    case SCALE_C_MAJOR: return cMajorScale;
    case SCALE_PENTATONIC: return pentatonicScale;
    case SCALE_BLUES: return bluesScale;
    case SCALE_HEXATONIC: return hexatonicScale;
    default: return chromaticScale;
  }
}

uint8_t NoteMapper::_getClosest(uint8_t baseNote, uint8_t *scale) {
  baseNote = constrain(baseNote, 0, 12);
  uint8_t i = 0;

  while (baseNote > *(scale + i) && *(scale + i) != SCALE_TERMINATOR) i++;

  if (*(scale + i) == SCALE_TERMINATOR) {
    return baseNote;
  }

  // If we have an identical match, return base note as is
  if (baseNote == *(scale + i)) return baseNote;

  // If the very first note in the scale was higher than our baseNote, return that note
  if (i == 0) return *scale;

  // Otherwise, determine whether the note above or under is closer, and return closest
  if ((baseNote - *(scale + i - 1)) < (*(scale + i) - baseNote)) {
    return *(scale + i - 1);
  } else {
    return *(scale + i);
  }
}
