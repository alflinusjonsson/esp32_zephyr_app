# ESP32 Zephyr WiFi Temperature Monitor

## Overview
A Zephyr RTOS application for ESP32 that reads DS18B20 temperature sensor data and transmits it over WiFi via UDP.

## Features
- DS18B20 temperature sensor integration
- WiFi connectivity with automatic reconnection
- UDP data transmission
- Modular architecture with separate components

## Hardware Requirements
- ESP32 DevKit C
- DS18B20 temperature sensor
- 4.7kΩ pull-up resistor

## Wiring
```
DS18B20 Data Pin -> GPIO 25
DS18B20 VCC     -> 3.3V
DS18B20 GND     -> GND
4.7kΩ resistor between Data and VCC
```

## Building
```bash
west build -b esp32_devkitc/esp32/procpu -- \
  -DCONFIG_WIFI_SAMPLE_SSID="Your_SSID" \
  -DCONFIG_WIFI_SAMPLE_PSK="Your_Password" \
  -DCONFIG_UDP_SERVER_ADDR="Your_Server_IP" \
  -DCONFIG_UDP_SERVER_PORT=Your_Server_Port
```

## Configuration
Edit `prj.conf` to adjust logging levels and other settings.