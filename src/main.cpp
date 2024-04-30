#include <cstdio>
#include <wm_cpu.h>
#include "core.h"
#include "wm_gpio.h"

constexpr auto O1 = GPIO_PIN_24;
constexpr auto O2 = GPIO_PIN_25;
constexpr auto O3 = GPIO_PIN_26;
const auto GRP    = GPIOB;

static constexpr auto GPIO_init = [] {
  auto init = GPIO_InitTypeDef{O1 | O2 | O3, GPIO_MODE_OUTPUT, GPIO_NOPULL};
  HAL_GPIO_Init(GRP, &init);
};

static constexpr auto set_all = [](GPIO_PinState st = GPIO_PIN_SET) {
  HAL_GPIO_WritePin(GRP, O1, st);
  HAL_GPIO_WritePin(GRP, O2, st);
  HAL_GPIO_WritePin(GRP, O3, st);
};

extern "C" {
[[noreturn]] int main() {
  SystemClock_Config(CPU_CLK_240M);
  core::serial_init();
  HAL_Init();
  GPIO_init();
  static bool flag = false;
  while (true) {
    if (flag) {
      set_all();
    } else {
      set_all(GPIO_PIN_RESET);
    }
    flag = not flag;
    printf("Hello, World!\n");
    HAL_Delay(500);
  }
}
}
