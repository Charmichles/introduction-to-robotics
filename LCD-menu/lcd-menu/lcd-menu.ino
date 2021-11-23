#include <LiquidCrystal.h>

const uint8_t lcdColNum = 16;
const uint8_t lcdRowNum = 2;
const uint8_t RS = 12;
const uint8_t enable = 11;
const uint8_t d4 = 5;
const uint8_t d5 = 4;
const uint8_t d6 = 3;
const uint8_t d7 = 2;

class LcdText {
  private:
    uint8_t col;
    uint8_t row;
    String text;
  public:
    LcdText(uint8_t col, uint8_t row, String text) {
      this->col = col;
      this->row = row;
      this->text = text;
    }

    uint8_t getCol() {
      return col;
    }

    uint8_t getRow() {
      return row;
    }

    String getText() {
      return text;
    }

    void setCol(uint8_t col) {
      this->col = col;
    }

    void setRow(uint8_t row) {
      this->row = row;
    }

    void setText(String text) {
      this->text = text;
    }

    void centerText() {
      this->col = (lcdColNum - this->text.length()) / 2;
    }

    void displayText(LiquidCrystal lcd) {
      lcd.setCursor(this->col, this->row);
      lcd.print(this->text);
    }
};

const uint8_t arrowLeftNum = 0;
const uint8_t arrowRightNum = 1;
byte arrowLeft[8] = {
  B00001,
  B00010,
  B00100,
  B01000,
  B00100,
  B00010,
  B00001
};
byte arrowRight[8] = {
  B10000,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B10000
};

class LcdMenu {
  private:
    LcdText* title;
    uint8_t nrOfOptions;
    LcdText* options;
    uint8_t currentOption;
  public:
    LcdMenu(LcdText* title, uint8_t nrOfOptions, LcdText* options) {
      this->title = title;
      this->title->centerText();
      this->nrOfOptions = nrOfOptions;
      this->options = options;
      for (uint8_t i = 0; i < nrOfOptions; i++) {
        options[i].centerText();
      }
      this->currentOption = 0;
    }
    void displayMenu(LiquidCrystal lcd) {
      title->displayText(lcd);
      options[currentOption].displayText(lcd);
      // display left and right arrows on LCD for navigation
      lcd.setCursor(options[currentOption].getCol() - 2, options[currentOption].getRow());
      lcd.write(byte(arrowLeftNum));
      lcd.setCursor(options[currentOption].getCol() + options[currentOption].getText().length() + 1, options[currentOption].getRow());
      lcd.write(byte(arrowRightNum));
    }
    ~LcdMenu() {
      delete this->title;
      delete this->options;
    }
};

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

const uint8_t nrOfMenus = 3;
uint8_t currentMenu = 0;
LcdMenu mainMenu(new LcdText(0, 0, "Main Menu"), 3, new LcdText[3]{LcdText(0, 1, "Start Game"), LcdText(0, 1, "User Menu"), LcdText(0, 1, "Score")});
LcdMenu userMenu(new LcdText(0, 0, "User Settings"), 2, new LcdText[2]{LcdText(0, 1, "Create User"), LcdText(0, 1, "Delete User")});
LcdMenu scoreMenu(new LcdText(0, 0, "Score"), 2, new LcdText[2]{LcdText(0, 1, "High Score"), LcdText(0, 1, "Reset")});
LcdMenu menus[nrOfMenus] = {mainMenu, userMenu, scoreMenu};

void setup() {
  lcd.createChar(arrowLeftNum, arrowLeft);
  lcd.createChar(arrowRightNum, arrowRight);
  lcd.begin(lcdColNum, lcdRowNum);
  menus[currentMenu].displayMenu(lcd);
}

void loop() {
  
}
