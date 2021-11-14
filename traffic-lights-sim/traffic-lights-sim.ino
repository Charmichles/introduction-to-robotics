#define GAME_DELAY_MIN 1000
#define GAME_DELAY_MAX 3000

// Using a PULL-UP resistor for the button, so the default value is HIGH.
const uint8_t buttonPin = 13;
bool buttonState = HIGH;
const bool defaultButtonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
int debounceDelay;
int reading;

uint8_t trafficState = 0;
int stateDurations[4] = {0, 3000, 10000, 5000};
int startDelay = 10000;
unsigned long lastStateChange = 0;

int blinkDelay = 500;
unsigned long lastBlinked = 0;
bool blinkState = HIGH;

const uint8_t buzzerPin = 10;
int buzzerTone = 432;
int buzzerDuration = 250;
int buzzerDelay = 1000;
unsigned long lastBuzzed = 0;

/*
 * Variables used for a reaction time game.
 * The aim of the game is to press the button when the blue LED is on. A correct button press causes the other blue LED next to the button to switch states.
 * This way the player can easily see if he pressed the button at the correct time.
 * The time between each flash of the game LED is randomized.
 * This also tests whether the traffic simulator still works while the Arduino is doing another task (correct use of timing with millis() instead of the delay() function).
 */
const uint8_t gameScorePin = 9;
const uint8_t gameEventPin = 5;
bool gameEventState = LOW;
bool gameScore = LOW;
int gameEventDuration = 250;
unsigned long lastGameEventTrigger = 0;
int gameEventDelay = 0;

class CarPins {
  public:
    const uint8_t redPin, yellowPin, greenPin;
    CarPins(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin) : redPin(redPin), yellowPin(yellowPin), greenPin(greenPin) {
      pinMode(redPin, OUTPUT);
      pinMode(yellowPin, OUTPUT);
      pinMode(greenPin, OUTPUT);
    }
};

class PedestrianPins {
  public:
    const uint8_t redPin, greenPin;
    PedestrianPins(uint8_t redPin, uint8_t greenPin) : redPin(redPin), greenPin(greenPin) {
      pinMode(redPin, OUTPUT);
      pinMode(greenPin, OUTPUT);
    }
};

const CarPins carLights(2, 4, 7);
const PedestrianPins pedestrianLights(12, 8);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(gameScorePin, OUTPUT);
  pinMode(gameEventPin, OUTPUT);
  randomSeed(millis());
  gameEventDelay = random(GAME_DELAY_MIN, GAME_DELAY_MAX);
  Serial.begin(9600);
}

void loop() {
  reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  // Traffic lights logic.
  switch (trafficState) {
    case 0:  // Green light for cars, red for pedestrians. Check for button pressing.
      digitalWrite(carLights.greenPin, HIGH);
      digitalWrite(pedestrianLights.redPin, HIGH);
      if (buttonState != defaultButtonState) {
        lastStateChange = millis();
        trafficState = 1;
      }
      break;
      
    case 1:  // Yellow light for cars, red for pedestrians.
      if (millis() - lastStateChange > startDelay) {  // turns on the yellow light for cars only if enough time has passed since pressing the button
        if (millis() - lastStateChange - startDelay > stateDurations[trafficState]) { // compensate waiting for startDelay when checking if the current state has run out of time
          lastStateChange = millis();
          trafficState = 2;
          digitalWrite(carLights.yellowPin, LOW);
          digitalWrite(pedestrianLights.redPin, LOW);
        }
        else {
          digitalWrite(carLights.greenPin, LOW);
          digitalWrite(carLights.yellowPin, HIGH);
        }
      }
      break;
      
    case 2:  // Red light for cars, constant green light and intermitent buzzing for pedestrians.
      if (millis() - lastStateChange > stateDurations[trafficState]) {
        lastStateChange = millis();
        trafficState = 3;
      }
      else {
        digitalWrite(carLights.redPin, HIGH);
        digitalWrite(pedestrianLights.greenPin, HIGH);
        if (millis() - lastBuzzed > buzzerDelay) {
          tone(buzzerPin, buzzerTone, buzzerDuration);
          lastBuzzed = millis();
        }
      }
      break;
     
    case 3:  // Red light for cars, intermitent green light and faster intermitent buzzing for pedestrians.
      if (millis() - lastStateChange > stateDurations[trafficState]) {
        trafficState = 0;
        digitalWrite(carLights.redPin, LOW);
        digitalWrite(pedestrianLights.greenPin, LOW);
      }
      else {
        if (millis() - lastBlinked > blinkDelay) {
          blinkState = !blinkState;
          digitalWrite(pedestrianLights.greenPin, blinkState);
          lastBlinked = millis();
        }
        if (millis() - lastBuzzed > buzzerDelay / 2) {
          tone(buzzerPin, buzzerTone, buzzerDuration);
          lastBuzzed = millis();
        }
      }
      break;
  }

  // Timing game logic.
  if (gameEventState == LOW && millis() - lastGameEventTrigger > gameEventDelay) {
    gameEventState = HIGH;
    lastGameEventTrigger = millis();
  }
  if (gameEventState == HIGH && millis() - lastGameEventTrigger > gameEventDuration) {
    gameEventState = LOW;
    lastGameEventTrigger = millis();
    gameEventDelay = random(GAME_DELAY_MIN, GAME_DELAY_MAX);
  }
  if (gameEventState == HIGH && buttonState == LOW) {
    gameScore = !gameScore;
    gameEventState = LOW;
    lastGameEventTrigger = millis();
    gameEventDelay = random(GAME_DELAY_MIN, GAME_DELAY_MAX);
  }

  digitalWrite(gameEventPin, gameEventState);
  digitalWrite(gameScorePin, gameScore);
  lastButtonState = reading;
}
