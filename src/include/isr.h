//
// Created by Kurosu Chan on 2024/4/30.
//

#ifndef INTERRUPTION_H
#define INTERRUPTION_H

extern "C" __attribute__((isr)) void CORET_IRQHandler();
extern "C" __attribute__((isr)) void TIM0_5_IRQHandler();

#endif // INTERRUPTION_H
