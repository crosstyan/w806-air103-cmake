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

extern "C" {
[[noreturn]] int main() {
  SystemClock_Config(CPU_CLK_240M);
  core::serial_init();
  core::rtos_init();
  HAL_Init();
  GPIO_init();
  StaticTask_t xTaskBuffer;
  StackType_t xStack[512];
#if CONFIG_KERNEL_FREERTOS
  xTaskCreateStatic(blink, "blink", std::size(xStack), nullptr, configMAX_PRIORITIES - 4, xStack, &xTaskBuffer);
  vTaskStartScheduler();
#else
  for (;;) {
    printf("t=%lld\n", hal::cpu::tick_us());
  }
#endif
}
}
