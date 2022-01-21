#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9


Servo myservo;

int green_led = 4;
int red_led = 5;

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  myservo.attach(6);
  myservo.write(0);

  Serial.println("waiting for card...");
}
void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check if the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
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
  //Serial.print("Tap card key: ");
  //Serial.println(strID);
  delay(500);

  if (strID.indexOf("73:5F:AA:09") >= 0) {  //put keycard tag;
   
    
    Serial.println("**Authorised acces**");
   
    digitalWrite(green_led, HIGH);
    myservo.write(90);
    delay(500);
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, LOW);
  }
  else 
  {
   
    Serial.println("**Acces denied**");
    digitalWrite(red_led, HIGH);
    delay(500);
    digitalWrite(green_led, LOW);
    myservo.write(0);
    digitalWrite(red_led, LOW);
  }
}
