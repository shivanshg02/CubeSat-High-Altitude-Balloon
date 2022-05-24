# CubeSat-High-Altitude-Balloon

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
    - GPS Module (?)
    - UV Sensor (?)
    - IMU (builtin)
    - SD card logger (SPI)
