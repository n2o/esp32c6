//===----------------------------------------------------------------------===//
//
// This source file is part of the Swift open source project
//
// Copyright (c) 2024 Apple Inc. and the Swift project authors.
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
//
//===----------------------------------------------------------------------===//

@_cdecl("app_main")
func main() {
  print("BookNook Light")

  let led = LED()

  // GPIO for BookNook LEDs (using C shim for reliable GPIO control)
  let bookNookGpioPin: Int32 = 4
  booknook_gpio_init(bookNookGpioPin)
  print("BookNook GPIO ready")

  // (1) Create a Matter root node
  let rootNode = Matter.Node()
  rootNode.identifyHandler = {
    print("identify")
  }

  // (2) Create a "light" endpoint
  let lightEndpoint = Matter.ExtendedColorLight(node: rootNode)
  lightEndpoint.eventHandler = { event in
    switch event.attribute {
    case .onOff:
      led.enabled = (event.value == 1)
      booknook_gpio_set(bookNookGpioPin, event.value == 1 ? 1 : 0)

    case .levelControl:
      led.brightness = Int(Float(event.value) / 255.0 * 100.0)

    default:
      break
    }
  }

  // (3) Add the endpoint to the node
  rootNode.addEndpoint(lightEndpoint)

  // (4) Provide the node to a Matter application and start it
  let app = Matter.Application()
  app.rootNode = rootNode
  app.start()

  // Keep local variables alive. Workaround for issue #10
  // https://github.com/apple/swift-matter-examples/issues/10
  while true {
    sleep(1)
  }
}
