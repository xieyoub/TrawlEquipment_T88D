#ifndef _LED_H
#define _LED_H
#include "main.h"

void Uart_SendData(USART_TypeDef* USARTx,uint16_t Data);
void AllLedOff(void);
void Led_Init(void);
void Uart2_Init(u32 bound);
void Uart3_Init(u32 bound);
void TM1652_Init(u32 bound);
void SendDataNum(USART_TypeDef* USARTx,char Num);
void NixieTubeLowLight(void);
void LeftRightOffsetDisplay(u32 Num);
void LeftOffsetDisplay(u16 Num);
void RightOffsetDisplay(u16 Num);
void BackOffsetDisplay(u16 Num);
void NixieTube_LeftRightOff(void);
void NixieTube_AllOff(void);
void NixieTubeDisplay(void);
void updateLedState(void);
void LedState(void);

//											LED1 R:PB14, G:PA15
//											LED2 R:PA8 , G:PB15
//											LED3 R:PA12, G:PA11
//           ×óÏÏ:R:PB8,  G:PB9
//           ÓÒÏÏ:R:PA4£¬ G:PA5
//           ºó·½:R:PB7£¬ G:PB6
											
#define Led_NetTail_GreenOn     GPIO_ResetBits(GPIOA,GPIO_Pin_15);GPIO_ResetBits(GPIOB,GPIO_Pin_7) //ÂÌ
#define Led_NetTail_RedOn       GPIO_ResetBits(GPIOB,GPIO_Pin_14);GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define Led_NetTail_GreenOff    GPIO_SetBits(GPIOA,GPIO_Pin_15);GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define Led_NetTail_RedOff      GPIO_SetBits(GPIOB,GPIO_Pin_14);GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define Led_NetLeft_GreenOn     GPIO_ResetBits(GPIOB,GPIO_Pin_15);GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define Led_NetLeft_RedOn       GPIO_ResetBits(GPIOA,GPIO_Pin_8);GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define Led_NetLeft_GreenOff    GPIO_SetBits(GPIOB,GPIO_Pin_15);GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define Led_NetLeft_RedOff      GPIO_SetBits(GPIOA,GPIO_Pin_8);GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define Led_NetRight_GreenOn     GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define Led_NetRight_RedOn       GPIO_ResetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define Led_NetRight_GreenOff    GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define Led_NetRight_RedOff      GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOA,GPIO_Pin_5)   

#endif
