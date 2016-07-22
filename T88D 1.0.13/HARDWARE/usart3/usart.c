#include "sys.h"
#include "usart.h"	  

static u8 Uart1_Rx_Num = 0;
static u8 ReceiveSta = 0;
static u16 crcdata,scrData; //У�����ɵ�����, ԴУ�������
static u8 NetSta[3];//t800�����״̬
static u8 err;

u8 rx1buf[UART_RX1_LEN];  //����1���ջ��� 0:֡ͷ 0x24; 1: ����(0x01:���, 0x31:ע��, 0x32:��ȡ);\
                            2~3:��ƫ��ֵ; 4:�������ж�(0:����,1:����); 5~6: ������ƫ��ֵ;

u8 tx1buf[UART_RX1_LEN]; //����1���ͻ��� 0:֡ͷ 0x24; 1: ����(0x01:���,  0x31:ע�룬0x32��ȡ);\
                      2~3:��ƫ��ֵ; 4:�������ж�(0:����,1:����); 5~6: ������ƫ��ֵ;\
																						7: ѡ��Ҫд���ʾλ��(2:��β,1:����,3:����)

 /***********************************************************
 * ������: USART1_ModeInit
 * ����  ��������9600���ֳ�8λ��һλֹͣλ������żУ��λ��
	          �շ�ģʽ
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_ModeInit(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
}

 /***********************************************************
 * ������: USART1_GPIOInit
 * ����  �����ô���1GPIO
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX Control
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //RX Control
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //485
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	Usart1_Receive;
}

 /***********************************************************
 * ������: USART1_NVIC_Configuration
 * ����  �����ô���1�ж�
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure; 
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

 /***********************************************************
 * ������: UART1_DMA_Configuration
 * ����  �����ô���1 DMA����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void UART1_DMA_Configuration(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //����DMAʱ��  

		DMA_DeInit(DMA1_Channel5); 	//USART3 DMA1ͨ��3����   
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);//�����ַ  
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rx1buf;     //�ڴ��ַ  
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma���䷽������赽�ڴ� 
		DMA_InitStructure.DMA_BufferSize = UART_RX1_LEN;               //����DMA�ڴ���ʱ�������ĳ���  
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA�������ַ���� 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //����DMA���ڴ����ģʽ  
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ� 8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //�ڴ������ֳ�8λ   
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //����DMA�Ĵ���ģʽ   
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //����DMA�����ȼ��� ���
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //����DMA��2��memory�еı����������  
		DMA_Init(DMA1_Channel5,&DMA_InitStructure);  

		DMA_Cmd(DMA1_Channel5,ENABLE);
}

 /***********************************************************
 * ������: USART1_Config
 * ����  �����ô���3
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	UART1_DMA_Configuration();
	USART1_NVIC_Configuration();
	USART1_GPIOInit();
	USART1_ModeInit();
	
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //��ֹ 
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	//����DMA��ʽ���� 
 USART_Cmd(USART1, ENABLE);
	
}

 /***********************************************************
 * ������: USART1_IRQHandler
 * ����  �����ô���1�жϴ���
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_IRQHandler(void)
{   
	u32 Length = 0;
	u16 crcdata,scrData; //У�����ɵ�����, ԴУ�������
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{  
		DMA_Cmd(DMA1_Channel5,DISABLE); 
		Length = USART1->SR;  
		Length = USART1->DR; //��USART_IT_IDLE��־  
		
		scrData = (rx1buf[16]<<8)+rx1buf[17];
		crcdata = msg_crc(rx1buf,16); //����У��
		
		if(crcdata == scrData)
		{
			GetCom1Data();
			Pendfault();
			OSMboxPost(msg_uart,(void*)rx1buf);//������Ϣ
		}
		
		DMA1_Channel5->CNDTR = UART_RX1_LEN;//��װ��,���ý��յ�ַƫַ��0��ʼ
		DMA_Cmd(DMA1_Channel5, ENABLE);//������,�ؿ�DMA   
	}
	__nop();   	
	
}

 /***********************************************************
 * ������: NetDetecting
 * ����  �����T800�����γ������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void NetDetecting(void)
{
	switch(NetSta[0])//����״̬
	{
		case 0://δ����
							if(netState.Net_Insert[0]!=0)
							{
								netState.Net_Insert[0] = 0;
								Nixie.Display = 1;
							}
							break;
		
		case 1://����
							if(netState.Net_Insert[0]==0)
							{
								netState.Net_Insert[0] = 1;
								nettemp = 1;
								OffSetInit();
							}
							break;
		
		case 2://����
							if(netState.Net_Insert[0]!=2)
							{
								netState.Net_Insert[0] = 2;
								Nixie.Display = 1;
							}
							break;
		default:
							break;
	}
	
	switch (NetSta[1])//��β
	{
		case 0://δ����
							if(netState.Net_Insert[1]!=0)
							{
								netState.Net_Insert[1] = 0;
								Nixie.Display = 1;
							}
							break;
		
		case 1://����
							if(netState.Net_Insert[1]==0)
							{
								netState.Net_Insert[1] = 1;
								nettemp = 2;
								OffSetInit();
							}
							break;
		
		case 2://����
							if(netState.Net_Insert[1]!=2)
							{
								netState.Net_Insert[1] = 2;
								Nixie.Display = 1;
							}
							break;
		default:
							break;		
	}
	
	switch(NetSta[2])//����
	{
		case 0://δ����
							if(netState.Net_Insert[2]!=0)
							{
								netState.Net_Insert[2] = 0;
								Nixie.Display = 1;
							}
							break;
		
		case 1://����
							if(netState.Net_Insert[2]==0)
							{
								netState.Net_Insert[2] = 1;
								nettemp = 3;
								OffSetInit();
							}
							break;
		
		case 2://����
							if(netState.Net_Insert[2]!=2)
							{
								netState.Net_Insert[2] = 2;
								Nixie.Display = 1;
							}
							break;
		default:
							break;	
	}
}

 /***********************************************************
 * ������: GetCom1Data
 * ����  �����ô���1��ȡ���ݼ�����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void GetCom1Data(void)
{
	if(rx1buf[0]==0x24) //֡ͷ�ж�
	{
		switch (rx1buf[1])
		{
			case 0x51:// ��������
								{
									Usart_flag = 1; //���ڿ���״̬
									OSMboxPost(msg_hand,(void*)rx1buf);//������Ϣ
									//��λ��״̬ 0��δ���� 1������ 2������
									NetSta[0] = rx1buf[14]>>6; //����״̬
									NetSta[1] = (rx1buf[14]>>4)&0x03; //��β״̬
									NetSta[2] = (rx1buf[14]>>2)&0x03; //����״̬
									NetDetecting();
								}
								break;
			
			case 0x17: //�򿪴���
								{
									if(netState.fault)//ѡ���Ϊ������λ��
									{
											CloseSerial();
											netState.fault = 0;
									}
									else
									{
										State = 1; //����д��״̬
										SilentTime = 0;
										netState.Net_Connet = netState.Net_Sel;
										netState.Net_Sel = 0;
										Usart_flag = 1; //���ڿ���״̬
										//����ܸ�ֵ
										switch (netState.Net_Connet)
										{
											case 1://������λ��
																{
																	LeftRightOffsetValue = netparam.left_x;
																	BackOffsetValue = netparam.left_y;
																}
																break;
																
											case 2:
																{
																	BackOffsetValue = netparam.tail_y;
																}
																break;
																
											case 3:
																{
																	LeftRightOffsetValue = netparam.right_x;
																	BackOffsetValue = netparam.right_y;
																}
																break;
										}
										Nixie.Display = 1;										
									}
								}
				    break;
			
			case 0x18: //�رմ���
								{
									//����ʱע��ɹ�
									if(SendCnt!=0)
									{
										OSMboxPost(msg_receive,(void*)rx1buf);
										OSMboxPend(msg_uart,1,&err);
									}
									else
									{
										State = 0; //�˳�д��״̬
										DisableEncode();
										Nixie.Display = 1;
										netState.Net_Connet = 0;
									}
									
									if(netState.Net_Sel) //��һ����λ�ǵ�д��״̬��������һ��
									{
										OpenSerial();
									}
									else
									{
										Usart_flag = 1; //���ڿ���״̬
									}
								}
								break;
			
			case 0x31: //ע��ɹ�
			    {
								CloseSerial();
								//�������
								WriteFlash_Dis();
			    }
				   break;
			
			default:
				   break;
		}
	}
}

/***********************************************************
 * ������: Com1SendData
 * ����  ����T81��������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Com1SendData()
{
	u8 i;
	u16 data;
	
	data = msg_crc(tx1buf,16);	//CRCУ����������
	tx1buf[16] = data>>8;
	tx1buf[17] = data;
	
	Usart1_OutPut;
	delay_ms(5);
	for(i=0;i<18;i++)	
	{
		USART_SendData(USART1, tx1buf[i]);
		while (!(USART1->SR & USART_FLAG_TXE));
	}
	delay_ms(3);
	Usart1_Receive;
}

//���ڷ�������
void usartsend_task(void *pdata)
{
	u8 *s;
	u8 *rxbuf;
	INT8U err;
	while(1)
	{
		s = OSQPend(q_msg,0,&err);
		if(err==OS_ERR_NONE)
		{
			Usart_flag = 0;
			Postfault();
			Com1SendData();
			OSMboxPend(msg_uart,200,&err);//�ȴ�����
			if(err==OS_ERR_NONE)
			{
				Usart_flag = 1; //�ͷŴ���
			}
			else if(err == OS_ERR_TIMEOUT)
			{
				Usart_flag = 1;
				netState.fault = 0;
				netState.Net_Sel = 0;
			}
		}
		OSTimeDlyHMSM(0, 0,0,10);
	}
}


//���������ж�
void Postfault()
{
	switch (tx1buf[7])
	{
		case 1:
							faultCnt[0]++;
							if(faultCnt[0]>=4)
							{
								faultCnt[0] = 0;
								netState.Net_Insert[0] = 2; //�����������
								Nixie.Display = 1;
							}
							break;
		case 2:
							faultCnt[1]++;
							if(faultCnt[1]>=4)
							{
								faultCnt[1] = 0;
								netState.Net_Insert[1] = 2; //��β�������
								Nixie.Display = 1;
							}		
							break;
		case 3:
							faultCnt[2]++;
							if(faultCnt[2]>=4)
							{
								faultCnt[2] = 0;
								netState.Net_Insert[2] = 2; //�����������
								Nixie.Display = 1;
							}		
							break;
		default:
							break;
	}
}

void Pendfault()
{
	switch (tx1buf[7])
	{
		case 1:
			    if(netState.Net_Insert[0]==2)
							{
								netState.Net_Insert[0] = 1;
								Nixie.Display = 1;
							}
							faultCnt[0]=0;
							break;
		case 2:
							if(netState.Net_Insert[1]==2)
							{
								netState.Net_Insert[1] = 1;
								Nixie.Display = 1;
							}
							faultCnt[1]=0;	
							break;
		case 3:
							if(netState.Net_Insert[2]==2)
							{
								netState.Net_Insert[2] = 1;
								Nixie.Display = 1;
							}
							faultCnt[2]=0;
							break;
		default:
							break;
	}
}
