#include <SPI.h>
#include <Wire.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Tone.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad
String C4Status = "Not Armed";
Tone buzzer;
float C4Timer = 15;
int CodeTime = 2;
int digitSelected = 0;
char password[4] = {0, 0, 0, 0};
char passwordIn[4];
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));
  pinMode(12, OUTPUT);

  buzzer.begin(13);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  for (int i = 0; i <= 3; i++) {
    password[i] = char(random(48, 58));
  }
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(25,25);
    display.println("Booting...");
  display.display();
  buzzer.play(NOTE_B4, 250); delay(500); buzzer.play(NOTE_C3, 250); delay(500); buzzer.play(NOTE_C3, 100); delay(40); buzzer.play(NOTE_B4, 50); delay(100); buzzer.play(NOTE_B6, 50); delay(100); buzzer.play(NOTE_B7, 50);

  display.clearDisplay();
  display.display();
  setupScreen();
  homeScreen();

}
/*

   NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7



                                                     <<<< text stuff here
      display.clearDisplay();
   display.setTextColor(WHITE);
   display.setTextSize(5);
   display.setCursor(0,0);
   display.println("Text Here");
   display.display();*/

//drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void loop() {
  char key = keypad.getKey();
  if (key)
  {
    TypingPassword(key);

  }
}




void TypingPassword(char Key) {                                                                                                   //bombLogic

  if (Key != 'A' && Key != 'B' && Key != 'C' && Key != 'D' && Key != '#' && Key != '*') {
    buzzer.play(NOTE_C3, 50);
    passwordIn[digitSelected] = Key;
    Serial.println(passwordIn[digitSelected]);
    digitSelected++;

    homeScreen();
  }
  else {
    if (Key == 'C') {  //c for code
      hackScreen();
    }
    if (Key == 'D') { //d for detonate i guess
      //bomb has been planted BEEP!
      if (C4Status == "Armed") {
        C4Status = "timer";
        C4timer();
        Serial.println("beep?");
      }                                                  // do stuff here                                   <<<<<<<<<<<<<<<<<<<<<<<<<<<

    }
  }

  if (digitSelected == 4) {
    if (passwordIn[0] == password[0] && passwordIn[1] == password[1] && passwordIn[2] == password[2] && passwordIn[3] == password[3]) {
      passwordIn[0] = ' ';
      passwordIn[1] = ' ';
      passwordIn[2] = ' ';
      passwordIn[3] = ' ';
      digitSelected = 0;
      delay(100);
      buzzer.play(NOTE_C7, 150);
      if (C4Status == "Not Armed") {
        C4Status = "Armed";
      }
      else if (C4Status == "timer") {
        C4Status = "CT Win";
        
      }
      else {
        C4Status = "Reset C4";

      }
      buzzer.play(NOTE_C7, 150);
      homeScreen();
      if (C4Status == "CT Win"){
      delay(40); buzzer.play(NOTE_B4, 50); delay(100); buzzer.play(NOTE_B6, 50); delay(100); buzzer.play(NOTE_B7, 50);
      }

    }
    else {
      passwordIn[0] = ' ';
      passwordIn[1] = ' ';
      passwordIn[2] = ' ';
      passwordIn[3] = ' ';
      digitSelected = 0;
      homeScreen();
      buzzer.play(NOTE_C4, 500);
    }
  }
}

void setupScreen() {                                                                                          //setup
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Select a Time");
  display.println("");
  display.println("A. 30 Seconds");
  display.println("B. 60 Seconds");
  display.println("C. 90 Secounds");
  display.println("D. 120 Seconds");
  display.display();
  char key = keypad.getKey();
  while (key != 'A' && key != 'B' && key != 'C' && key != 'D') {
    key = keypad.getKey();
  }
  if (key == 'A') {
    C4Timer = 30;
    CodeTime = 5;
  }
  else if (key == 'B') {
    C4Timer = 60;
    CodeTime = 10;
  }
  else if (key == 'C') {
    C4Timer = 90;
    CodeTime = 15;
  }
  else if (key == 'D') {
    C4Timer = 120;
    CodeTime = 20;
  }
  else {
    C4Timer = 60;
    CodeTime = 10;
  }
delay(40); buzzer.play(NOTE_B4, 50); delay(100); buzzer.play(NOTE_B6, 50); delay(100); buzzer.play(NOTE_B7, 50);
}
void homeScreen() {                                                                                                       //home

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 0);
  if (C4Status != "timer") {
    display.println(C4Status);
  }
  else {
    display.println(C4Timer);
  }
  display.setCursor(20, 36);
  display.print(passwordIn[0]);
  display.print(" ");
  display.print(passwordIn[1]);
  display.print(" ");
  display.print(passwordIn[2]);
  display.print(" ");
  display.print(passwordIn[3]);
  display.setCursor(20, 40);
  display.print("_ _ _ _");
  display.display();
}

void hackScreen() {
  display.clearDisplay();


  display.setTextSize(2);
  display.setCursor(10, 0);
  display.println(C4Status);
  display.display();
  for (int i = 0; i <= CodeTime; i += 1) {
    display.fillRoundRect(0, 32, i * (display.width() / CodeTime), 50, 2, WHITE);
    buzzer.play(NOTE_C7, 150);
    display.display();
    delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.println(C4Status);
  display.setCursor(20, 36);
  display.print(password[0]);
  display.print(" ");
  display.print(password[1]);
  display.print(" ");
  display.print(password[2]);
  display.print(" ");
  display.print(password[3]);
  display.setCursor(20, 40);
  display.print("_ _ _ _");
  display.display();
  delay((CodeTime * 1000) / 2);
  if (C4Status == "timer") {
    C4Timer -= CodeTime;
  }
  homeScreen();
}

void C4timer() {

  Serial.println(C4Status);


  for (C4Timer; C4Timer >= 0; C4Timer -= 0.05) {
    
   C4countdownBeep();
    char key = keypad.getKey();
    if (key)
    {
      TypingPassword(key);

    }
    if (C4Status == "CT Win") {
      break;
      
    }
    homeScreen();
    Serial.println(C4Timer);
    delay(5);
    
  }
  if (C4Status == "timer"){
  C4Status = "T Win";
  homeScreen();
  delay(40); buzzer.play(NOTE_B4, 50); delay(100); buzzer.play(NOTE_B6, 50); delay(100); buzzer.play(NOTE_B7, 50);
  }
}

void C4countdownBeep(){
  
  
  }

