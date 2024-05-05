#include <cstdint>
#include <wm_hal.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include "core.h"
#include "wm_cpu.hpp"
#include "wm_regs.h"

constexpr auto O1 = GPIO_PIN_24;
constexpr auto O2 = GPIO_PIN_25;
constexpr auto O3 = GPIO_PIN_26;
const auto GRP    = GPIOB;

static const auto GPIO_init = [] {
  auto init = GPIO_InitTypeDef{O1 | O2 | O3, GPIO_MODE_OUTPUT, GPIO_NOPULL};
  HAL_GPIO_Init(GRP, &init);
};

static const auto set_all = [](GPIO_PinState st = GPIO_PIN_SET) {
  HAL_GPIO_WritePin(GRP, O1, st);
  HAL_GPIO_WritePin(GRP, O2, st);
  HAL_GPIO_WritePin(GRP, O3, st);
};

[[noreturn]] static constexpr auto blink = [](void *pvParameters) -> void {
  for (;;) {
    set_all(GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(500));

    const auto s = hal::cpu::tick_us();
    for (int i = 0; i < 100'000; i++) {
      __NOP();
    }
    const auto e = hal::cpu::tick_us();
    printf("diff=%lld\n", e - s);

    set_all(GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(500));
    printf("t=%lld m=%ld\n", hal::cpu::tick_us(), xTaskGetTickCount());
  }
};

/**
 * @brief naive implementation of delay
 */
constexpr auto naive_delay = [](uint32_t cycles) {
  for (uint32_t i = 0; i < cycles; i++) {
    __NOP();
  }
};

/**
 * @brief delay in microsecond in 240Mhz
 * @warning based on empirical data
 */
constexpr auto naive_240M_delay_us = [](uint32_t us) {
  // 100'000 iteration taks 1565 us
  const auto iteration_per_us = 100'000 / 1565;
  for (auto i = 0; i < iteration_per_us * us; i++) {
    __NOP();
  }
};

extern "C" {
[[noreturn]] int main() {
  constexpr auto M240 = 240'000'000;
  SystemClock_Config(CPU_CLK_240M);
  core::rtos_init(0b00);
  core::serial_init();
  GPIO_init();

  StaticTask_t xTaskBuffer;
  StackType_t xStack[512];
#if CONFIG_KERNEL_FREERTOS
  // relate to interruption priority
  // CORET Interruption might conflict with
  // Timer Interruption

  // exception caused by
  // portSET_INTERRUPT_MASK_FROM_ISR
  HAL_InitTick(0b10);
  xTaskCreateStatic(blink, "blink", std::size(xStack), nullptr, configMAX_PRIORITIES - 4, xStack, &xTaskBuffer);
  vTaskStartScheduler();
#else
  for (;;) {
    printf("t=%lld\n", hal::cpu::tick_us());
  }
#endif
}
}
