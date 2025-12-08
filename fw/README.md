# Diva Card example firmware (Arduino RP2040)

This folder contains a simple example firmware for the Diva Card RP2040-based board. It demonstrates:

- USB-Serial logging at 115200
- Initializing and toggling the user LED (GPIO9)
- Initializing I2C (Wire) and scanning for the Microchip ATECC608A at address `0x60`
- A placeholder pin for WS2812 data (`GPIO10`).

Build with PlatformIO (recommended for setting custom VID/PID):

1. Install PlatformIO extension for VS Code or use `pio` CLI.
2. Edit `platformio.ini` to set your desired USB VID/PID:

   - `board_build.usb_vendor_id = 0x1234`
   - `board_build.usb_product_id = 0xABCD`

   Replace the hex values with your vendor/product IDs.

3. Build and upload:

```powershell
pio run -e raspberrypi_pico -t upload
pio device monitor -e raspberrypi_pico
```

Notes:
- The board schematic maps `I2C0_SDA` to `GPIO2` and `I2C0_SCL` to `GPIO3` — `Wire.begin()` uses those defaults.
- The example initializes the I2C bus and then uses Microchip's `cryptoauthlib` to talk to the ATECC608A. The sketch calls `atcab_init`, `atcab_wakeup`, `atcab_read_serial_number`, and `atcab_sleep` (the library is fetched by PlatformIO via `platformio.ini`).
- If you prefer the Arduino IDE / Arduino CLI, building with a custom VID/PID is platform/core dependent; PlatformIO makes it easy via `platformio.ini`.
