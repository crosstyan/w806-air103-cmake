//
// Created by cross on 5/5/2024.
//
#include "core.h"
#include "wm_tim.h"
#include "isr.h"
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>

namespace core::timer {
TIM_HandleTypeDef htim0;

constexpr auto hz_to_us         = [](const auto hz) constexpr { return 1'000'000 / hz; };
static constexpr auto TIM0_Init = [] {
  htim0.Instance        = TIM0;
  htim0.Init.Unit       = TIM_UNIT_US;
  htim0.Init.Period     = hz_to_us(configTICK_RATE_HZ);
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

__attribute__((isr)) void TIM0_5_IRQHandler() {
  using namespace core::timer;
  if (__HAL_TIM_GET_FLAG(&htim0) != RESET) {
    __HAL_TIM_CLEAR_IT(&htim0);
#if CONFIG_KERNEL_FREERTOS
    xTaskIncrementTick();
#endif
  }
  // TODO: handle TIMER1-4 interrupt
  // with callback function
  // `cib` could help

#if CONFIG_KERNEL_FREERTOS
  portYIELD_FROM_ISR(pdTRUE);
#endif
}

namespace core {
void rtos_init(uint32_t NVIC_priority) {
  __HAL_RCC_TIM_CLK_ENABLE();
  NVIC_SetPriority(TIM_IRQn, NVIC_priority);
  HAL_NVIC_EnableIRQ(TIM_IRQn);
  timer::TIM0_Init();
};
}
