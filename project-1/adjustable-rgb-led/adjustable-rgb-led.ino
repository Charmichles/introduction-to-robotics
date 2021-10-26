
#define MAX_POT_VALUE 1023
#define MAX_LED_VALUE 255
#define INTENSITY_MULTIPLIER 0.25

// the pins below are ordered according to the color they control - R G B
uint8_t rgbLedPins[3] = {9, 10, 11};
uint8_t potPins[3] = {A0, A3, A5};
float potValue;
uint8_t ledIntensity;

void setup() {
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(rgbLedPins[i], OUTPUT);
    pinMode(potPins[i], INPUT);
  }
}

void loop() {
  for (uint8_t i = 0; i < 3; i++) {
    potValue = analogRead(potPins[i]);
    ledIntensity = map(potValue, 0, MAX_POT_VALUE, 0, MAX_LED_VALUE) * INTENSITY_MULTIPLIER;
    analogWrite(rgbLedPins[i], ledIntensity);
  }
}
