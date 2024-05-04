#include <cstdint>
#include <stdio.h>
#include <wm_gpio.h>
#include "core.h"
#include "core_804.h"
#include "wm_cpu.h"
#include "wm_cpu.hpp"
#include "wm_regs.h"


// constexpr auto O1 = GPIO_PIN_24;
// constexpr auto O2 = GPIO_PIN_25;
// constexpr auto O3 = GPIO_PIN_26;
// const auto GRP    = GPIOB;
//
// static const auto GPIO_init = [] {
//   auto init = GPIO_InitTypeDef{O1 | O2 | O3, GPIO_MODE_OUTPUT, GPIO_NOPULL};
//   tls_gpio_write(GRP, &init);
// };
//
// static const auto set_all = [](GPIO_PinState st = GPIO_PIN_SET) {
//   HAL_GPIO_WritePin(GRP, O1, st);
//   HAL_GPIO_WritePin(GRP, O2, st);
//   HAL_GPIO_WritePin(GRP, O3, st);
// };

extern "C" __attribute__((constructor)) void premain() {
  // SystemClock_Config(CPU_CLK_240M);
  tls_sys_clk_set(CPU_CLK_240M);
  core::serial_init();
  // HAL_Init();
}

extern "C" {
[[noreturn]] int main() {
  // GPIO_init();
  // printf("Hello, World!\n");
  // bool state = false;
  // for (;;) {
  //   hal::cpu::delay_ms(500);
  //   set_all(state ? GPIO_PIN_SET : GPIO_PIN_RESET);
  //   state            = !state;
  //   const auto count = hal::cpu::tick_ms();
  //   printf("tick=%lums\n", count);
  // }
}
}
