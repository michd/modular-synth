#include "notemapper.h"

unsigned int NoteMapper::_noteOutputValues[NOTE_RANGE];
volatile byte NoteMapper::_selectedScale = SCALE_CHROMATIC;
volatile byte NoteMapper::_rangeMinNote = 12; // TODO constant for default
volatile byte NoteMapper::_rangeMaxNote = 24;

void NoteMapper::init() {
  float noteStep = (float)DAC_MAX / (float)(NOTE_RANGE - 1);

  for (byte i = 0; i < NOTE_RANGE; i++) {
    _noteOutputValues[i] = (unsigned int)round(noteStep * (float)i);
  }

  _rangeMaxNote = 36;
  _rangeMinNote = 12;
}

uint8_t NoteMapper::mapToNote(unsigned short int input) {
  byte note = (byte)(
      ((float)input / (float)ADC_MAX) * 
      (_rangeMaxNote - _rangeMinNote + 1) + _rangeMinNote
    );

  byte baseNote = note % 12;
  byte mappedBaseNote = _getClosest(baseNote, _getScale(_selectedScale));

  return note - baseNote + mappedBaseNote; 
}

unsigned int NoteMapper::getNoteOutput(byte note) {
  note = constrain(note, 0, NOTE_RANGE -1);
  return _noteOutputValues[note];
}

char * NoteMapper::getNoteText(byte note) {
  static char text[5];
  byte baseNote = note % 12;
  switch (baseNote) {
    case 0:
      text[0] = 'C';
      text[1] = ' ';
      break;
    case 1:
      text[0] = 'D';
      text[1] = 'b';
      break;
    case 2:
      text[0] = 'D';
      text[1] = ' ';
      break;
    case 3:
      text[0] = 'E';
      text[1] = 'b';
      break;
    case 4:
      text[0] = 'E';
      text[1] = ' ';
      break;
    case 5:
      text[0] = 'F';
      text[1] = ' ';
      break;
    case 6:
      text[0] = 'G';
      text[1] = 'b';
      break;
    case 7:
      text[0] = 'G';
      text[1] = ' ';
      break;
    case 8:
      text[0] = 'A';
      text[1] = 'b';
      break;
    case 9:
      text[0] = 'A';
      text[1] = ' ';
      break;
    case 10:
      text[0] = 'B';
      text[1] = 'b';
      break;
    case 11:
      text[0] = 'B';
      text[1] = ' ';
      break;
    case 12:
      text[0] = 'C';
      text[1] = 'b';
      break;
  }

  text[2] = ' ';
  text[3] = '0' + ((note + MIN_NOTE_MIDI) / 12);
  text[4] = '\0';

  return text;
}

char* NoteMapper::getScaleText(byte scale) {
  if (scale == SCALE_C_MAJOR) {
    char text[] = "CMaj";
    return text;
  } else if (scale == SCALE_CHROMATIC) {
    char text[] = "Chro";
    return text;
  } else {
    char text[] = "Chro";
    return text;
  }
}

byte NoteMapper::cycleMinNote(bool up) {
  if (up) {
    if (_rangeMinNote < NOTE_RANGE) _rangeMinNote++;
  } else {
    if (_rangeMinNote > 0) _rangeMinNote--;
  }

  return _rangeMinNote;
}

byte NoteMapper::cycleMaxNote(bool up) {
  if (up) {
    if (_rangeMaxNote < NOTE_RANGE) _rangeMaxNote++;
  } else {
    if (_rangeMaxNote > 0) _rangeMaxNote--;
  }

  return _rangeMaxNote; 
}

byte* NoteMapper::_getScale(byte scaleIndex) {
  static byte chromaticScale[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, SCALE_TERMINATOR };
  static byte cMajorScale[] = { 0, 2, 4, 5, 7, 9, 11, SCALE_TERMINATOR };

  switch (scaleIndex) {
    case SCALE_CHROMATIC: return chromaticScale;
    case SCALE_C_MAJOR: return cMajorScale;
    default: return chromaticScale;
  }
}

byte NoteMapper::_getClosest(byte baseNote, byte *scale) {
  baseNote = constrain(baseNote, 0, 12);
  byte i = 0;

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
