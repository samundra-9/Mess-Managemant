# Mess-Managemant
 # 🔐 RFID Access Control System using Arduino, LCD, and Servo Motor

This project is a simple RFID-based access control system built using an Arduino Uno, MFRC522 RFID module, 16x2 LCD display, and a servo motor. When a valid RFID tag is scanned, the system grants access by displaying a message on the LCD and rotating the servo motor to simulate a door unlocking mechanism.

---

## 📷 Project Overview

- **Microcontroller:** Arduino Uno
- **RFID Module:** MFRC522
- **Display:** 16x2 LCD (parallel interface)
- **Servo Motor:** SG90 or similar
- **Libraries Used:**
  - `SPI.h`
  - `MFRC522.h`
  - `LiquidCrystal.h`
  - `Servo.h`

---

## 🔌 Circuit Diagram

| Component        | Arduino Pin |
|------------------|-------------|
| LCD RS           | D8          |
| LCD EN           | D7          |
| LCD D4           | D5          |
| LCD D5           | D4          |
| LCD D6           | D3          |
| LCD D7           | D2          |
| Servo Signal     | D6          |
| RFID SDA (SS)    | D10         |
| RFID RST         | D9          |
| RFID MOSI        | D11         |
| RFID MISO        | D12         |
| RFID SCK         | D13         |
| 5V, GND          | Connect accordingly for all modules |

> ℹ️ Use a **220Ω resistor** in series with LCD backlight if needed.

---

## ✅ Features

- Display messages on LCD for user interaction.
- Authenticate RFID cards using UID.
- Show UID of scanned tags on Serial Monitor.
- Grant or deny access with visual feedback and servo movement.
- Easy to extend to database verification or IoT integration.

---

## 📄 How It Works

1. On startup, the LCD displays a welcome message.
2. The system waits for a card to be scanned.
3. Once a card is detected:
   - UID is read and shown in Serial Monitor.
   - If the UID matches the authorized UID, access is granted.
   - The servo rotates to simulate door unlocking.
   - LCD displays "Access Granted!" or "Access Denied!" accordingly.
4. Servo resets, and the system waits for the next scan.

---

## 🧾 Authorized UID

```cpp
const byte authorizedUID[] = {0x53, 0x4D, 0x95, 0xDD};
