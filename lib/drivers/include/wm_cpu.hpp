//
// Created by cross on 5/4/2024.
//

#ifndef WM_CPU_HPP
#define WM_CPU_HPP
#include <chrono>
#include "wm_cpu.h"

namespace hal::cpu {
constexpr auto HAL_TICK_FREQ_1MHZ = 1000'000;
constexpr auto HAL_TICK_FREQ_1KHz = 1000;
static constexpr auto TICK_FREQUENCY = HAL_TICK_FREQ_1MHZ;
namespace details {
  inline uint64_t __tick__;
}
using tick_rep_t   = decltype(details::__tick__);
using milliseconds = std::chrono::duration<tick_rep_t, std::milli>;
using microseconds = std::chrono::duration<tick_rep_t, std::micro>;
using tick_t       = std::conditional_t<TICK_FREQUENCY == HAL_TICK_FREQ_1MHZ, microseconds, milliseconds>;

inline void inc_tick() {
  details::__tick__ += 1;
}

inline auto tick_now() {
  return tick_t{details::__tick__};
}

inline auto tick_ms() -> tick_rep_t {
  return std::chrono::duration_cast<milliseconds>(tick_now()).count();
}

inline auto tick_us() -> tick_rep_t {
  return std::chrono::duration_cast<microseconds>(tick_now()).count();
}

inline void delay_ms(int ms) {
  const auto start = std::chrono::duration_cast<milliseconds>(tick_now());
  const auto dur   = milliseconds{ms};
  while (std::chrono::duration_cast<milliseconds>(tick_now() - start) < dur) {
    __asm__("nop");
  }
}

template <typename Rep, typename Period>
void delay(std::chrono::duration<Rep, Period> dur) {
  const auto start = tick_now();
  while (tick_now() - start < dur) {
    __asm__("nop");
  }
}
}

#endif // WM_CPU_HPP
