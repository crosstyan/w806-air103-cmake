/**
 * @file    wm_irq.h
 *
 * @brief   interupt driver module
 *
 * @author  dave
 *
 * Copyright (c) 2015 Winner Microelectronics Co., Ltd.
 */

#ifndef WM_IRQ_H
#define WM_IRQ_H
#include "int_alias.h"

typedef void (*intr_handler_func) (void *);

/**
 * @typedef struct tls_irq_handler
 */
typedef struct tls_irq_handler
{
    void (*handler) (void *);
    void *data;
    u32 counter;
} tls_irq_handler_t;

/**
 * @defgroup Driver_APIs Driver APIs
 * @brief Driver APIs
 */

/**
 * @addtogroup Driver_APIs
 * @{
 */

/**
 * @defgroup IRQ_Driver_APIs IRQ Driver APIs
 * @brief IRQ driver APIs
 */

/**
 * @addtogroup IRQ_Driver_APIs
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief          This function is used to enable interrupt.
 *
 * @param[in]      vec_no       interrupt NO
 *
 * @return         None
 *
 * @note           None
 */
void tls_irq_enable(u8 vec_no);

/**
 * @brief          This function is used to disable interrupt.
 *
 * @param[in]      vec_no       interrupt NO
 *
 * @return         None
 *
 * @note           None
 */
void tls_irq_disable(u8 vec_no);

void tls_irq_priority(u8 vec_no, u32 prio);


/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* WM_IRQ_H */
