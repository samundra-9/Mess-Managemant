#include <SPI.h>          // Required for MFRC522 RFID module
#include <MFRC522.h>        // RFID RC522 library
#include <LiquidCrystal.h>  // LCD library
#include <Servo.h>          // Servo motor library

// --- Define LCD Pin Connections ---
// LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

// --- Define Servo Pin Connection ---
Servo myServo;              // Create a servo object to control a servo
const int servoPin = 6;     // Digital pin for the servo control signal

// --- Define RFID RC522 Pin Connections ---
#define SS_PIN 10           // SDA (SS) pin for RC522 (Slave Select)
#define RST_PIN 9           // RST pin for RC522 (Reset)

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// --- Define the authorized UID ---
// The UID "53 4D 95 DD" as a byte array
// Make sure to match the format of your RFID tags (e.g., 4 bytes for MIFARE Classic)
const byte authorizedUID[] = {0x53, 0x4D, 0x95, 0xDD};
const int authorizedUID_size = sizeof(authorizedUID);

void setup() {
  Serial.begin(9600);     // Initialize serial communication for debugging
  while (!Serial);        // Wait for serial port to connect (for Leonardo/Micro)

  // --- Initialize LCD ---
  lcd.begin(16, 2);       // Set up the LCD's number of columns and rows
  lcd.print("   Welcome!   ");
  lcd.setCursor(0, 1);
  lcd.print("RFID Access");
  delay(2000); // Display welcome message for 2 seconds

  // --- Initialize Servo ---
  myServo.attach(servoPin); // Attaches the servo on the defined pin to the servo object
  myServo.write(0);         // Set servo to initial position (0 degrees)

  // --- Initialize SPI bus for RFID ---
  SPI.begin();            // Start SPI communication

  // --- Initialize MFRC522 RFID module ---
  mfrc522.PCD_Init();
  Serial.println("RFID Reader Initialized");
  Serial.println("Ready to scan...");
  lcd.clear();
  lcd.print("Scan RFID Card");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
}

void loop() {
  // --- RFID Card Detection Logic ---
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // No new card, exit loop early
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // No card selected, exit loop early
  }

  // --- Read Scanned Card's UID and convert to String ---
  String scannedUID_String = "";
  Serial.print("Scanned UID :");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    scannedUID_String += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    scannedUID_String += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // --- Validate the Scanned UID ---
  bool accessGranted = true;
  if (mfrc522.uid.size != authorizedUID_size) {
    accessGranted = false; // UID size doesn't match
  } else {
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] != authorizedUID[i]) {
        accessGranted = false; // Mismatch in UID bytes
        break;
      }
    }
  }

  // --- Display Result on LCD and Control Servo ---
  lcd.clear();
  lcd.setCursor(0, 0);

  if (accessGranted) {
    lcd.print("ACCESS GRANTED!");
    Serial.println("Access Granted!");
    lcd.setCursor(0, 1);
    lcd.print(scannedUID_String.substring(0, min((int)scannedUID_String.length(), 16)));

    // --- Control Servo (Open/Close action) ---
    Serial.println("Moving servo for access...");
    for (int pos = 0; pos <= 180; pos += 1) { // Open
      myServo.write(pos);
      delay(10);
    }
    delay(1000); // Hold open

    for (int pos = 180; pos >= 0; pos -= 1) { // Close
      myServo.write(pos);
      delay(10);
    }
    Serial.println("Servo action complete.");

  } else {
    lcd.print("ACCESS DENIED!");
    Serial.println("Access Denied!");
    lcd.setCursor(0, 1);
    lcd.print(scannedUID_String.substring(0, min((int)scannedUID_String.length(), 16))); // Show the scanned UID
    // You could optionally move the servo to a 'denied' position or not move it at all
    // For now, it stays at 0 degrees (default closed position).
  }

  // --- Halt PICC (the card) and stop encryption on PCD (the reader) ---
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  // --- Prepare LCD for next scan ---
  delay(3000); // Keep result visible for 3 seconds
  lcd.clear();
  lcd.print("Scan RFID Card");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
}
