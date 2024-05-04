/**
 * @file    wm_irq.c
 *
 * @brief   interupt driver module
 *
 * @author  dave
 *
 * Copyright (c) 2015 Winner Microelectronics Co., Ltd.
 */

#include "priv/core_804.h"
#include "priv/wm_regs.h"
#include "pub/wm_irq.h"

static u32 irqen_status = 0;

/**
 * @brief          	This function is used to enable interrupt.
 *
 * @param[in]      	vec_no       interrupt no
 *
 * @return         	None
 *
 * @note           	None
 */
void tls_irq_enable(u8 vec_no) {
  if ((irqen_status & (1 << vec_no)) == 0) {
    irqen_status |= 1 << vec_no;
    NVIC_ClearPendingIRQ((IRQn_Type)vec_no);
    NVIC_EnableIRQ((IRQn_Type)vec_no);
  }
}

/**
 * @brief          	This function is used to disable interrupt.
 *
 * @param[in]      	vec_no       interrupt no
 *
 * @return         	None
 *
 * @note           	None
 */
void tls_irq_disable(u8 vec_no) {
  if (irqen_status & (1 << vec_no)) {
    irqen_status &= ~(1 << vec_no);
    NVIC_DisableIRQ((IRQn_Type)vec_no);
  }
}

void tls_irq_priority(u8 vec_no, u32 prio) {
  NVIC_SetPriority((IRQn_Type)vec_no, prio);
}
