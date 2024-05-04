#include <cstdint>
#include <stdio.h>
#include <wm_hal.h>
#include "core.h"
#include "core_804.h"
#include "wm_cpu.h"
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

void delay_ms(size_t s) {
  // 240MHz CPU, 240000000/3 = 80000000
  // why divide by 3? I don't know, copilot told me to do so
  // add, compare, and nop; well, I guess it indeed needs 3 cycles
  // unless writing assembly code directly, the timing is hard to control
  constexpr uint64_t delay = 80000000 / 1000;
  for (size_t i = 0; i < s; i++) {
    for (uint64_t j = 0; j < delay; j++) {
      __asm__("nop");
    };
  }
}

extern "C" {
int main() {
  SystemClock_Config(CPU_CLK_240M);
  core::serial_init();
  HAL_Init();
  GPIO_init();
  printf("Hello, World!\n");
  bool state = false;
  for (;;) {
    HAL_Delay_Ms(1000);
    set_all(state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    state = !state;
    const auto count            = HAL_GetTick();
    const auto coret_load       = CORET->LOAD;
    const auto coret_val        = CORET->VAL;
    const auto coret_count_flag = (CORET->CTRL & CORET_CTRL_COUNTFLAG_Msk) >> CORET_CTRL_COUNTFLAG_Pos;
    printf("Hello, World! count=%lu, coret_load=%lu, coret_val=%lu\n", count, coret_load, coret_val);
    const auto coret_tick_int = (CORET->CTRL & CORET_CTRL_TICKINT_Msk) >> CORET_CTRL_TICKINT_Pos;
    const auto vic_enable     = csi_vic_get_enabled_irq(SYS_TICK_IRQn);
    const auto vic_activate   = csi_vic_get_active(SYS_TICK_IRQn);
    const auto vic_pending    = csi_vic_get_pending_irq(SYS_TICK_IRQn);
    csi_vic_clear_pending_irq(SYS_TICK_IRQn);
    const auto fn_addr = csi_vic_get_vector(SYS_TICK_IRQn);
    const auto fn      = reinterpret_cast<void (*)()>(fn_addr);
  }
  return 0;
}
}
