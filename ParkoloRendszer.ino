
#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Vector.h>

#define SS_PIN 10
#define RST_PIN 9


Servo myservo;

int green_led = 4;
int red_led = 5;

int blueButtonValue = 0;
int redButtonValue = 0;
int blueButton = 2;
int redButton = 3;

Vector<String> tag;

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
  pinMode(blueButton, INPUT);
  pinMode(redButton, INPUT);
  
  myservo.attach(6);
  myservo.write(0);
}
void loop() {  
  /*MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check if the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }*/

  blueButtonValue = digitalRead(blueButton);
  redButtonValue = digitalRead(redButton);

  String strID = "";

  for (byte i = 0; i < 4; i++)
  {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i != 3 ? ":" : "");
  }
  
  if (blueButtonValue == HIGH)
  {
    lcd.clear();
    lcd.print("Jaj");
    Serial.print(strID);
    for (int i = 0; i < 1; i++)
    {
      if (tag[i] == strID)
      {
        lcd.clear();
        lcd.print("A kartya mar regisztralva van!");
      }
      else
      {
        tag.push_back(strID);
        lcd.clear();
        lcd.print("Sikeres regisztracio");
        digitalWrite(green_led, HIGH);
      }
    }
    Serial.print(tag.size());
    delay(10000);

    digitalWrite(green_led, LOW);
  }

  lcd.clear();
  lcd.print("Kerem a kartyat");
  delay(500);  
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
  {
    return;
  }

 if (strID.indexOf("73:5F:AA:09") >= 0) {  //put keycard tag;
   
    
    Serial.println("**Authorised acces**");

    lcd.clear();
    lcd.print("Kartya elfogadva");
   
    digitalWrite(green_led, HIGH);
    myservo.write(90);
    delay(500);
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, LOW);
    delay(1000);
  }
  else 
  {   
    Serial.println("**Acces denied**");

    lcd.clear();
    lcd.print("Kartya elutasitva");
    
    digitalWrite(red_led, HIGH);
    delay(500);
    digitalWrite(green_led, LOW);
    myservo.write(0);
    digitalWrite(red_led, LOW);
    delay(1000);
  }
}
