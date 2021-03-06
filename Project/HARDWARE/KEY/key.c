#include "key.h"

/************************************************************
 * 函数名：Key_Init
 * 描述  ：按键GPIO配置,确定：PB0，网尾：PB2，左：PB12，右：PB13
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由EXTIX_Init调用
 ************************************************************/
void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/************************************************************
 * 函数名：key_task
 * 描述  ：按键扫描任务
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void key_task(void *pdata)
{																
	u8 keyValue = 0;
	static char keyStatus = 0; //按键状态
	static char keyValueBackup=0;
	static char i = 0;
	while(1)
	{
		OSTimeDlyHMSM(0, 0,0,5);
		keyValue = Get_KeyValue();
		
		switch(keyStatus)
		{
			case KEY_STAT_noKeyPress:
			     if(keyValue != NO_KEY_PRESS)
								{
									if(++i==2)
									{
										keyValueBackup = keyValue;
										keyStatus = KEY_STAT_filter;
										i=0;
									}
								}
								break;
		
			case KEY_PRESS:
								if(keyValue == NO_KEY_PRESS)
								{
									keyStatus = KEY_STAT_noKeyPress;
								}
								break;
							
			case KEY_STAT_filter:
			{
				switch(keyValueBackup)
				{
					case 1: //网尾
										if(Usart_flag==1)
										{
											if(netState.Net_Insert[1]==1)//插入
											{
												if(netState.Net_Connet!=2)//屏蔽再次按此按键
												{
													SilentTime=0;
													netState.Net_Connet = 2;
													State = 1;
													BackOffsetValue = netparam.tail_y;
													Nixie.Display = 1;
												}
											}
											else if(netState.Net_Insert[1]==2)//故障
											{
												
											}
										}
										break;
					
					case 2://左舷
										if(Usart_flag==1)
										{
											if(netState.Net_Insert[0]==1)//插入
												{
													if(netState.Net_Connet!=1)//屏蔽再次按此按键
													{
														SilentTime=0;
														netState.Net_Connet = 1;
														State = 1;
														LeftRightOffsetValue = netparam.left_x;
														BackOffsetValue = netparam.left_y;
														Nixie.Display = 1;
													}
												}
												else if(netState.Net_Insert[0]==2)//故障
												{
													
												}
										}
										break;
						
					case 3://右舷
										if(Usart_flag==1)
										{
											if(netState.Net_Insert[2]==1)//插入
											{
												if(netState.Net_Connet!=3)//屏蔽再次按此按键
												{
													SilentTime=0;
													netState.Net_Connet = 3;
													State = 1;
													LeftRightOffsetValue = netparam.right_x;
													BackOffsetValue = netparam.right_y;
													Nixie.Display = 1;
												}
											}
											else if(netState.Net_Insert[2]==2)//故障
											{
												
											}
										}
										break;
						
					case 4://确定
										if(State == 1)   //处于写码状态
										{
											SilentTime=0;
											switch (netState.Net_Connet)
											{
												case 2: //网尾
																	{
																		netparam.tail_x = 0;
																		netparam.tail_y = BackOffsetValue;
																		WriteFlash_param();
																		OffSetWrite();
																	}
																	break;
																	
												case 1:
																	{
																		netparam.left_x = LeftRightOffsetValue;
																		netparam.left_y = BackOffsetValue;
																		WriteFlash_param();
																		OffSetWrite();
																	}
												case 3:
																	{
																		netparam.right_x = LeftRightOffsetValue;
																		netparam.right_y = BackOffsetValue;
																		WriteFlash_param();
																		OffSetWrite();
																	}
																	break;
												default:
																	break;					
											}
										}
										break;
					default:
										break;
				}
				keyStatus = KEY_PRESS;
			}
		}
	}
}

/************************************************************
 * 函数名：Get_KeyValue
 * 描述  ：获取按键值
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
u8 Get_KeyValue(void)
{
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2))
		return 1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
		return 2;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))
		return 3;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
		return 4;
	return 0;
}

