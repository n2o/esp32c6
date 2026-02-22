// Simple on/off driver for the Seeed Studio XIAO ESP32-C6 onboard LED.
// The LED is a regular (non-RGB) LED on GPIO 15, active LOW.

final class SeeedLED {
  private let pin: Int32 = 15

  var enabled: Bool = false {
    didSet {
      // Active LOW: 0 = on, 1 = off
      booknook_gpio_set(pin, enabled ? 0 : 1)
    }
  }

  init() {
    booknook_gpio_init(pin)
    // Start with LED off (HIGH = off for active LOW)
    booknook_gpio_set(pin, 1)
  }
}
