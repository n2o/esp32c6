# Swift Matter Examples

Build a Matter accessory using Embedded Swift.

Forked from [swiftlang/swift-matter-examples](https://github.com/swiftlang/swift-matter-examples). This fork targets the [Waveshare ESP32-C6 Zero](https://www.waveshare.com/esp32-c6-zero.htm) and adds new examples beyond the original Apple smart-light demo.

## Examples

### smart-light

The original Apple example. Implements a Matter smart light accessory using the onboard WS2812 RGB LED on an ESP32-C6. Supports on/off, brightness, and color control via Apple Home.

### booknook

Controls ambient LEDs inside a BookNook (a decorative miniature bookshelf insert) via Apple Home. Uses a Waveshare ESP32-C6 Zero to replace the original battery-powered touch switch. LEDs are connected directly to GPIO 4.

## Background

### Embedded Swift

Embedded Swift is a subset of Swift designed for constrained environments such as embedded devices and kernel code. It includes most Swift language features — generics, value and reference types, optionals, error handling, and more — while introducing the safety and expressivity of Swift to environments usually dominated by C or C++. See [A Vision for Embedded Swift](https://github.com/swiftlang/swift-evolution/blob/main/visions/embedded-swift.md) for more information.

### Matter

Matter is an open standard for building smart home accessories, supported natively by ecosystems such as Apple HomeKit. See the [Matter documentation](https://project-chip.github.io/connectedhomeip-doc/index.html) for more information.

### Additional Resources

- [Embedded Swift User Manual](https://github.com/swiftlang/swift/blob/main/docs/EmbeddedSwift/UserManual.md)
- [More Embedded Swift Example Projects](https://github.com/swiftlang/swift-embedded-examples)
- [Swift Forums — Embedded Discussion](https://forums.swift.org/c/development/embedded/107)

## Requirements

- Hardware: [Waveshare ESP32-C6 Zero](https://www.waveshare.com/esp32-c6-zero.htm) (or any ESP32-C6 board)
- Software:
  - [Swift Nightly Toolchain](https://www.swift.org/download)
  - [CMake 3.29+](https://cmake.org/download)
  - [ESP-IDF v5.4.1](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32c6/get-started/index.html)
  - [ESP-Matter SDK 1.2](https://docs.espressif.com/projects/esp-matter/en/latest/esp32c6/introduction.html)

The examples are designed for macOS and Linux host systems.

## Quick Start

1. Clone the repository and navigate to an example.
   ```shell
   git clone https://github.com/n2o/swift-matter-examples.git
   cd swift-matter-examples/smart-light
   ```

2. Set the target microcontroller.
   ```shell
   idf.py set-target esp32c6
   ```

3. Build and flash the application.
   ```shell
   idf.py build flash monitor
   ```

For initial environment setup, refer to the upstream tutorials for [macOS](https://swiftlang.github.io/swift-matter-examples/tutorials/swiftmatterexamples/setup-macos) or [Linux with Docker](https://swiftlang.github.io/swift-matter-examples/tutorials/swiftmatterexamples/setup-docker).
