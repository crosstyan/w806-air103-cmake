#include <stdio.h>
#include <wm_hal.h>
#include "core.h"
#include "wm_cpu.h"

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

void delay_1s(size_t s){
    // 240MHz CPU, 240000000/3 = 80000000
    // why divide by 3? I don't know, copilot told me to do so
    // add, compare, and nop; well, I guess it indeed needs 3 cycles
    // unless writing assembly code directly, the timing is hard to control
    constexpr uint64_t delay = 80000000;
    for (size_t i = 0; i < s; i++) {
        for (uint64_t j = 0; j < delay; j++){
            __asm__("nop");
        };
    }
}

extern "C" {
int main() {
  SystemClock_Config(CPU_CLK_240M);
  core::serial_init();
  constexpr auto TICK_INT_PRIORITY = 7;
  HAL_InitTick(TICK_INT_PRIORITY);
  HAL_Init();
  GPIO_init();
  printf("Hello, World!\n");
  bool state = false;
  for (;;){
    set_all(state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    state = !state;
    delay_1s(1);
    const auto count = HAL_GetTick();
    const auto coret_load = CORET->LOAD;
    const auto coret_val = CORET->VAL;
    const auto coret_count_flag = (CORET->CTRL & CORET_CTRL_COUNTFLAG_Msk) >> CORET_CTRL_COUNTFLAG_Pos;
    const auto coret_tick_int = (CORET->CTRL & CORET_CTRL_TICKINT_Msk) >> CORET_CTRL_TICKINT_Pos;
    printf("Hello, World! count=%lu, coret_load=%lu, coret_val=%lu, coret_count_flag=%lu\n", count, coret_load, coret_val, coret_count_flag);
    printf("Hello, World! coret_tick_int=%lu\n", coret_tick_int);
  }
  return 0;
}
}