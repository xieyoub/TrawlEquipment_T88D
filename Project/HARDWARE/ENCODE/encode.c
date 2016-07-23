#include "encode.h"

/************************************************************
 * ��������TIME2_Encode
 * ����  ����ʱ��2������ģʽ
 * ����  ����
 * ���  ����
 * ����  ����Encode_Init����
 ************************************************************/
void TIME2_Encode()
{
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA0 PA1��������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);     
	
	//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = 1500; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =0; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM2_ICInitStructure);//���ṹ���е�����ȱʡ����
 TIM2_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM2
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
//	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
//	
// TIM_Cmd(TIM2,ENABLE); 	//ʹ�ܶ�ʱ��2
}

/************************************************************
 * ��������TIME3_Encode
 * ����  ����ʱ��3������ģʽ
 * ����  ����
 * ���  ����
 * ����  ����Encode_Init����
 ************************************************************/
void TIME3_Encode()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA6 PA7��������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 1500;  //�趨��������װֵ   TIMx_ARR = 30*4
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
															
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
 TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	//TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);//
	//Reset counter

	//TIM_Cmd(TIM3, ENABLE);   //����TIM3��ʱ��
}

/************************************************************
 * ��������TIM2_IRQHandler
 * ����  ����ʱ��2�ж�
           ����������λ�����
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET)
	{
		SilentTime = 1;
		
		while((TIM2->CNT)%4)  //��֤ÿ�μ�������4�ı���,ÿšһ������ֵʵ���ϼ�4(cnt)
		{
			TIM2->CNT++;
		}
		
		LeftRightOffsetValue = ((TIM2->CNT)/4)*5;  //, ÿ����5
		if(LeftRightOffsetValue>=599 && LeftRightOffsetValue<=1000)//���599
		{
			LeftRightOffsetValue = 599;
			TIM2->CNT = 480;
		}
		
		if(LeftRightOffsetValue >1000) //��С0
		{
			LeftRightOffsetValue = 0;
			TIM2->CNT = 0;
		}
		Nixie.valueChange = 1;
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
	}
}

/************************************************************
 * ��������TIM3_IRQHandler
 * ����  ����ʱ��3�ж�
           ����һ����λ�����
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3,TIM_IT_CC1) != RESET)
	{
		SilentTime = 1;
		while((TIM3->CNT)%4)  //��֤ÿ�μ�������4�ı���,ÿšһ������ֵʵ���ϼ�4(cnt)
		{
			TIM3->CNT++;
		}
		
		BackOffsetValue = ((TIM3->CNT)/4)*20;  //, ÿ����20
		if(BackOffsetValue>=3000 && BackOffsetValue<=4500) //���3000
		{
			BackOffsetValue = 3000;
			TIM3->CNT = 600;
		}
		
		if(BackOffsetValue >4500) //��С0
		{
			BackOffsetValue = 0;
			TIM3->CNT = 0;
		}
		Nixie.valueChange = 1;
		TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
	}
}

/************************************************************
 * ��������Encode_Init
 * ����  ��������GPIO����
 * ����  ����
 * ���  ����
 * ����  ����main����
 ************************************************************/
void Encode_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);//ʹ��TIM2/3ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	
	TIME2_Encode();
	TIME3_Encode();
}

/************************************************************
 * ��������NixieTub_three
 * ����  ����λ���������
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void NixieTub_three(u16 Num)
{
		TIM_Cmd(TIM2,ENABLE);
		TIM2->CNT = (Num/5)*4;
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);		
}


/************************************************************
 * ��������NixieTub_four
 * ����  ����λ���������
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void NixieTub_four(u16 Num)
{
		TIM_Cmd(TIM3,ENABLE);  
		TIM3->CNT = (Num/20)*4;
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
}

/************************************************************
 * ��������DisableEncode
 * ����  �����ñ�����
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void DisableEncode()
{
		TIM_Cmd(TIM2,DISABLE);
		TIM_Cmd(TIM3,DISABLE);
		TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
		TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
}
