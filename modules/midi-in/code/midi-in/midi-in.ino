#include <SoftwareSerial.h>

#define MIN_MIDI_BYTES 3
#define PIN_GATE 7
#define PIN_TRIGGER 8

#define STATUS_NOTE_ON 0x90
#define STATUS_NOTE_OFF 0x80

SoftwareSerial sSerial(15,14);

unsigned short notesOn = 0;

void setup() {
  pinMode(PIN_GATE, OUTPUT);
  pinMode(PIN_TRIGGER, OUTPUT);
  
  sSerial.begin(31250);
}

void loop() {
  do {
    if (!sSerial.available()) continue;

    byte data[3];
    
    data[0] = sSerial.read();
    data[1] = sSerial.read();
    data[2] = sSerial.read();
    
    processMessage(data);
  }
  while(sSerial.available() >= MIN_MIDI_BYTES);
}

void processMessage(byte data[3]) {
  if (data[0] == STATUS_NOTE_ON) {
    notesOn++;
    digitalWrite(PIN_GATE, HIGH);
    digitalWrite(PIN_TRIGGER, HIGH);
    delay(1);
    digitalWrite(PIN_TRIGGER, LOW);
  } else if (data[0] == STATUS_NOTE_OFF) {
    notesOn--;
    if (notesOn == 0) {
      digitalWrite(PIN_GATE, LOW);
    }
  }
}
