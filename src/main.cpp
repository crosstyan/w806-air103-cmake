#include <cstdint>
#include <wm_hal.h>
#include "core.h"
#include "core_804.h"
#include "wm_cpu.hpp"
#include "wm_regs.h"
// #include "wm_timer.hpp"

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

extern "C" __attribute__((constructor)) void premain() {
  SystemClock_Config(CPU_CLK_240M);
  core::serial_init();
  HAL_Init();
}

extern "C" {
[[noreturn]] int main() {
  GPIO_init();
  printf("Hello, World!\n");
  bool state = false;
  // auto timer_cfg = tls_timer_cfg{TLS_TIMER_UNIT_MS, 500, true, [] { printf("timer expired\n"); }};
  // const auto id  = tls_timer_create(&timer_cfg);
  // tls_timer_start(id);
  for (;;) {
    hal::cpu::delay_ms(500);
    set_all(state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    state            = !state;
    const auto count = hal::cpu::tick_ms();
    printf("tick=%lums\n", count);
  }
}
}
