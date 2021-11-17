#define PICK_DISPLAY false
#define PICK_DIGIT true
#define JOY_MOVE_ERROR 50

const uint8_t dataPin = 12; // DS
const uint8_t storagePin = 11; // STCP
const uint8_t shiftPin = 10; // SHCP

const uint8_t segD1 = 7;
const uint8_t segD2 = 6;
const uint8_t segD3 = 5;
const uint8_t segD4 = 4;

const uint8_t displayCount = 4;
uint8_t displayPins[displayCount] = {segD1, segD2, segD3, segD4};
int displayDigits[displayCount];  // the digit shown on each display - is in reverse order of the pins the digits are displayed on
bool displayDecimalPoint[displayCount];  // state of decimal point on each display
int dpIdx = 0;
int digitIdx = 0;
bool dpState = true;
unsigned long dpBlinkDelay = 500;
unsigned long dpBlinkDuration = 500;
unsigned long dpLastBlink = 0;

bool gameState = PICK_DISPLAY;
uint16_t currentNumber = 0;

uint8_t nrOfDigits = 10;
uint8_t digitArray[16] = {
// ABCDEFGP 
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110   // F
};

const uint8_t vxPin = A0;
const uint8_t vyPin = A1;
const uint8_t buttonPin = 3;
uint16_t vxRead = 0;
uint16_t vyRead = 0;
uint16_t xDefault = 511;
uint16_t yDefault = 511;
uint16_t threshMin = 350;
uint16_t threshMax = 750;
bool buttonState = HIGH;
bool lastButtonState = HIGH;
bool buttonDebounceRead;
unsigned long debounceDelay = 100;
unsigned long lastDebounceTime = 0;
bool stickMove = false;

void writeDigit(uint8_t digit, bool showDecimalPoint) {
  digitalWrite(storagePin, LOW);
  digit |= showDecimalPoint;  // in the bitstring representation of the digit the last bit is the decimal point, so we can control it with this bitwise or
  shiftOut(dataPin, shiftPin, MSBFIRST, digit);
  digitalWrite(storagePin, HIGH);
}

void activateDisplay(uint8_t displayPin) {
  for (uint8_t i = 0; i < displayCount; i++) {
    digitalWrite(displayPins[i], HIGH);
  }
  digitalWrite(displayPin, LOW);
}

void displayNumber(uint16_t number) {
  for (uint8_t i = 0; i < displayCount; i++) {
    activateDisplay(displayPins[i]);
    writeDigit(digitArray[number % 10], displayDecimalPoint[i]);
    delay(5);  // for muxing, duration is too low to make a noticeable difference
    number /= 10;
  }
}

void blinkDecimalPoint() {
  if (dpState == true && millis() - dpLastBlink > dpBlinkDelay) {
    dpState = false;
    dpLastBlink = millis();
  }
  if (dpState == false && millis() - dpLastBlink > dpBlinkDuration) {
    dpState = true;
    dpLastBlink = millis();
  }
  displayDecimalPoint[dpIdx] = dpState;
}

void handleDecimalPoint() {
  blinkDecimalPoint();
  vyRead = analogRead(vyPin);
  if (vyRead >= yDefault - JOY_MOVE_ERROR && vyRead <= yDefault + JOY_MOVE_ERROR) {  // making sure the joystick is moved along the correct axis (variable error)
    vxRead = analogRead(vxPin);
    if (vxRead <= threshMin && vyRead >= yDefault - 10 && vyRead <= yDefault + 10 && stickMove == false) {
      displayDecimalPoint[dpIdx] = false;
      dpIdx++;
      stickMove = true;
    }
    if (vxRead >= threshMax && stickMove == false) {
      displayDecimalPoint[dpIdx] = false;
      dpIdx--;
      stickMove = true;
    }
    if (vxRead > threshMin && vxRead < threshMax) {
      stickMove = false;
    }
    // cyclic display
    if (dpIdx < 0) {
      dpIdx = displayCount - 1;
    }
    if (dpIdx >= displayCount) {
      dpIdx = 0;
    }
  }
}

void handleDigit() {
  digitIdx = displayCount - 1 - dpIdx;  // the order of the digits in the number is reversed from the order of the display pins (1234 -> 1 on D4, 2 on D3, etc.)
  vxRead = analogRead(vxPin);
  if (vxRead >= xDefault - JOY_MOVE_ERROR && vxRead <= xDefault + JOY_MOVE_ERROR) {  // making sure the joystick is moved along the correct axis (variable error)
    vyRead = analogRead(vyPin);
    if (vyRead <= threshMin && stickMove == false) {
      displayDigits[digitIdx]--;
      stickMove = true;
    }
    if (vyRead >= threshMax && stickMove == false) {
      displayDigits[digitIdx]++;
      stickMove = true;
    }
    if (vyRead > threshMin && vyRead < threshMax) {
      stickMove = false;
    }
    // cyclic display
    if (displayDigits[digitIdx] < 0) {
      displayDigits[digitIdx] = nrOfDigits - 1;
    }
    if (displayDigits[digitIdx] >= nrOfDigits) {
      displayDigits[digitIdx] = 0;
    }
  }
}

void changeGameState() {
  displayDecimalPoint[dpIdx] = true;  // at the start of each state the decimal point is turned on
  switch (gameState) {
    case PICK_DISPLAY:
      gameState = PICK_DIGIT;
      break;
    case PICK_DIGIT:
      gameState = PICK_DISPLAY;
      break;
    default:
      return;
  }
}

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(storagePin, OUTPUT);
  pinMode(shiftPin, OUTPUT);
  for (uint8_t i = 0; i < displayCount; i++) {
    pinMode(displayPins[i], OUTPUT);
    digitalWrite(displayPins[i], LOW);
  }
  pinMode(vxPin, INPUT);
  pinMode(vyPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), changeGameState, FALLING);
  Serial.begin(9600);
}

void loop() {
  currentNumber = 0;
  for (uint8_t i = 0; i < displayCount; i++) {
    currentNumber = currentNumber * 10 + displayDigits[i];
  }
  displayNumber(currentNumber);

  buttonDebounceRead = digitalRead(buttonPin);
  if (buttonDebounceRead != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > debounceDelay && buttonDebounceRead != buttonState) {
    buttonState = buttonDebounceRead;
  }
  lastButtonState = buttonDebounceRead;

  switch (gameState) {
    case PICK_DISPLAY:
      handleDecimalPoint();
      break;
    case PICK_DIGIT:
      handleDigit();
      break;
    default:
      return;
  }
}
