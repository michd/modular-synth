#define DIGIT_COUNT 2
#define PIN_LATCH 3
#define PIN_CLK 4
#define PIN_DATA 2

byte DIGITS[10];
byte value = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  Serial.begin(9600);

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

  digitalWrite(PIN_LATCH, LOW);
  update7Seg(1);

  // TODO: bind reset to board, set high here
  // TODO: add pushbutton on an interrupt pin
  // TODO: increment number upon debounced push
}

void loop() {

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


