#include <cstdio>
#include <wm_cpu.h>
#include "board_init.h"

constexpr auto O1 = GPIO_PIN_24;
constexpr auto O2 = GPIO_PIN_25;
constexpr auto O3 = GPIO_PIN_26;
const auto G      = GPIOB;

static constexpr auto GPIO_init = [] {
  auto init = GPIO_InitTypeDef{O1 | O2 | O3, GPIO_MODE_OUTPUT, GPIO_NOPULL};
  HAL_GPIO_Init(G, &init);
};

static constexpr auto all_set = [] {
  HAL_GPIO_WritePin(G, O1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(G, O2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(G, O3, GPIO_PIN_SET);
};

static constexpr auto all_unset = [] {
  HAL_GPIO_WritePin(G, O1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(G, O2, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(G, O3, GPIO_PIN_RESET);
};

extern "C" {
[[noreturn]] int main() {
  SystemClock_Config(CPU_CLK_240M);
  core::board_init();
  HAL_Init();
  GPIO_init();
  bool flag = false;
  while (true) {
    if (flag) {
      all_set();
    } else {
      all_unset();
    }
    flag = not flag;
    printf("Hello, World!\n");
    HAL_Delay(500);
  }
}
}
