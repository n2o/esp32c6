# AGENTS.md — ESP32 Embedded Swift Matter Project

This file documents key findings, lessons learned, and known pitfalls for AI agents and developers working on this project.

---

## Project Structure

- This is a forked version of swiftlang/swift-matter-examples.
- Contains two ESP-IDF projects: `smart-light/` and `booknook/`.
- Each project is a standalone ESP-IDF application built with `idf.py`.
- Hardware target: Waveshare ESP32-C6 Zero.
- Matter/HomeKit integration via ESP-Matter SDK.

---

## Embedded Swift Limitations

- `print()` only accepts `StaticString`. You CANNOT print variables. For example, `print("value:", someVar)` will not compile. Only string literals are allowed.
- ESP-IDF C enum types (such as `gpio_mode_t` and `gpio_num_t`) are not reliably imported into Embedded Swift. Calling ESP-IDF GPIO functions directly from Swift may compile but will not work correctly at runtime.
- The workaround is to use C/C++ shim functions (see `Matter/MatterInterface.cpp`) for any ESP-IDF hardware interaction that involves C enums or complex types.
- `esp_err_t` cannot be cast with `Int()` or `.rawValue` in Embedded Swift.

---

## GPIO Control Pattern

- Do NOT call `gpio_set_direction()` or `gpio_set_level()` directly from Swift. It compiles but the GPIO does not actually work.
- Instead, use C shim functions defined in `Matter/MatterInterface.cpp` and declared in `Matter/MatterInterface.h`.
- The shim pattern: declare as `extern "C"` in the `.h` file, implement in the `.cpp` file, call from Swift.
- `gpio_reset_pin()` should be called before configuring a pin to clear any alternate functions.
- ESP32-C6 GPIO 4-7 are JTAG pins but CAN be used as GPIO when configured via C shims using `gpio_config()`.

### Example Shim Pattern

In `Matter/MatterInterface.h`:
```c
extern "C" void shim_gpio_set_output(int pin, int level);
```

In `Matter/MatterInterface.cpp`:
```cpp
extern "C" void shim_gpio_set_output(int pin, int level) {
    gpio_reset_pin((gpio_num_t)pin);
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);
    gpio_set_level((gpio_num_t)pin, level);
}
```

In Swift:
```swift
shim_gpio_set_output(4, 1)
```

---

## Waveshare ESP32-C6 Zero Pinout

- GPIO 8: Onboard WS2812 RGB LED (used by `LED.swift` via RMT peripheral).
- GPIO 9: Boot button (used for Matter commissioning).
- GPIO 4-7: JTAG pins — usable as GPIO with proper reset via `gpio_config()` in a C shim.
- GPIO 0-3: Safe general-purpose pins.
- GPIO 14, 15, 18-23: Available on the right side of the board.
- A 3.3V output pin is available for powering external components.
- Pin labels are printed on the PCB silkscreen.

---

## Build System

- Built with ESP-IDF CMake, NOT Swift Package Manager.
- `Package.swift` at the repository root is a stub for IDE support only. It does not drive the build.
- Swift sources are listed in `main/CMakeLists.txt` under `target_sources()`.
- C++ shims are compiled via the `SRCS` list in `idf_component_register()`.
- `BridgingHeader.h` imports all C/C++ headers into Swift.
- `#include <driver/gpio.h>` must be present in `BridgingHeader.h` for GPIO types to be visible.

### Adding a New Swift File

1. Create the `.swift` file in the `main/` directory.
2. Add it to `target_sources()` in `main/CMakeLists.txt`.
3. If it uses C types, ensure the relevant headers are included in `BridgingHeader.h`.

### Adding a New C/C++ Shim

1. Declare the function with `extern "C"` in `Matter/MatterInterface.h`.
2. Implement it in `Matter/MatterInterface.cpp`.
3. Add the `.cpp` file to `SRCS` in the component's `CMakeLists.txt` if it is not already listed.
4. Include the header in `BridgingHeader.h`.

---

## Matter / Apple Home

- Matter commissioning uses BLE (NimBLE) during initial setup.
- The button on GPIO 9 (long press, 5 seconds) triggers re-commissioning.
- The `ExtendedColorLight` endpoint type provides on/off, brightness, and color control in Apple Home.
- The event handler receives attribute changes: `.onOff`, `.levelControl`, `.colorControl(...)`.
- For simple on/off devices, only handle the `.onOff` case in the event handler.

---

## Flash and Monitor

Build the project:
```
idf.py set-target esp32c6
idf.py build
```

Flash and open serial monitor:
```
idf.py -p /dev/cu.usbmodem101 flash monitor
```

Common issues:

- If the port is busy: close any other serial monitors and replug the USB cable.
- If flashing fails: hold the BOOT button, press RESET, release BOOT, then run the flash command again.
- The port path `/dev/cu.usbmodem101` may vary. Use `ls /dev/cu.*` to find the correct device.

---

## General Pitfalls

- Embedded Swift is not the same as standard Swift. Many stdlib features are unavailable. When in doubt, check whether a type or function is supported in the Embedded Swift subset.
- Always clean and rebuild after modifying `CMakeLists.txt` or `sdkconfig`. Stale build artifacts cause confusing errors.
- Do not rely on Swift's `Foundation` or `Darwin` modules. They are not available in Embedded Swift.
- If a function compiles but does nothing at runtime, the most likely cause is that it calls an ESP-IDF function with a C enum argument that was not correctly imported. Move that logic into a C shim.
- The `sdkconfig` file controls which ESP-IDF components are enabled. Missing a required component in `sdkconfig` will cause linker errors, not compile errors.
