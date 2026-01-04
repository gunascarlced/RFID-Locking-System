#include <MFRC522.h> //rfid
#include <MFRC522Extended.h> //rfid
#include <deprecated.h>
#include <require_cpp11.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h> //lcd
#include <Keypad.h> // keypad
#include <Servo.h> // servo

#define RST_PIN  9
#define SS_PIN   10
#define BUZZER_PIN 15
#define SERVO_PIN 16

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

Servo myServo; // servo motor

// Add UIDs and unique welcome messages
struct UIDMessage {
    String uid;
    String message;
};

UIDMessage uidMessages[] = {
    {"40:E:D2:55", "Ola Papa Jirimih"},
    {"f3:1d:df:c1", "Hai Pian ^-^"},
    {"a3:35:32:c2", "Hi Val ❤"},
    {"13:c6:de:c1", "Hi Gigi!"},
    {"23:7f:dc:c1", "Konnichiwa Kai!"} // Add ("Card UID", "Message") to add more access
};

String passcode = "69"; // passcode
String masterPassword = "1234"; //code to change pass

const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols] = {
    {'1', '2', '3', 'D'},
    {'4', '5', '6', 'C'},
    {'7', '8', '9', 'B'},
    {'*', '0', '#', 'A'}
};

byte rowPins[numRows] = {0, 14, 2, 3};
byte colPins[numCols] = {4, 5, 6, 7};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

const unsigned long timeoutDuration = 5000; // Duration before resetting
unsigned long lastInputTime; // Last time input was received for resetting

void setup() {
    Serial.begin(9600);
    pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
    myServo.attach(SERVO_PIN); // Attach the servo to the digital pin 16
    myServo.write(180); // Set initial position to 180 (access denied/close lock)
    
    while (!Serial);
    Serial.println(F("\n[START] Initializing RFID Reader..."));

    SPI.begin();
    rfid.PCD_Init();
    delay(1000);

    byte version = rfid.PCD_ReadRegister(rfid.VersionReg);
    if (version == 0x00 || version == 0xFF) {
        Serial.println(F("[ERROR] RFID reader not found. Check wiring and power."));
        while (true); // notify if reader is not found
    }

    lastInputTime = millis();
    Serial.print(F("[OK] RFID firmware version: 0x"));
    Serial.println(version, HEX);
    Serial.println(F("[READY] Place an RFID card near the reader..."));
    // proceed to the main code
    lcd.begin();
    lcd.clear();
    lcd.backlight();
    resetToScan(); // start to scan state
}

// Scan State
void resetToScan() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello Alexnatics");
    lcd.setCursor(2, 1);
    lcd.print("Scan A Card");
    lastInputTime = millis(); // Timer before reset
}

// Check for master card
bool isMasterCard(const String& uid) {
    return uid.equalsIgnoreCase("B3:55:F5:C8"); // master card UID
}

// Master mode state
void handleMasterCard(const String& uid) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Master Mode");
    lcd.setCursor(0, 1);
    lcd.print("D: Grant  A: Pass");

    unsigned long startTime = millis();
    while (millis() - startTime < timeoutDuration) {
        char key = myKeypad.getKey();
        if (key) {
            if (key == 'D') { // 'D' key for grant access
                grantAccess(uid);
                return;
            }
            if (key == 'A') { // 'A' key for change passcode
                changePasscode();
                return;
            }
        }
    }
    resetToScan(); // Timeout, reset to scan
}

// Change pass state
void changePasscode() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Master ");
    lcd.setCursor(0, 1);
    lcd.print("Password: ");
    String inputMasterPassword = "";
    char key;
    unsigned long startTime = millis();

    // Step 1: Enter Master Password
    while (millis() - startTime < timeoutDuration) {
        key = myKeypad.getKey();
        if (key) {
            startTime = millis(); // Reset timeout on input
            if (key == '#') {
                if (inputMasterPassword.equals(masterPassword)) {
                    // Step 2: Enter New Passcode
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Enter New Pass:");
                    String newPasscode = "";
                    unsigned long startTime2 = millis();
                    while (millis() - startTime2 < timeoutDuration) {
                        key = myKeypad.getKey();
                        if (key) {
                            startTime2 = millis(); // Reset timeout on input
                            if (key == '#') {
                                if (newPasscode.length() > 0) {
                                    passcode = newPasscode; // Update the passcode
                                    lcd.clear();
                                    lcd.setCursor(0, 0);
                                    lcd.print("Passcode Changed"); //password change notification
                                    delay(2000);
                                } else {
                                    lcd.clear();
                                    lcd.setCursor(0, 0);
                                    lcd.print("Invalid Passcode"); //if no new password is detected
                                    delay(2000);
                                }
                                resetToScan(); // Reset back to scan state
                                return;
                            } else if (key == 'C') { // 'C' key to delete/backspace
                                if (newPasscode.length() > 0) {
                                    newPasscode.remove(newPasscode.length() - 1); // Remove last character
                                    lcd.setCursor(0, 1);
                                    lcd.print("                "); // clear the rest of the line
                                    lcd.setCursor(0, 1);
                                    lcd.print(generateAsterisks(newPasscode.length())); // print asterisk instead of the actual number
                                }
                            } else {
                                newPasscode += key;
                                lcd.setCursor(0, 1);
                                lcd.print("                "); // Clear the rest of the line
                                lcd.setCursor(0, 1);
                                lcd.print(generateAsterisks(newPasscode.length())); // print asterisk instead of the actual number
                            }
                        }
                    }
                    resetToScan(); // Timeout in new passcode entry
                    return;
                } else {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Access Denied");
                    delay(2000);
                    resetToScan(); // Reset back to scan state
                    return;
                }
            } else if (key == 'C') { // 'C' key to delete/backspace
                if (inputMasterPassword.length() > 0) {
                    inputMasterPassword.remove(inputMasterPassword.length() - 1); // Backspace
                    lcd.setCursor(10, 1);
                    lcd.print("                "); // Clear the rest of the line
                    lcd.setCursor(10, 1);
                    lcd.print(generateAsterisks(inputMasterPassword.length())); // print asterisk instead of the actual number pressed
                }
            } else {
                inputMasterPassword += key;
                lcd.setCursor(10, 1);
                lcd.print("                "); // Clear the rest of the line
                lcd.setCursor(10, 1);
                lcd.print(generateAsterisks(inputMasterPassword.length())); // print asterisks instead of actual number pressed
            }
        }
    }
    resetToScan(); // Timeout in master password entry
}

// scanning UID of cards state
String scanUID() {
    String uid = "";
    unsigned long startTime = millis();
    while (millis() - startTime < timeoutDuration) {
        if (!rfid.PICC_IsNewCardPresent()) {
            delay(100);
            continue;
        }
        if (!rfid.PICC_ReadCardSerial()) {
            Serial.println("Failed to read card."); // unable to read the card uid, rescan
            delay(100);
            continue;
        }

        for (byte i = 0; i < rfid.uid.size; i++) {
            if (i > 0) uid += ":"; //print uid with ":" in between
            uid += String(rfid.uid.uidByte[i], HEX);
        }
        Serial.print("UID scanned: ");
        Serial.println(uid); // print uid in serial monitor
        return uid; // return the scanned UID
    }
    return ""; // Timeout, return empty
}

// Grant access state
void grantAccess(const String& uid) {
    tone(BUZZER_PIN, 440, 150);
    delay(150);
    tone(BUZZER_PIN, 523, 150);
    delay(150);
    tone(BUZZER_PIN, 659, 150);
    delay(150); // play the access granted sound
    myServo.write(0); // Set servo to 0 degrees (access granted / unlock door)
    delay(5000); // Auto-close after 5 seconds
    myServo.write(180); // Close the door
}

// Function to deny access and activate the servo
void denyAccess() {
    tone(BUZZER_PIN, 500, 500); 
    delay(500); // Play access denied sound
    myServo.write(180); // Set servo to 180 degrees (access denied / lock door)
}

// check uid validity state
bool checkUID(const String& uid) {
    for (int i = 0; i < sizeof(uidMessages) / sizeof(uidMessages[0]); i++) {
        if (uid.equalsIgnoreCase(uidMessages[i].uid)) {
            return true; // UID found
        }
    }
    return false; // UID not found
}

// input password state
void inputPassword(const String& uid) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password:");
    String inputPassword = "";
    char key;
    unsigned long startTime = millis();

    while (millis() - startTime < timeoutDuration) {
        key = myKeypad.getKey();
        if (key) {
            startTime = millis(); // Reset timeout on input
            if (key == '#') {
                if (inputPassword.equals(passcode)) {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Access Granted");
                    displayWelcomeMessage(uid); // Display unique message after access granted
                    grantAccess(uid); // start the grant access state
                } else {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Access Denied"); // wrong password
                    delay(2000);
                    denyAccess();
                }
                resetToScan(); // Reset to scan state
                return;
            } else if (key == 'C') { // 'C' key to delete/backspace
                if (inputPassword.length() > 0) {
                    inputPassword.remove(inputPassword.length() - 1); // Remove last character
                    lcd.setCursor(0, 1);
                    lcd.print("                "); // Clear the line
                    lcd.setCursor(0, 1);
                    lcd.print(generateAsterisks(inputPassword.length())); // Display asterisks instead of actual number
                }
            } else {
                inputPassword += key;
                lcd.setCursor(0, 1);
                lcd.print("                "); // Clear the line
                lcd.setCursor(0, 1);
                lcd.print(generateAsterisks(inputPassword.length())); // Display asterisks instead of actual number
            }
        }
    }
    resetToScan(); // Timeout, reset to scan
}

// welcome message state print to lcd
void displayWelcomeMessage(const String& uid) {
    for (int i = 0; i < sizeof(uidMessages) / sizeof(uidMessages[0]); i++) {
        if (uid.equalsIgnoreCase(uidMessages[i].uid)) {
            lcd.setCursor(0, 1);
            lcd.print("                "); // Clear the line
            lcd.setCursor(0, 1);
            lcd.print(uidMessages[i].message); // Display the unique message
            delay(2000); // Display the message for 2 seconds before going back to scan state
            return;
        }
    }
}

// generate asterisk instead of number state
String generateAsterisks(int length) {
    String asterisks = "";
    for (int i = 0; i < length; i++) {
        asterisks += '*';
    }
    return asterisks; // Return the asterisks string
}

void loop() {
    if (millis() - lastInputTime > timeoutDuration) {
        resetToScan(); // check last input before going back to scan state
    }

    String uidString = scanUID(); // Scan the UID
    if (uidString != "") { // Only proceed if a UID was scanned
        lastInputTime = millis(); // Update last input time
        if (isMasterCard(uidString)) {
            handleMasterCard(uidString); // master card uid
        } else {
            if (checkUID(uidString)) {
                inputPassword(uidString); // if registered uid proceed to input password
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Access Denied");
                lcd.setCursor(0, 1);
                lcd.print("Try Again");
                denyAccess(); // Deny access if UID is not in the list
            }
        }
    }

    rfid.PICC_HaltA(); // Halt the PICC
    rfid.PCD_StopCrypto1(); // Stop encryption
}
