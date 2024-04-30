#include <cstdio>
#include <wm_cpu.h>
#include "board_init.h"

extern "C" {
[[noreturn]] int main() {
  core::board_init();
  while (true) {
    printf("Hello, World!\n");
    HAL_Delay(1000);
  }
}
}
