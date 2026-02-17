# BookNook Ambient Lighting

A Matter/HomeKit-enabled ambient lighting controller for a decorative BookNook shelf insert, built with Embedded Swift on an ESP32-C6 (Waveshare ESP32-C6 Zero).

## Overview

BookNooks are decorative miniature scenes that fit inside a bookshelf between books. This project adds smart home control to a commercially available BookNook kit whose LEDs were originally powered by 2x AAA batteries (3V) through a touch switch PCB.

The ESP32-C6 replaces the battery pack entirely, powers the LEDs directly from a GPIO pin, and exposes an On/Off switch to Apple Home via the Matter protocol.

## Hardware

- **Board:** Waveshare ESP32-C6 Zero
- **LEDs:** 4 small ambient LEDs, less than 20mA total draw, connected to GPIO 4
- **Power:** LEDs driven directly from GPIO 4 (3.3V output, no MOSFET required)
- **Status indicator:** Onboard WS2812 RGB LED on GPIO 8

### Wiring

| Signal | GPIO | Wire color |
|--------|------|------------|
| LED +  | 4    | Red        |
| LED -  | GND  | Black      |

The original touch switch PCB and AAA battery holder are removed and replaced with this board.

## Software

This project is based on the `smart-light` example from the swift-matter-examples repository. The application is written in Embedded Swift and communicates with ESP-IDF and the Matter SDK through C/C++ shims.

### File Structure

```
booknook/
  main/
    Main.swift              Entry point, GPIO initialisation, Matter event handler
    LED.swift               Onboard WS2812 LED driver wrapper
    BridgingHeader.h        C/C++ header imports exposed to Swift
  Matter/
    MatterInterface.cpp     C++ shims including booknook GPIO helper functions
    Node, Clusters, ...     Matter protocol overlay (Swift)
```

### GPIO Shim

Embedded Swift does not reliably pass ESP-IDF GPIO enum types across the language boundary. For this reason, GPIO control for the BookNook LEDs is handled through two C shim functions defined in `Matter/MatterInterface.cpp`:

- `booknook_gpio_init()` — configures GPIO 4 as an output
- `booknook_gpio_set(bool on)` — drives GPIO 4 high or low

Swift calls these functions directly rather than invoking the ESP-IDF GPIO API itself.

## Building and Flashing

Prerequisites: ESP-IDF and the swift-matter-examples toolchain set up as described in the repository root README.

```sh
cd booknook
idf.py set-target esp32c6
idf.py build flash monitor
```

## Usage

After flashing, commission the device into Apple Home using the Matter pairing code printed to the serial monitor. The BookNook LEDs are then controllable as an On/Off light accessory from the Home app, Siri, or any Matter-compatible controller.

The onboard RGB LED reflects the device status during commissioning and normal operation.
