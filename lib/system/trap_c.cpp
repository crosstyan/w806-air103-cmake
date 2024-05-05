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
 * @file     trap_c.cpp
 * @brief    source file for the trap process
 * @version  V1.0
 * @date     12. December 2017
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <core_804.h>

extern "C" __attribute__((used)) void trap_c(uint32_t *regs) {
  int i;
  uint32_t psr_ = 0;
  asm volatile(
      "mfcr    %0, psr \n"
      : "=r"(psr_) :);

  const auto &psr = *reinterpret_cast<PSR_Type *>(&psr_);
  printf("psr=%8x\tvec=%d\n", psr.w, psr.b.VEC);

  for (i = 0; i < 16; i++) {
    printf("r%d=%08x\t", i, regs[i]);

    if ((i % 5) == 4) {
      printf("\n");
    }
  }
  printf("\n");
  printf("epsr=%8x\tepc=%8x", regs[16], regs[17]);
}
