#define DIGIT_COUNT 2
#define PIN_LATCH 3
#define PIN_CLK 4
#define PIN_DATA 5
#define PIN_BUTTON 2
#define POWERUP_MS 200
#define BUTTON_DEBOUNCE_MS 150
#define MIDI_CHANNELS 16

byte DIGITS[10];
volatile byte channel = 1;
volatile long lastButtonPress = 0;

void setup() {
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);

  // Segments:  ABCDEFGdp
  DIGITS[0] = 0b11111100;
  DIGITS[1] = 0b01100000;
  DIGITS[2] = 0b11011010;
  DIGITS[3] = 0b11110010;
  DIGITS[4] = 0b01100110;
  DIGITS[5] = 0b10110110;
  DIGITS[6] = 0b10111110;
  DIGITS[7] = 0b11100000;
  DIGITS[8] = 0b11111110;
  DIGITS[9] = 0b11110110;

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonOnPressed, FALLING); 

  digitalWrite(PIN_LATCH, LOW);
  delay(POWERUP_MS); // Power-up register reset leeway.
  update7Seg(channel);
}

void loop() {
}

void buttonOnPressed() {
  // Debounce
  long currentTime = millis();
  if (currentTime - lastButtonPress < BUTTON_DEBOUNCE_MS) return;
  lastButtonPress = currentTime;

  increment();
}

void increment() {
  channel++;
  if (channel > MIDI_CHANNELS) channel = 1;

  update7Seg(channel);
}

void update7Seg(int number) {
  byte *digits = getDigitBytes(number);

  digitalWrite(PIN_LATCH, HIGH);

  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[1]);
  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[0]);

  // Done writing data
  digitalWrite(PIN_LATCH, LOW);

  // Slightly hacky lacking, but doing this makes the registers actually output
  // the newly shifted data. Maybe that's how it's meant to work.
  digitalWrite(PIN_LATCH, HIGH);
  digitalWrite(PIN_LATCH, LOW);
}

byte * getDigitBytes(int number) {
  static byte digits[DIGIT_COUNT];
  int ones = number % 10;
  int tens = (number - ones) / 10;

  if (tens > 9) tens = 0;

  digits[0] = DIGITS[tens];
  digits[1] = DIGITS[ones];
  return digits;
}
