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

#include "BridgingHeader.h"

esp_err_t esp_matter::attribute::set_callback_shim(callback_t_shim callback) {
  return set_callback((callback_t)callback);
}

esp_matter::cluster_t *esp_matter::cluster::get_shim(esp_matter::endpoint_t *endpoint, unsigned int cluster_id) {
  return get(endpoint, (uint32_t)cluster_id);
}

esp_matter::attribute_t *esp_matter::attribute::get_shim(esp_matter::cluster_t *cluster, unsigned int attribute_id) {
  return get(cluster, (uint32_t)attribute_id);
}

extern "C" void booknook_gpio_init(int pin) {
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = (1ULL << pin);
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&io_conf);
  gpio_set_level((gpio_num_t)pin, 1);
}

extern "C" void booknook_gpio_set(int pin, int level) {
  gpio_set_level((gpio_num_t)pin, level);
}

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <esp_openthread.h>
#include <esp_openthread_types.h>
#include <platform/ESP32/OpenthreadLauncher.h>

// Default macros from ESP-IDF OpenThread examples
#define BOOKNOOK_OT_RADIO_CONFIG() \
    { .radio_mode = RADIO_MODE_NATIVE, }

#define BOOKNOOK_OT_HOST_CONFIG() \
    { .host_connection_mode = HOST_CONNECTION_MODE_NONE, }

#define BOOKNOOK_OT_PORT_CONFIG() \
    { .storage_partition_name = "nvs", .netif_queue_size = 10, .task_queue_size = 10, }
#endif

extern "C" void booknook_openthread_init(void) {
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
  esp_openthread_platform_config_t config = {
    .radio_config = BOOKNOOK_OT_RADIO_CONFIG(),
    .host_config = BOOKNOOK_OT_HOST_CONFIG(),
    .port_config = BOOKNOOK_OT_PORT_CONFIG(),
  };
  set_openthread_platform_config(&config);
#endif
}

void recomissionFabric() {
  if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0) {
    chip::CommissioningWindowManager & commissionMgr = chip::Server::GetInstance().GetCommissioningWindowManager();
    constexpr auto kTimeoutSeconds = chip::System::Clock::Seconds16(300);
    if (!commissionMgr.IsCommissioningWindowOpen()) {
      commissionMgr.OpenBasicCommissioningWindow(kTimeoutSeconds, chip::CommissioningWindowAdvertisement::kDnssdOnly);
    }
  }
}
