#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10  // Define the Slave Select Pin
#define RST_PIN 9  // Define the Reset Pin
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create an MFRC522 instance

#define RED_LED_PIN 2
#define GREEN_LED_PIN 3
#define BUZZER_PIN 4
#define SERVO_PIN 5

Servo myservo;  // Create a servo instance

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  
  mfrc522.PCD_Init();  // Initialize MFRC522 RFID module
  myservo.attach(SERVO_PIN);  // Attach servo to its pin

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Turn off the initial state of LEDs and buzzer
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Move the servo to its default position (0 degrees)
  myservo.write(0);
}

void loop() {
  // Reset LEDs and buzzer to their default states
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Reset the servo to its default position (0 degrees)
  myservo.write(0);

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Print card details
    Serial.print("Card UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Check if the card is the correct one (you need to replace this with your actual card UID)
    if (checkCardUID(mfrc522.uid)) {
      // Correct card detected
      digitalWrite(GREEN_LED_PIN, HIGH);
      tone(BUZZER_PIN, 1000, 500);
      myservo.write(90);  // Rotate servo to 90 degrees
      delay(5000);  // Wait for 5 seconds
      myservo.write(0);  // Reset servo to 0 degrees
    } else {
      // Wrong card detected
      digitalWrite(RED_LED_PIN, HIGH);
      tone(BUZZER_PIN, 2000, 1000);
      delay(1000);  // Wait for 1 second
    }
    
    mfrc522.PICC_HaltA();  // Halt the card
  }
}

bool checkCardUID(MFRC522::Uid uid) {
  // Replace this with the actual UID of the allowed card
  byte allowedUID[] = {0x11, 0x22, 0x33, 0x44};
  
  for (byte i = 0; i < uid.size; i++) {
    if (uid.uidByte[i] != allowedUID[i]) {
      return false;
    }
  }
  return true;
}
