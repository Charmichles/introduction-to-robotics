#define MAX_POT_VALUE 1023
#define MAX_LED_VALUE 255
#define INTENSITY_MULTIPLIER 1
#define MAX_PROGRESS_PER_COLORED_LED MAX_LED_VALUE / 3
#define ERROR_MARGIN 2

// the pins below are ordered according to the color they control - R G B
uint8_t rgbLedPins[3] = {9, 10, 11};
uint8_t potPins[3] = {A0, A3, A5};
float potValue;
uint8_t ledIntensity;

uint8_t gameLedPin = 6;
uint8_t gameLedIntensity = 0;
uint8_t targetColors[3] = {4, 139, 222};

/*
 * The objective was to modify the intensity of a single color LED to match the progress in reaching a target color using a controllable RGB LED.
 * It means that each of the colors should modify 1/3 of the range of the single color LED ==> MAX_PROGRESS_PER_COLORED_LED.
 * Ideally, this should use a mathematical function to map the progress to a value in the interval [0, MAX_PROGRESS_PER_COLORED_LED].
 * For now, it just returns MAX_PROGRESS_PER_COLORED_LED if the color got within an error margin of the target color and 0 otherwise.
 * This results in a LED that has 4 possible states depending on how many colors got close enough to the target.
 */
uint8_t progressFunction(uint8_t color, uint8_t targetColor) {
  return 0;
}
uint8_t getIntensityOfColorProgress(uint8_t color, uint8_t targetColor) {
  return color >= targetColor - ERROR_MARGIN && color <= targetColor + ERROR_MARGIN ? MAX_PROGRESS_PER_COLORED_LED : progressFunction(color, targetColor);
}

void setup() {
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(rgbLedPins[i], OUTPUT);
    pinMode(potPins[i], INPUT);
  }
  pinMode(gameLedPin, OUTPUT);
}

void loop() {
  gameLedIntensity = 0;
  for (uint8_t i = 0; i < 3; i++) {
    potValue = analogRead(potPins[i]);
    ledIntensity = map(potValue, 0, MAX_POT_VALUE, 0, MAX_LED_VALUE) * INTENSITY_MULTIPLIER;
    analogWrite(rgbLedPins[i], ledIntensity);
    gameLedIntensity += getIntensityOfColorProgress(ledIntensity, targetColors[i]);
  }
  analogWrite(gameLedPin, gameLedIntensity);
}
