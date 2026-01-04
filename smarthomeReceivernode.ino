/*
  MKR1010 Receiver Node - LCD Status Screen + UART Listener
  ---------------------------------------------------------
  UART:
    Receives data on Serial1 (from Nano TX -> MKR RX1).
*/

#include <LiquidCrystal.h>

const int LCD_RS = 2;
const int LCD_E  = 3;
const int LCD_D4 = 4;
const int LCD_D5 = 5;
const int LCD_D6 = 6;
const int LCD_D7 = 7;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  // USB serial to laptop
  Serial.begin(115200);
  while (!Serial) {}

  // UART from sensor node (Nano TX -> MKR RX1)
  Serial1.begin(9600);

  // LCD init
  lcd.begin(16, 2);
  lcd.clear();

  // Startup screen 
  lcd.setCursor(0, 0);
  lcd.print("Receiver Node");
  lcd.setCursor(0, 1);
  lcd.print("Waiting for data");

  Serial.println("Receiver Node running - waiting for UART data...");
}

void loop() {
  while (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);
  }

  
}
