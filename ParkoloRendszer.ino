#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define SS_PIN 10
#define RST_PIN 9


Servo myservo;

int green_led = 4;
int red_led = 5;

int trigPin = 7;
int echoPin = 8;
int maxDistance = 20;

const String regiszteredCard = "73:5F:AA:09";

NewPing sonar(trigPin, echoPin, maxDistance);

LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  lcd.begin();
  lcd.backlight();
  
  rfid.PCD_Init();
  
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  
  myservo.attach(6);
  myservo.write(0);
}

void loop() {
  lcd.clear();
  if (sonar.ping_cm() < 20 && sonar.ping_cm() > 0)
  {
    lcd.clear();
    lcd.print("Kerem a kartyat");
    delay(500);

    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
      return;
    }
  
    String strID = "";
    for (byte i = 0; i < 4; i++) {
      strID +=
        (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
        String(rfid.uid.uidByte[i], HEX) +
        (i != 3 ? ":" : "");
    }
    strID.toUpperCase();
    delay(500);
  
    if (strID.indexOf(regiszteredCard) >= 0)
    {
      lcd.clear();
      lcd.print("Kartya elfogadva");
      Serial.println("**Authorised acces**");
      digitalWrite(green_led, HIGH);
      myservo.write(90);
      delay(5000);
      digitalWrite(green_led, LOW);
      myservo.write(0);
    }
    else 
    {   
      Serial.println("**Acces denied**");

      lcd.clear();
      lcd.print("Kartya elutasitva");
      digitalWrite(red_led, HIGH);
      delay(5000);
      digitalWrite(red_led, LOW);
    }
  }
}
