#include <Arduino.h>
#include <Wire.h>

// Pin mapping from the schematic:
// USER_LED => GPIO9
// LED DI (WS2812 data) => GPIO10
// I2C0_SDA => GPIO2, I2C0_SCL => GPIO3 (Wire.begin() uses default SDA/SCL)

const int USER_LED_PIN = 9;
const int LED_DATA_PIN = 10;

// CryptoAuthLib (Microchip) headers
#include <cryptoauthlib.h>

void printHex(const uint8_t* data, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    if (data[i] < 0x10) Serial.print('0');
    Serial.print(data[i], HEX);
    if (i + 1 < len) Serial.print(':');
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Diva Card example firmware (Arduino RP2040)");

  // Initialize user LED pin
  pinMode(USER_LED_PIN, OUTPUT);
  digitalWrite(USER_LED_PIN, LOW);

  // Initialize LED data pin (for external WS2812 chain if used)
  pinMode(LED_DATA_PIN, OUTPUT);
  digitalWrite(LED_DATA_PIN, LOW);

  // Initialize I2C bus (Wire uses default SDA/SCL pins mapped in board)
  Wire.begin();
  Serial.println("I2C initialized (Wire.begin)");

  // Initialize CryptoAuthLib to talk to ATECC608A over I2C
  ATCA_STATUS status = ATCA_GEN_FAIL;
  Serial.print("Initializing CryptoAuthLib... ");
  // Use the default I2C interface cfg built into the library (cfg_ateccx08a_i2c_default)
  status = atcab_init(&cfg_ateccx08a_i2c_default);
  if (status != ATCA_SUCCESS) {
    Serial.print("failed (status=0x"); Serial.print(status, HEX); Serial.println(")");
  } else {
    Serial.println("ok");

    // Wake the device (library provides helper)
    status = atcab_wakeup(NULL);
    if (status != ATCA_SUCCESS) {
      Serial.print("ATECC wake failed (0x"); Serial.print(status, HEX); Serial.println(")");
    } else {
      Serial.println("ATECC woke up");

      // Read the 9-byte serial number
      uint8_t serial_num[9] = {0};
      status = atcab_read_serial_number(serial_num);
      if (status == ATCA_SUCCESS) {
        Serial.print("ATECC serial: ");
        printHex(serial_num, sizeof(serial_num));
        Serial.println();
      } else {
        Serial.print("Failed to read serial (0x"); Serial.print(status, HEX); Serial.println(")");
      }

      // Put the device back to sleep
      atcab_sleep();
    }
  }
}

unsigned long lastToggle = 0;
bool ledState = false;

void loop() {
  unsigned long now = millis();
  if (now - lastToggle > 500) {
    ledState = !ledState;
    digitalWrite(USER_LED_PIN, ledState ? HIGH : LOW);
    lastToggle = now;
  }
}
