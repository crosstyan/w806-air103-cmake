//
// Created by Kurosu Chan on 2024/4/30.
//

#include "interruption.h"
#include "wm_hal.h"
#include "wm_regs.h"
#include <stdio.h>

/**
 * @brief Reads a 32-bit value from a memory-mapped hardware register or memory location.
 *
 * This function reads a 32-bit unsigned integer value from the specified memory address.
 * It is commonly used for accessing hardware registers in memory-mapped I/O (MMIO) regions.
 *
 * @param addr The memory address to read from. It should be a valid pointer to a volatile memory location.
 * @return The 32-bit unsigned integer value read from the specified memory address.
 *
 * @note The memory address is treated as volatile to prevent compiler optimizations that may affect the intended behavior.
 *
 * @warning This function directly accesses memory-mapped hardware registers. Ensure that the address is valid and accessible.
 *
 * @sa https://github.com/IOsetting/wm-sdk-w806/blob/03b0f7fec247b05e16b5abb8c2310958f07114e9/app/src/wm_it.c
 */
[[nodiscard]] inline uint32_t read_addr(uintptr_t addr) {
  const auto ptr = reinterpret_cast<volatile uint32_t *>(addr);
  return *ptr;
}

#define readl(addr) ({unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })
extern "C" __attribute__((isr)) void CORET_IRQHandler() {
  /**
   * No one knows what this magic number is for.
   * Some random article on the internet says it's for the SysTick timer.
   * But I can't find any reference to it in the datasheet.
   *
   * Apparently one must read the register to clear the interrupt flag,
   * Otherwise no more interrupt will be triggered.
   *
   * Wait...? This moron find the register in Arm Cortex-M0 Documentation, how could
   * that relate to WinnerMicro W806?! It's a different architecture!
   *
   * Unless the moron who designed this chip is a fan of Arm Cortex-M0 and decided to
   * map the SysTick register to the same address as the Arm Cortex-M0. But why it
   * doesn't mention it in the datasheet?!
   *
   * 0xE000'0000 - 0xFFFF'FFFF is called System Level Peripheral, without giving any
   * documentation about it. It's like a black box that no one knows what's inside.
   * Luckily, `core_804.h` is open source and still readable, although not enough
   * details given. Better than nothing.
   *
   * That's weird. I'll just leave it here.
   *
   * @sa https://developer.arm.com/documentation/dui0497/a/cortex-m0-peripherals/optional-system-timer--systick/systick-control-and-status-register
   * @sa https://bbs.elecfans.com/jishu_2197051_1_1.html
   * @sa https://gitee.com/openLuat/luatos-soc-air101
   */

  // https://github.com/RT-Thread/rt-thread/blob/b632dc1aaf2efa4156b87b2d534cf6732010eaee/libcpu/c-sky/ck802/core_ck802.h#L398-L445
  constexpr uintptr_t MAGIC_ADDR = CORET_BASE; // 0xE000'E010
  static_assert(MAGIC_ADDR == 0xE000'E010, "unexpected address for CORET_BASE");
  HAL_IncTick();
  const auto _ = read_addr(MAGIC_ADDR);
  static_cast<void>(_);
}
