#include <stdio.h>
#include <wm_hal.h>
#include "core.h"

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
  // core::serial_init();
  HAL_Init();
  GPIO_init();
  set_all();
  printf("Hello, World!\n");
  return 0;
}
}