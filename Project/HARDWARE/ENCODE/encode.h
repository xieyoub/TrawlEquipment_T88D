#ifndef _ENCODE_H
#define _ENCODE_H
#include "main.h"

void Encode_Init(void);
void TIME2_Encode(void);
void TIME3_Encode(void);
void EXTI4_IRQhandler(void);
void EXTI9_5_IRQhandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void NixieTub_three(u16 Num);
void NixieTub_four(u16 Num);
void DisableEncode(void);

#endif
