#include <cstdint>
#include <wm_hal.h>
#include "core.h"
#include "core_804.h"
#include "wm_cpu.hpp"
#include "wm_regs.h"
#include "interruption.h"

#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>

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

namespace core::timer {
static TIM_HandleTypeDef htim0;
static constexpr auto TIM0_Init = [] {
  htim0.Instance        = TIM0;
  htim0.Init.Unit       = TIM_UNIT_US;
  htim0.Init.Period     = 1'000;
  htim0.Init.AutoReload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim0) != HAL_OK) {
    printf("TIM0 init failed\n");
  }
  const auto ok = HAL_TIM_Base_Start_IT(&htim0);
  if (ok != HAL_OK) {
    printf("TIM0 start failed\n");
  }
};
}

namespace core {
static constexpr auto rtos_init = [] {
  using namespace core::timer;
  __HAL_RCC_TIM_CLK_ENABLE();
  HAL_NVIC_SetPriority(TIM_IRQn, 0b01);
  HAL_NVIC_EnableIRQ(TIM_IRQn);
  TIM0_Init();
};
}

__attribute__((isr)) void TIM0_5_IRQHandler() {
  using namespace core::timer;
#if CONFIG_KERNEL_FREERTOS
  const portLONG psr = portSET_INTERRUPT_MASK_FROM_ISR();
#endif

  if (__HAL_TIM_GET_FLAG(&htim0) != RESET) {
    __HAL_TIM_CLEAR_IT(&htim0);
#if CONFIG_KERNEL_FREERTOS
    xTaskIncrementTick();
#endif
  }

#if CONFIG_KERNEL_FREERTOS
  portYIELD_FROM_ISR(pdTRUE);
  portCLEAR_INTERRUPT_MASK_FROM_ISR(psr);
#endif
}

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
  const auto cycles_ms = cpu_freq / 3 / 1'000;
  for (uint32_t i = 0; i < ms * cycles_ms; ++i) {
    __asm volatile("nop");
  }
};

extern "C" {
[[noreturn]] int main() {
  constexpr auto CPU_CYCLES = 240'000'000;
  SystemClock_Config(CPU_CLK_240M);
  // naive_delay(CPU_CYCLES, 3'000);
  core::serial_init();
  GPIO_init();
  core::rtos_init();
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
