## Wiring Connections

### MFRC522 RFID Reader
| Arduino Pin | MFRC522 Pin | Description |
| :--- | :--- | :--- |
| **3.3V** | VCC | **Power (Do not use 5V)** |
| **GND** | GND | Ground |
| **D10** | SDA (SS) | SPI Slave Select |
| **D9** | RST | Reset |
| **D11** | MOSI | SPI Master Out Slave In |
| **D12** | MISO | SPI Master In Slave Out |
| **D13** | SCK | SPI Serial Clock |
| *NC* | IRQ | Not Connected |

---

### 16x2 I2C LCD Display
| Arduino Pin | LCD Pin | Description |
| :--- | :--- | :--- |
| **5V** | VCC | Power |
| **GND** | GND | Ground |
| **A4** | SDA | I2C Data |
| **A5** | SCL | I2C Clock |

---

### 4x4 Matrix Keypad
| Arduino Pin | Keypad Pin | Description |
| :--- | :--- | :--- |
| **D0** | Row 1 | Matrix Row 1 |
| **A0 (D14)** | Row 2 | Matrix Row 2 |
| **D2** | Row 3 | Matrix Row 3 |
| **D3** | Row 4 | Matrix Row 4 |
| **D4** | Col 1 | Matrix Column 1 |
| **D5** | Col 2 | Matrix Column 2 |
| **D6** | Col 3 | Matrix Column 3 |
| **D7** | Col 4 | Matrix Column 4 |

---

### Servo & Buzzer
| Component | Arduino Pin | Wire Color / Terminal |
| :--- | :--- | :--- |
| **Servo** | **5V** | Red (VCC) |
| **Servo** | **GND** | Black/Brown (GND) |
| **Servo** | **A2 (D16)** | Orange/Yellow (Signal) |
| **Buzzer** | **A1 (D15)** | Positive (+) |
| **Buzzer** | **GND** | Negative (-) |

> **Note:** For the buzzer, it is recommended to add a **220Ω resistor** in series if the volume is too high or if you are using a sensitive passive piezo.
