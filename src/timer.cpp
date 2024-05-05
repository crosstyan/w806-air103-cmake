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

namespace core {
void rtos_init() {
  __HAL_RCC_TIM_CLK_ENABLE();
  HAL_NVIC_SetPriority(TIM_IRQn, 0b00);
  HAL_NVIC_EnableIRQ(TIM_IRQn);
  timer::TIM0_Init();
};
}
