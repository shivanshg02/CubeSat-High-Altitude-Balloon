# CubeSat-High-Altitude-Balloon

## Libraries
- `Adafruit BME 280 Library`
- `Adafruit LTR 390 Library`

## Payloads

### UNO
- Pi Zero
  - Iridium (Serial)
  - Camera (cable)
  - Arduino UNO
    - GPS Logger (shield)
    - BME280 (I2C) - see `sparkfun_bme_380_adafruit_library.ino` for sample code
    - UV Sensor (?)
    - Thermistor (?)



### DOS
- Pi Zero
  - LORA radio module (SPI) - see `rfm96_receive.py` for sample code
    - Antenna
  - Camera (cable)
  - Arduino Nano BLE
    - BME280 (I2C) - see `sparkfun_bme_380_adafruit_library.ino` for sample code
    - GPS Module (UART)
    - UV Sensor (I2C)
    - IMU (builtin)
    - SD card logger (SPI)
