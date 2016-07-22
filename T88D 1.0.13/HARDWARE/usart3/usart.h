#ifndef __USART_H
#define __USART_H
#include "main.h"

#define Usart1_OutPut   GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define Usart1_Receive  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define UART_RX1_LEN     20 

void USART1_ModeInit(void);
void USART1_GPIOInit(void);
void USART1_NVIC_Configuration(void);
void UART1_DMA_Configuration(void);
void USART1_Config(void);
void T800Insertion(void);
void GetCom1Data(void);
void Com1SendData(void);
void GetOffset(void);
void NetDetecting(void);

void Postfault(void);
void Pendfault(void);
extern u8 rx1buf[UART_RX1_LEN];  //串口1接收缓存
extern u8 tx1buf[UART_RX1_LEN]; 	   //串口1发送缓冲

#endif


