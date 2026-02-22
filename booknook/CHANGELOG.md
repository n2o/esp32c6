# Changelog

## 1.1.0

Target board: **Seeed Studio XIAO ESP32-C6**

- Switch from Wi-Fi to Thread (OpenThread) for Matter commissioning via 802.15.4
- Add SeeedLED driver for XIAO onboard user LED (GPIO 15, active LOW)
- Switch BookNook external LED GPIO from pin 4 to pin 0 (avoids JTAG conflict on XIAO)
- Add `booknook_openthread_init()` C shim for OpenThread platform config
- Print version number on boot

## 1.0.0

Target board: **Waveshare ESP32-C6 Zero**

- Initial booknook example with Matter ExtendedColorLight
- WS2812 RGB LED driver (onboard LED on GPIO 8)
- BookNook external LED control via GPIO 4 C shim
- Matter commissioning over Wi-Fi with BLE
