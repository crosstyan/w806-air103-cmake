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
    set_all(GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(500));
    printf("t=%lld m=%ld\n", hal::cpu::tick_us(), xTaskGetTickCount());
  }
};

constexpr auto naive_delay = [](uint32_t cpu_freq, uint32_t ms) {
  const auto cycles_ms = cpu_freq / 8 / 1'000;
  for (uint32_t i = 0; i < ms * cycles_ms; ++i) {
    __asm volatile("nop");
    if (i % 5000'000 == 0) {
      printf("t=%lld\n", hal::cpu::tick_us());
    }
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

  // might caused by
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
