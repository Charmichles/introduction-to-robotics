#define MAX_ANALOG_READ 1023

const uint8_t pinA = 2;
const uint8_t pinB = 3;
const uint8_t pinC = 4;
const uint8_t pinD = 5;
const uint8_t pinE = 6;
const uint8_t pinF = 7;
const uint8_t pinG = 8;
const uint8_t pinDP = 9;

const uint8_t segSize = 8;
const uint8_t segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

const uint8_t nrOfDigits = 10;
const bool dpState = LOW;
bool digitMatrix[nrOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

const uint8_t antennaPin = A5;
const unsigned int readSensitivity = 15;
const unsigned int samplesPerRead = 50;
unsigned int readings[samplesPerRead];
unsigned int readingIdx = 0;
unsigned int antennaRead = 0;
unsigned long readSum = 0;
unsigned long readAvg = 0;
uint8_t digitRead = 0;

unsigned int displayDelay = 250;
unsigned long lastDisplayed = 0;
unsigned int zeroReadDelay = 100;
unsigned long lastZero = 0;

const uint8_t buzzerPin = 11;
unsigned int buzzerFreq = 432;
unsigned int buzzDuration = 100;

void displayNumber(uint8_t digit, bool decimalPoint) {
  for (uint8_t i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(pinDP, decimalPoint);
}

void setup() {
  for (uint8_t i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(antennaPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  for (uint8_t i = 0; i < samplesPerRead; i++) {
    readings[i] = 0;
  }
  Serial.begin(9600);
}

void loop() {
  antennaRead = analogRead(antennaPin);
  if (antennaRead > 0) {
    antennaRead = constrain(antennaRead, 1, readSensitivity);
    antennaRead = map(antennaRead, 1, readSensitivity, 0, MAX_ANALOG_READ);
    readSum -= readings[readingIdx];
    readings[readingIdx] = antennaRead;
    readSum += readings[readingIdx];
    readingIdx = (readingIdx + 1) % samplesPerRead;
    readAvg = readSum / samplesPerRead;
    digitRead = map(readAvg, 0, MAX_ANALOG_READ, 0, nrOfDigits);
  }
  else if (lastZero == 0) {
    lastZero = millis();
  }
  if (millis() - lastZero > zeroReadDelay) {
    digitRead = 0;
    lastZero = 0;
  }
  if (millis() - lastDisplayed > displayDelay) {
     displayNumber(digitRead, dpState);
     tone(buzzerPin, buzzerFreq * digitRead, buzzDuration);
     lastDisplayed = millis();
  }
}
