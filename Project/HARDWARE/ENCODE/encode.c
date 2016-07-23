#include "encode.h"

/************************************************************
 * 函数名：TIME2_Encode
 * 描述  ：定时器2编码器模式
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由Encode_Init调用
 ************************************************************/
void TIME2_Encode()
{
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA0 PA1浮空输入	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);     
	
	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = 1500; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =0; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
	TIM_ICStructInit(&TIM2_ICInitStructure);//将结构体中的内容缺省输入
 TIM2_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器 
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM2
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
//	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
//	
// TIM_Cmd(TIM2,ENABLE); 	//使能定时器2
}

/************************************************************
 * 函数名：TIME3_Encode
 * 描述  ：定时器3编码器模式
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由Encode_Init调用
 ************************************************************/
void TIME3_Encode()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA6 PA7浮空输入	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 1500;  //设定计数器重装值   TIMx_ARR = 30*4
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
															
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
	TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
 TIM_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器 
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

	//TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);//
	//Reset counter

	//TIM_Cmd(TIM3, ENABLE);   //启动TIM3定时器
}

/************************************************************
 * 函数名：TIM2_IRQHandler
 * 描述  ：定时器2中断
           调节两个三位数码管
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET)
	{
		SilentTime = 1;
		
		while((TIM2->CNT)%4)  //保证每次计数都是4的倍数,每拧一档计数值实际上加4(cnt)
		{
			TIM2->CNT++;
		}
		
		LeftRightOffsetValue = ((TIM2->CNT)/4)*5;  //, 每档加5
		if(LeftRightOffsetValue>=599 && LeftRightOffsetValue<=1000)//最大599
		{
			LeftRightOffsetValue = 599;
			TIM2->CNT = 480;
		}
		
		if(LeftRightOffsetValue >1000) //最小0
		{
			LeftRightOffsetValue = 0;
			TIM2->CNT = 0;
		}
		Nixie.valueChange = 1;
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
	}
}

/************************************************************
 * 函数名：TIM3_IRQHandler
 * 描述  ：定时器3中断
           调节一个四位数码管
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3,TIM_IT_CC1) != RESET)
	{
		SilentTime = 1;
		while((TIM3->CNT)%4)  //保证每次计数都是4的倍数,每拧一档计数值实际上加4(cnt)
		{
			TIM3->CNT++;
		}
		
		BackOffsetValue = ((TIM3->CNT)/4)*20;  //, 每档加20
		if(BackOffsetValue>=3000 && BackOffsetValue<=4500) //最大3000
		{
			BackOffsetValue = 3000;
			TIM3->CNT = 600;
		}
		
		if(BackOffsetValue >4500) //最小0
		{
			BackOffsetValue = 0;
			TIM3->CNT = 0;
		}
		Nixie.valueChange = 1;
		TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
	}
}

/************************************************************
 * 函数名：Encode_Init
 * 描述  ：编码器GPIO配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由main调用
 ************************************************************/
void Encode_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);//使能TIM2/3时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
	
	TIME2_Encode();
	TIME3_Encode();
}

/************************************************************
 * 函数名：NixieTub_three
 * 描述  ：三位数码管启用
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void NixieTub_three(u16 Num)
{
		TIM_Cmd(TIM2,ENABLE);
		TIM2->CNT = (Num/5)*4;
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);		
}


/************************************************************
 * 函数名：NixieTub_four
 * 描述  ：四位数码管启用
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void NixieTub_four(u16 Num)
{
		TIM_Cmd(TIM3,ENABLE);  
		TIM3->CNT = (Num/20)*4;
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
}

/************************************************************
 * 函数名：DisableEncode
 * 描述  ：禁用编码器
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void DisableEncode()
{
		TIM_Cmd(TIM2,DISABLE);
		TIM_Cmd(TIM3,DISABLE);
		TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
		TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
}
