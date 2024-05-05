//
// Created by Kurosu Chan on 2024/4/30.
//

#ifndef BOARD_INIT_H
#define BOARD_INIT_H

namespace core {
void serial_init();
/**
 * @brief start timer 0, which is used for FreeRTOS
 */
void rtos_init();
}

#endif // BOARD_INIT_H
