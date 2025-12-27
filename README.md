# AHT10-temperature-humidity-clock

This project measures and displays temperature and humidity using the AHT10 digital sensor and a 1.3" OLED Display. It is battery powered using a TP4056 charging module and an LM2596 buck converter.

## Features

- Temperature measurement using AHT10
- Humidity measurement using AHT10
- Simple, clean 1.3in OLED Display
- Battery powered and rechargeable

## Hardware Used

- Arduino Nano
- AHT10 temperature and humidity sensor (I2C)
- 1.3" OLED Display
- LM2596S DC-DC buck converter module
- TP4056 1A lithium battery charging module
- Li-ion battery (3300mAh or similar)

## Connections

### AHT10 to Arduino Nano

| AHT10 |  Arduino Nano | 1.3in OLED Display |
|-------|---------------|--------------------|
| VCC   | 3.3V or 5V    | 5V                 |
| GND   | GND           | GND                |
| SDA   | A4            | A4                 |
| SCL   | A5            | A5                 |

### Power Section

- Battery → TP4056 charging module  
- TP4056 → LM2596 buck converter  
- LM2596 output → Arduino Nano and Display

## Software

- Arduino IDE
- Libraries:
  - Adafruit AHTX0
  - Wire
  - Display driver library (U8glib)
  - Adafruit GFX

## Future Improvements

- Add real-time clock (RTC) module
- Add data logging to SD card

## License

This project is for learning and hobby use.

