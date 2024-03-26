#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9
#define SS_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do display

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  lcd.init();
  lcd.backlight();
  rfid.PCD_Init();

  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Bem Vindo ao");
  lcd.setCursor(0, 1);
  lcd.print("FABLAB ");
}

void loop() {
  resetarMFRC522();

  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  } 

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  String cardID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    cardID += String(rfid.uid.uidByte[i], HEX);
  }

  // Verifica se o ID do cartão é conhecido
  if (cardID == "1234") {
    displayWelcomeMessage("Prof. Willian");
  } else if (cardID == "69df61e5") {
    displayWelcomeMessage("Victoria");
  } else if (cardID == "5cdef852") {
    displayWelcomeMessage("Joao Lucas");
  } else if (cardID == "4929ce5") {
    displayWelcomeMessage("Elielson");
  } else if (cardID == "13beb3a8") {
    displayWelcomeMessage("Richard");
  } else {
    displayErrorMessage(cardID);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void displayWelcomeMessage(String name) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bem Vindo (a)");
  lcd.setCursor(0, 1);
  lcd.print(name);
  digitalWrite(7, HIGH);
  delay(1000);             // Tempo que a tranca fica aberta
  digitalWrite(7, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bem Vindo(a) ao");
  lcd.setCursor(0, 1);
  lcd.print("FABLAB ");
}

void displayErrorMessage(String cardID) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cartao nao Cdtdo");
  lcd.setCursor(0, 1);
  lcd.print(cardID);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bem Vindo(a) ao");
  lcd.setCursor(0, 1);
  lcd.print("FABLAB ");
}

void resetarMFRC522() {
  digitalWrite(RST_PIN, LOW);  // Set pin to LOW
  delay(50);                   // Wait for a brief moment
  digitalWrite(RST_PIN, HIGH); // Set pin to HIGH
  delay(50);                   // Wait for a brief moment
  rfid.PCD_Init();
}
