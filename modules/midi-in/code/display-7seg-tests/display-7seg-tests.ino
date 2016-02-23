#define DIGIT_COUNT 2
#define PIN_LATCH 3
#define PIN_CLK 4
#define PIN_DATA 5
#define PIN_RESET 6
#define PIN_BUTTON 2

byte DIGITS[10];
volatile byte channel = 1;
volatile long lastButtonPress = 0;

void setup() {
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
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
  digitalWrite(PIN_RESET, LOW);  
  delay(5);
  digitalWrite(PIN_RESET, HIGH);
  // TODO: ugly hack for weird bug to be removed. Figure out
  // why I'm having to run this twice.
  update7Seg(1);
  update7Seg(1);
}

void loop() {
}

void buttonOnPressed() {
  // Debounce
  long currentTime = millis();
  if (currentTime - lastButtonPress < 150) return;
  lastButtonPress = currentTime;
  
  increment();
}

void increment() {
  channel++;
  if (channel > 16) channel = 1;

  // TODO: ugly hack for weird bug to be removed. Figure out
  // why I'm having to run this twice.
  update7Seg(channel);
  update7Seg(channel);
}

void update7Seg(int number) {
  byte *digits = getDigitBytes(number);
 
  // Listen for data
  digitalWrite(PIN_LATCH, HIGH);

  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[1]);
  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, ~digits[0]);

  // Done writing data
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


