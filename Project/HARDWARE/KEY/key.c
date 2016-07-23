#include "key.h"

/************************************************************
 * ��������Key_Init
 * ����  ������GPIO����,ȷ����PB0����β��PB2����PB12���ң�PB13
 * ����  ����
 * ���  ����
 * ����  ����EXTIX_Init����
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
 * ��������key_task
 * ����  ������ɨ������
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void key_task(void *pdata)
{																
	u8 keyValue = 0;
	static char keyStatus = 0; //����״̬
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
					case 1: //��β
										if(Usart_flag==1)
										{
												if(netState.Net_Insert[1]!=0)//ʾλ���Ѳ���,�������ϵ�
												{
													if(State) //������д��״̬
													{
														if(netState.Net_Connet != 2)//�����ٴΰ��˰���
														{
															if(netState.Net_Insert[1]==1) //ѡ��ĸ���λ��״̬Ϊ����
															{
																netState.Net_Sel = 2;
																CloseSerial();
															}
															else //ѡ��ĸ���λ��Ϊ���ϵ���λ��
															{
																netState.Net_Sel = 2;
																OpenSerial();
																netState.fault = 1;	
															}
														}
													}
													else//��д��״̬
													{
														if(netState.Net_Insert[1]==1)//����λ������
														{
															netState.Net_Sel = 2;
															OpenSerial();
														}
														else//����λ�ǹ���
														{
															netState.Net_Sel = 2;
															OpenSerial();
															netState.fault = 1;					
														}
													}
												}					
										}
										break;
					
					case 2://����
										if(Usart_flag==1)
										{
											if(netState.Net_Insert[0]!=0)//ʾλ���Ѳ���,�������ϵ�
											{
												if(State) //������д��״̬
												{
													if(netState.Net_Connet != 1)//�����ٴ�ѡ�д˰���
													{
														if(netState.Net_Insert[0]==1) //ѡ��ĸ���λ��״̬Ϊ����
														{
															netState.Net_Sel = 1;
															CloseSerial();
														}
														else //ѡ��ĸ���λ��Ϊ���ϵ���λ��
														{
														netState.Net_Sel = 1;
														OpenSerial();
														netState.fault = 1;	
														}
													}
												}
												else//��д��״̬
												{
													if(netState.Net_Insert[0]==1)//����λ������
													{
														netState.Net_Sel = 1;
														OpenSerial();
													}
													else//����λ�ǹ���
													{
														netState.Net_Sel = 1;
														OpenSerial();
														netState.fault = 1;					
													}
												}
											}
										}
										break;
						
					case 3://����
										if(Usart_flag==1)
										{
											if(netState.Net_Insert[2]!=0)//ʾλ���Ѳ���,�������ϵ�
											{
												if(State)//������д��״̬
												{
													if(	netState.Net_Connet != 3)//�����ٴ�ѡ�д˰���
													{
														if(netState.Net_Insert[2]==1) //ѡ��ĸ���λ��״̬Ϊ����
														{
															netState.Net_Sel = 3;
															CloseSerial();
														}
														else //ѡ��ĸ���λ��Ϊ���ϵ���λ��
														{
														netState.Net_Sel = 3;
														OpenSerial();
														netState.fault = 1;	
														}
													}
												}
												else //��д��״̬
												{
													if(netState.Net_Insert[2]==1)//����λ������
													{
														netState.Net_Sel = 3;
														OpenSerial();
													}
													else//����λ�ǹ���
													{
														netState.Net_Sel = 3;
														OpenSerial();
														netState.fault = 1;				
													}
												}
											}
										}
										break;
						
					case 4://ȷ��
										if(State == 1)   //����д��״̬
										{
											SilentTime=0;
											switch (netState.Net_Connet)
											{
												case 2: //��β
																	{
																		LeftRightOffsetValue = 0;
																		OffSetWrite();
																	}
																	break;
																	
												case 1:
												case 3:
																	OffSetWrite();
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
 * ��������Get_KeyValue
 * ����  ����ȡ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
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

