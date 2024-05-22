//
// Created by Kurosu Chan on 2024/4/30.
//

#ifndef BOARD_INIT_H
#define BOARD_INIT_H
#include <stdint.h>

namespace core {
/**
 * @brief initialize serial port for logging
 */
void serial_init();
/**
 * @brief start TIMER 0, which is used for FreeRTOS
 */
void rtos_init(uint32_t NVIC_priority);
} // namespace core

#endif // BOARD_INIT_H
