# RFID-Locking-System

A dual-authentication security system using an MFRC522 RFID reader and a 4x4 Keypad to control a Servo-based locking mechanism.

## 📖 Project Story
*This system was developed as a final project for our 12th-grade Advanced Arduino Coding course. To bring the code to life, we integrated the electronics into a small-scale physical house model, effectively demonstrating how hardware and software work together to provide real-world home security solutions.*

---

## ✨ Features
* **Dual Authentication:** Scan a registered UID, then enter a passcode.
* **Master Mode:** Use a Master Card to change the system passcode without re-uploading code.
* **Custom Greetings:** Personalized LCD messages for different users (e.g., "Ola Papa Jirimih").
* **Timeout System:** Automatically resets to "Scan" mode after 5 seconds of inactivity.

> [!IMPORTANT]
> Before uploading the code to your Arduino, ensure you update the following variables:
> * **`masterPassword`**: Change the default `"1234"` to a unique 4-digit code.
> * **`uidMessages`**: Replace the example UID hex codes with the specific IDs of your own RFID tags/cards. If you don't know your UIDs, use an RFID scanner sketch first to identify them.

---

## 🛠 Hardware Requirements

### Core Electronic Components
These are the main parts directly referenced in the code and circuit diagram.

| Component | Quantity | Purpose |
| :--- | :---: | :--- |
| **Arduino Uno** | 1 | The "brain" of the system; handles all logic and I/O. |
| **MFRC522 RFID Module** | 1 | Scans UIDs for access control (**Note: Requires 3.3V**). |
| **16x2 I2C LCD Display** | 1 | Displays system status and user feedback. |
| **4x4 Matrix Keypad** | 1 | Numeric input for passwords and menu navigation. |
| **Servo Motor (SG90)** | 1 | Physical locking mechanism. |
| **Passive Buzzer** | 1 | Audio feedback for access granted or denied. |



---

### Wiring & Connectivity
* **Jumper Wires (M-M & M-F):** For connecting Arduino pins to components.
* **Breadboard:** For prototyping and organizing power rails.
* **Resistor (220Ω):** For current limiting (Buzzer/LCD protection).
* **RFID Cards/Tags:** 13.56MHz compatible tags for testing access.
* **USB Cable (Type A to B):** For programming and power.

---

## 📦 Required Libraries
Ensure these libraries are installed in your environment to compile the code:

* **MFRC522** (by GithubCommunity)
* **LiquidCrystal_I2C** (by Frank de Brabander)
* **Keypad** (by Mark Stanley, Alexander Brevig)
* **Servo** (Built-in)

---

## 💻 Software & Tools
* **VS Code:** Primary editor for code development.
* **Arduino Extension for VS Code:** Recommended for compiling and uploading directly from the editor.

---

## 👥 Credits & Recognition

### Development Team
* **Adrian Peñano**
* **Aein Ortigosa**
* **Aldrin Dela Peña**
* **Carl Cedrick Gunas**
* **Dave Ashle Escuadro**
* **Diandrei Orejas**
* **Franz Leigh Hilario**
* **James Robert Atengco**
* **Jessielyn Suson**
* **Johmel Respicio**
* **Jon Reymon Agrimano**
* **Juri Jabonete**
* **Katrina Michelle Gabriel**
* **Kiezer Peñalber**
* **Mark Angelo Pombo**
* **Mary Jane Marpuri**
* **mary Navales**
* **Maxine Laguinlin**
* **Price Charles Giron**
* **Aein Ortigosa**
* **Princess Phean Batulat**
* **Ron Vivo**
* **Vince Ryan Alpasar**
* **Yra Mae Sison**

### Project Guidance
* **Adviser:** Engr. Alexander Gerimi Basilio
