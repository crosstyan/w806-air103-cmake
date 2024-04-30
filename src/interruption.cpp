//
// Created by Kurosu Chan on 2024/4/30.
//

#include "interruption.h"
#include "wm_hal.h"

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
inline uint32_t readl(volatile uintptr_t addr) {
  const auto ptr = reinterpret_cast<volatile uint32_t *>(addr);
  return *ptr;
}

extern "C" __attribute__((isr)) void CORET_IRQHandler() {
  readl(0xE000E010);
  HAL_IncTick();
}
