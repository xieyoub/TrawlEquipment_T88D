#include "Led.h"

/************************************************************
 * 函数名：Uart_SendData
 * 描述  ：串口发送数据
 * 输入  ：无
 * 输出  ：无
 * 调用  ：
 ************************************************************/
void Uart_SendData(USART_TypeDef* USARTx,uint16_t Data)
{
	  USART_SendData(USARTx, Data);
   while (!(USARTx->SR & USART_FLAG_TXE));
}

/************************************************************
 * 函数名：AllLedOff
 * 描述  ：熄灭全部的二极管灯
 * 输入  ：无
 * 输出  ：无
 * 调用  ：
 ************************************************************/
void AllLedOff(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);

	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);		
}

/************************************************************
 * 函数名：Led_Init
 * 描述  ：拖网选择指示灯LED1，LED2，LED3,左舷，右舷，后方
											LED1 R:PB14, G:PA15
											LED2 R:PA8 , G:PB15
											LED3 R:PA12, G:PA11
           左舷:R:PB8,  G:PB9
           右舷:R:PA4， G:PA5
           后方:R:PB7， G:PB6
 * 输入  ：无
 * 输出  ：无
 * 调用  ：main
 ************************************************************/
void Led_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_5|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
 AllLedOff();
}



/************************************************************
 * 函数名：Uart2_Init
 * 描述  ：串口2配置 ，TX:PA2，RX:PA3，波特率19200
	          字长9位，一位停止位，一位奇校验位
											控制两个三位数码管
 * 输入  ：bound:19200
 * 输出  ：无
 * 调用  ：TM1652_Init
 ************************************************************/
//三位数码管数据接口
void Uart2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	USART_DeInit(USART2);
	//USART2_TX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART2_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART2 参数设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Odd;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2,&USART_InitStructure);
	USART_Cmd(USART2,ENABLE);
}

/************************************************************
 * 函数名：Uart3_Init
 * 描述  ：串口3配置 ，TX:PB10，RX:PB11,波特率19200，
								   字长9位，一个停止位，一位奇校验位
											控制一个四位数码管
 * 输入  ：bound :19200
 * 输出  ：无
 * 调用  ：TM1652_Init
 ************************************************************/
void Uart3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
 USART_DeInit(USART3);
	
	//USART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//USART3_RX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	//USART3 参数设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Odd;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3,&USART_InitStructure);
	USART_Cmd(USART3,ENABLE);	
}

/************************************************************
 * 函数名：TM1652_Init
 * 描述  ：配置USART3：控制一个四位数码管
           配置USART2：控制两个三位数码管
 * 输入  ：bound 串口波特率
 * 输出  ：无
 * 调用  ：由main调用
 ************************************************************/
void TM1652_Init(u32 bound)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
	Uart2_Init(bound);
	Uart3_Init(bound);
 NixieTube_AllOff();
}

//TM1652 串口通信
//位地址，最多为6位：
//GR1:0X08,GR2:0X88,GR3:0X48,GR4:0XC8,GR5:0X28,GR6:0XA8
//显示数据7段：0~9：
//0:0x3F, 1:0x06, 2:0x5B, 3:0x4F, 4:0x66, 5:0x6D, 6:0x7D
//7:0x07, 8:0x7F, 9:0x6F
//
//发送显示数据，0-9
//
void SendDataNum(USART_TypeDef* USARTx,char Num)
{
	switch(Num)//显示的数据
	{
			case 0:
			    	Uart_SendData(USARTx,0x3F);
								break;
			
			case 1:
								Uart_SendData(USARTx,0x06);
								break;
			
			case 2:
				    Uart_SendData(USARTx,0x5B);
								break;
			
			case 3:
				    Uart_SendData(USARTx,0x4F);
								break;
			
			case 4:
					   Uart_SendData(USARTx,0x66);
								break;
			
			case 5:
					   Uart_SendData(USARTx,0x6D);
								break;
			
			case 6:
					   Uart_SendData(USARTx,0x7D);
								break;
			
			case 7:
					   Uart_SendData(USARTx,0x07);
								break;
					
			case 8:
					   Uart_SendData(USARTx,0x7F);
								break;
			
			case 9:
					   Uart_SendData(USARTx,0x6F);
								break;
			case 10:
								Uart_SendData(USARTx,0x00);
								break;
			default:
								break;
		}
}

/************************************************************
 * 函数名：NixieTubeLowLight
 * 描述  ：数码管显示变暗
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void NixieTubeLowLight(void)
{	
		NixieTube_AllOff();
		if(netState.Net_Insert[0]==1 && netState.Net_Insert[2]==1)//左舷右舷网位仪都在
		{
				LeftRightOffsetValue = ((netparam.left_x*1000)+netparam.right_x);
				LeftRightOffsetDisplay(LeftRightOffsetValue);
		}
		else if(netState.Net_Insert[0]==1 || netState.Net_Insert[2]==1)
		{
			if(netState.Net_Insert[0]==1)                  //左舷在，右舷不在
			{
				LeftOffsetDisplay(netparam.left_x);
			}
			else if(netState.Net_Insert[2] == 1) 															                 //右舷在，左舷不在
			{
				RightOffsetDisplay(netparam.right_x);
			}
		}
		
		if(netState.Net_Insert[1] == 1)                   //网尾网位仪在
		{
			BackOffsetDisplay(netparam.tail_y);
		}
}

/************************************************************
 * 函数名：LeftRightOffsetDisplay
 * 描述  ：左舷偏移显示
 * 输入  ：Num:显示的数(0~9)
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
//左舷右舷设置数码管显示
void LeftRightOffsetDisplay(u32 Num)
{
	char left_units,left_tens,left_hundreds; //左舷三位
	char right_units,right_tens,right_hundreds; //右舷三位
	
	left_units = (Num/1000)%10; //个
	left_tens = (Num/10000)%10; //十
	left_hundreds = Num/100000; //百
	
	right_units = Num%10;
	right_tens = (Num/10)%10;
	right_hundreds = (Num/100)%10;
	
	Uart_SendData(USART2,0x08);
	SendDataNum(USART2,left_hundreds);
	SendDataNum(USART2,left_tens);
	SendDataNum(USART2,left_units);
	SendDataNum(USART2,right_hundreds);
	SendDataNum(USART2,right_tens);
	SendDataNum(USART2,right_units);
	delay_ms(6);
	Uart_SendData(USART2,0x18);//显示命令
	if(State == 1)//写码状态
		Uart_SendData(USART2,0xFF);//高亮
	else //非写码状态
		Uart_SendData(USART2,0xF9);//低亮
}


/************************************************************
 * 函数名：LeftOffsetDisplay
 * 描述  ：左舷偏移显示
 * 输入  ：Num:显示的数(0~9)
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
//左舷设置数码管显示
void LeftOffsetDisplay(u16 Num)
{
	char units,tens,hundreds;
	units = Num%10;
	tens = (Num/10)%10;
	hundreds = Num/100;
	
	Uart_SendData(USART2,0x08);
	SendDataNum(USART2,hundreds);
	SendDataNum(USART2,tens);
	SendDataNum(USART2,units);
	SendDataNum(USART2,10);
	SendDataNum(USART2,10);
	SendDataNum(USART2,10);
	delay_ms(6);
	Uart_SendData(USART2,0x18);//显示命令
	if(State == 1)//写码状态
		Uart_SendData(USART2,0xFF);//高亮
	else //非写码状态
		Uart_SendData(USART2,0xF9);//低亮
}

/************************************************************
 * 函数名：RightOffsetDisplay
 * 描述  ：右舷设置数码管显示
 * 输入  ：Num:显示的数(0~9)
 * 输出  ：无
 * 调用  ：
 ************************************************************/
void RightOffsetDisplay(u16 Num)
{
	char units,tens,hundreds;
	units = Num%10;
	tens = (Num/10)%10;
	hundreds = Num/100;
	
	Uart_SendData(USART2,0x08);
	SendDataNum(USART2,10);
	SendDataNum(USART2,10);
	SendDataNum(USART2,10);
	SendDataNum(USART2,hundreds);
	SendDataNum(USART2,tens);
	SendDataNum(USART2,units);
	delay_ms(6);
	Uart_SendData(USART2,0x18);//显示命令
	if(State == 1)//写码状态
		Uart_SendData(USART2,0xFF);//高亮
	else //非写码状态
		Uart_SendData(USART2,0xF9);//低亮
}

/************************************************************
 * 函数名：BackOffsetDisplay
 * 描述  ：后方偏移设置数码管显示
 * 输入  ：Num:显示的数(0~9)
 * 输出  ：无
 * 调用  ：
 ************************************************************/
void BackOffsetDisplay(u16 Num)
{
	char units,tens,hundreds,thousands;
	units = Num%10;
	tens = (Num/10)%10;
	hundreds = (Num/100)%10;
	thousands = Num/1000;
	
	Uart_SendData(USART3,0x08);
 SendDataNum(USART3,thousands);
	SendDataNum(USART3,hundreds);
	SendDataNum(USART3,tens);
	SendDataNum(USART3,units);
	delay_ms(6);
	Uart_SendData(USART3,0x18);//显示命令
	if(State == 1)//写码状态
		Uart_SendData(USART3,0xFF);//高亮
	else //非写码状态
		Uart_SendData(USART3,0xF9);//低亮
		
}

/************************************************************
 * 函数名：NixieTube_LeftRightNoDis
 * 描述  ：左舷右舷数码管不显示
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void NixieTube_LeftRightOff(void)
{
	Uart_SendData(USART2,0x18);//显示命令
	Uart_SendData(USART2,0x00);//
}

/************************************************************
 * 函数名：NixieTube_AllOff
 * 描述  ：数码管全都不显示
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void NixieTube_AllOff(void)
{
	Uart_SendData(USART2,0x18);//显示命令
	Uart_SendData(USART2,0x00);//
	
	Uart_SendData(USART3,0x18);//显示命令
	Uart_SendData(USART3,0x00);//
	
	delay_ms(5);
}

/************************************************************
 * 函数名：NixieTubeDisplay
 * 描述  ：数码管显示
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void NixieTubeDisplay()
{
	if(State)//处于写码状态
	{
		NixieTube_AllOff();
		switch(netState.Net_Connet)
		{
			case 1://左舷
								if(netState.Net_Insert[0]==0) //当前写码的网位仪被拔出
								{
									State = 0;
									DisableEncode();
									if(netState.Net_Insert[1]==1) //船尾网位仪插入
										BackOffsetDisplay(netparam.tail_y);
									if(netState.Net_Insert[2]==1) //右舷网位仪插入
										RightOffsetDisplay(netparam.right_y);
								}
								else if(netState.Net_Insert[0] == 1) //网位仪已插入
								{
									LeftOffsetDisplay(netparam.left_x);
									BackOffsetDisplay(netparam.left_y);
									
									//启用编码器
									NixieTub_three(netparam.left_x);
									NixieTub_four(netparam.left_y);
								}
								break;
			
			case 2://网尾
								if(netState.Net_Insert[1]==0)//当前写码的网位仪被拔出
								{
									State = 0;
									DisableEncode();
									if(netState.Net_Insert[0]==1 && netState.Net_Insert[2]==1) //两个都插入
									{
										LeftRightOffsetValue = ((netparam.left_x*1000)+netparam.right_x);
										LeftRightOffsetDisplay(LeftRightOffsetValue);
									}
									else if(netState.Net_Insert[0]==1)//只有左舷插入
									{
										LeftOffsetDisplay(netparam.left_x);
									}
									else if(netState.Net_Insert[2]==1) //只有右舷插入
									{
										RightOffsetDisplay(netparam.right_x);
									}
								}
								else if(netState.Net_Insert[1]==1)//网位仪已插入
								{
									BackOffsetDisplay(netparam.tail_y);
									
									//启用编码器
									NixieTub_four(netparam.tail_y);
								}
								break;
			
			case 3://右舷
								if(netState.Net_Insert[2]==0)//当前写码的网位仪被拔出
								{
									State = 0;
									DisableEncode();
									if(netState.Net_Insert[0]==1)//左舷网位仪已插入
										LeftOffsetDisplay(netparam.left_x);
									if(netState.Net_Insert[1]==1)//网尾网位仪已插入
										BackOffsetDisplay(netparam.tail_y);
								}
								else if(netState.Net_Insert[2]==1)//网位仪已插入
								{
									//数码管显示
									BackOffsetDisplay(netparam.right_y);
									RightOffsetDisplay(netparam.right_x);
									
									//启用编码器
									NixieTub_four(netparam.right_y);
									NixieTub_three(netparam.right_x);
								}
								break;
			default:
								break;
		}
	}
	else //非写码状态下 //
	{
		NixieTubeLowLight();
	}
}

/************************************************************
 * 函数名：updateLedState
 * 描述  ：更新灯状态
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void updateLedState()
{
	AllLedOff();
	if(State==0) //无写码状态
	{
		LedState();
	}
	else //写码状态
	{
		LedState();
		switch(netState.Net_Connet)
		{
			case 1:
								Led_NetLeft_GreenOff;
								Led_NetLeft_RedOn;
								break;
			
			case 2:
								Led_NetTail_GreenOff;
								Led_NetTail_RedOn;
								break;			
			
			case 3:
								Led_NetRight_GreenOff;
								Led_NetRight_RedOn;
								break;
		}
	}
}

void LedState(void)
{
		if(netState.Net_Insert[0] == 1)//左舷插入
		{
			Led_NetLeft_GreenOn;				
			Led_NetLeft_RedOff;
		}
		else if(netState.Net_Insert[0] == 0)//左舷拔出
		{
			Led_NetLeft_GreenOff;
			Led_NetLeft_RedOff;
		}
		
		if(netState.Net_Insert[1]==1)//网尾插入
		{
			Led_NetTail_GreenOn;	
			Led_NetTail_RedOff;
		}
		else if(netState.Net_Insert[1]==0) //网尾拔出
		{  
			Led_NetTail_GreenOff;
			Led_NetTail_RedOff;
		}
		
		if(netState.Net_Insert[2] == 1)//右舷插入
		{
			Led_NetRight_GreenOn;
			Led_NetRight_RedOff;
		}
		else if(netState.Net_Insert[2] == 0)//右舷拔出
		{
			Led_NetRight_GreenOff;
			Led_NetRight_RedOff;
		}
}

/************************************************************
 * 函数名：NixieTube_task
 * 描述  ：数码管显示任务
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void NixieTube_task(void *pdata)
{
	while(1)
	{
		delay_ms(50);
		if(Nixie.Display)
		{
			Nixie.Display = 0;
			NixieTubeDisplay();
			updateLedState();
		}
		
		if(Nixie.valueChange)
		{
			Nixie.valueChange = 0;
			if(netState.Net_Connet == 1)
			{
				LeftOffsetDisplay(LeftRightOffsetValue);
				BackOffsetDisplay(BackOffsetValue);
			}
			else if(netState.Net_Connet == 2)
				BackOffsetDisplay(BackOffsetValue);
			else if(netState.Net_Connet == 3)
			{
				RightOffsetDisplay(LeftRightOffsetValue);
				BackOffsetDisplay(BackOffsetValue);
			}
		}
	}
}
