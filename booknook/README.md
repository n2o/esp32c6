# BookNook Ambient Lighting

A Matter/HomeKit-enabled ambient lighting controller for a decorative BookNook shelf insert, built with Embedded Swift on an ESP32-C6.

## Overview

BookNooks are decorative miniature scenes that fit inside a bookshelf between books. This project adds smart home control to a commercially available BookNook kit whose LEDs were originally powered by 2x AAA batteries (3V) through a touch switch PCB.

The ESP32-C6 replaces the battery pack entirely, powers the LEDs directly from a GPIO pin, and exposes an On/Off switch to Apple Home via the Matter protocol.

## Hardware

- **Board:** Seeed Studio XIAO ESP32-C6
- **LEDs:** 4 small ambient LEDs, less than 20mA total draw, connected to GPIO 0
- **Power:** LEDs driven directly from GPIO 0 (3.3V output, no MOSFET required)
- **Status indicator:** Onboard user LED on GPIO 15 (active LOW)
- **Commissioning:** Matter over Thread (802.15.4), requires a Thread Border Router (e.g. Apple HomePod mini, Apple TV 4K)

### Wiring

| Signal | GPIO | Wire color |
|--------|------|------------|
| LED +  | 0    | Red        |
| LED -  | GND  | Black      |

The original touch switch PCB and AAA battery holder are removed and replaced with this board.

## Software

This project is based on the `smart-light` example from the swift-matter-examples repository. The application is written in Embedded Swift and communicates with ESP-IDF and the Matter SDK through C/C++ shims.

### File Structure

```
booknook/
  main/
    Main.swift              Entry point, GPIO init, Matter event handler
    LED.swift               WS2812 LED driver wrapper (no-op on XIAO)
    SeeedLED.swift          XIAO onboard user LED driver (GPIO 15)
    BridgingHeader.h        C/C++ header imports exposed to Swift
  Matter/
    MatterInterface.cpp     C++ shims: GPIO helpers, OpenThread platform config
    Node, Clusters, ...     Matter protocol overlay (Swift)
```

### GPIO Shim

Embedded Swift does not reliably pass ESP-IDF GPIO enum types across the language boundary. For this reason, GPIO control for the BookNook LEDs is handled through two C shim functions defined in `Matter/MatterInterface.cpp`:

- `booknook_gpio_init()` -- configures a GPIO pin as output
- `booknook_gpio_set()` -- drives a GPIO pin high or low
- `booknook_openthread_init()` -- sets up OpenThread platform config for Thread commissioning

Swift calls these functions directly rather than invoking the ESP-IDF GPIO API itself.

## Building and Flashing

Prerequisites: ESP-IDF and the swift-matter-examples toolchain set up as described in the repository root README.

```sh
cd booknook
idf.py set-target esp32c6
idf.py build flash monitor
```

## Usage

After flashing, commission the device into Apple Home using the Matter pairing code printed to the serial monitor. A Thread Border Router (HomePod mini, HomePod 2nd gen, or Apple TV 4K) is required on your network. The BookNook LEDs are then controllable as a light accessory from the Home app, Siri, or any Matter-compatible controller.

The onboard LED on the XIAO board lights up when the device is switched on.
