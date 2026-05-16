# IoT Smart Irrigation System using ESP8266 and Blynk

## Overview

This project is an IoT-based smart irrigation system that automatically waters plants when soil becomes dry. The system uses an ESP8266 NodeMCU, soil moisture sensor, relay module, water pump, and Blynk IoT platform.

## Features

- Real-time soil moisture monitoring
- Automatic irrigation
- Manual pump control using Blynk
- Smart watering logic
- WiFi-enabled monitoring
- Remote access using mobile dashboard

## Tech Stack

- ESP8266 NodeMCU
- Arduino IDE
- Blynk IoT
- C++
- Soil Moisture Sensor
- Relay Module
- Water Pump


## Installation

1. Install Arduino IDE
2. Install ESP8266 board package
3. Install Blynk library
4. Connect ESP8266
5. Upload code
6. Configure Blynk datastreams
7. Power the system

## Configuration

Before uploading the code:

Replace these values:

```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";
```

### Relay to ESP8266

| ESP8266 | Relay |
|---|---|
| GND | GND |
| 3V | VCC |
| D2 | IN2 |

### Soil Sensor

| Sensor | ESP8266 |
|---|---|
| VCC | 3V |
| GND | GND |
| AO | A0 |

## Working Logic

- Soil moisture is checked every second
- If soil is dry:
  - Pump turns ON for 3 seconds
  - System waits 40 seconds
  - Moisture checked again
- User can manually control pump via Blynk

## Future Potential Improvements

- Solar power support
- Weather integration
- Water tank level sensor
- Better battery solution

## Author

Taha  
Computer Science Engineering Student
