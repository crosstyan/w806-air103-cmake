/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     system.c
 * @brief    CSI Device System Source File
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include "csi_core.h"
#include "wm_regs.h"

/**
 * @brief  initialize the system
 *         Initialize the psr and vbr.
 * @return None
 */
void SystemInit(void) {
  __set_VBR((uint32_t) & (irq_vectors));

#if defined(CONFIG_SEPARATE_IRQ_SP) && !defined(CONFIG_KERNEL_NONE)
  extern int32_t g_top_irqstack;
  __set_Int_SP((uint32_t)&g_top_irqstack);
  __set_CHR(__get_CHR() | CHR_ISE_Msk);
  VIC->TSPR = 0xFF;
#else
#warning "No Kernel are configured or CONFIG_SEPARATE_IRQ_SP is not enabled, if this is not expected, please check the configuration file."
#endif
  __set_CHR(__get_CHR() | CHR_IAE_Msk);

  /* Clear active and pending IRQ */
  VIC->IABR[0] = 0x0;
  VIC->ICPR[0] = 0xFFFFFFFF;

#if CONFIG_KERNEL_NONE
#warning "No Kernel are configured, if this is not expected, please check the configuration file."
  __enable_excp_irq();
#endif
}
