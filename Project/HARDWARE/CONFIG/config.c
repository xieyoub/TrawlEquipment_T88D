#include "config.h"

static u8 tim2flag = 0;
static INT8U err;
static u8 Netbuf[3][18];
NixieTub Nixie;

/************************************************************
 * 函数名：updateOffset
 * 描述  ：更新参数
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void updateOffset()
{
	switch (netState.Net_Connet)
	{
		case 1:
		    	netparam.left_x = LeftRightOffsetValue;
			    netparam.left_y = BackOffsetValue;
							break;
		
		case 2:
			    netparam.tail_x = 0;
							netparam.tail_y = BackOffsetValue;
							break;
		
		case 3:
							netparam.right_x = LeftRightOffsetValue;
							netparam.right_y = BackOffsetValue;
							break;
		default:
							break;
	}
}

/************************************************************
 * 函数名：OffSetWrite
 * 描述  ：注入参数
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OffSetWrite()
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	tx1buf[2] = BackOffsetValue>>8;
	tx1buf[3] = BackOffsetValue;
	updateOffset();
	
	if(netState.Net_Connet==1 || LeftRightOffsetValue==0) //左舷
		tx1buf[4] = 0;
	else 
		tx1buf[4] = 1;
	
	tx1buf[5] = LeftRightOffsetValue>>8;
	tx1buf[6] = LeftRightOffsetValue;
	
	tx1buf[7] = netState.Net_Connet;
	for(i=8;i<18;i++)
		tx1buf[i] = 0;
	
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：OffSetInit
 * 描述  ：参数初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OffSetInit()
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	switch (nettemp)
	{
		case 1://左舷
		{
			tx1buf[2] = netparam.left_y>>8;
			tx1buf[3] = netparam.left_y;
			tx1buf[4] = 0;
			tx1buf[5] = netparam.left_x>>8;
			tx1buf[6] = netparam.left_x;
			tx1buf[7] = 1;
			tx1buf[8] = 1; //第一次注入
			for(i=0;i<18;i++)
			 Netbuf[0][i] = tx1buf[i];
			OSQPost(msg_write,(void*)Netbuf[0]);
		}
			break;
		
		case 2://网尾
		{
			tx1buf[2] = netparam.tail_y>>8;
			tx1buf[3] = netparam.tail_y;
			tx1buf[4] = 0;
			tx1buf[5] = netparam.tail_x>>8;
			tx1buf[6] = netparam.tail_x;
			tx1buf[7] = 2;
			tx1buf[8] = 1; //第一次注入
			for(i=0;i<18;i++)
			 Netbuf[1][i] = tx1buf[i];
			OSQPost(msg_write,(void*)Netbuf[1]);
		}
			break;
		
		case 3://右舷
		{
			tx1buf[2] = netparam.right_y>>8;
			tx1buf[3] = netparam.right_y;
			tx1buf[4] = 1;
			tx1buf[5] = netparam.right_x>>8;
			tx1buf[6] = netparam.right_x;
			tx1buf[7] = 3;
			tx1buf[8] = 1; //第一次注入
			for(i=0;i<18;i++)
			 Netbuf[2][i] = tx1buf[i];
			OSQPost(msg_write,(void*)Netbuf[2]);
		}
			break;
	}
}

/************************************************************
 * 函数名：OpenSerial
 * 描述  ：打开串口命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OpenSerial()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x17;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：OffSetRead
 * 描述  ：读取命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OffSetRead()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x32;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：CloseSerial
 * 描述  ：关闭串口命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void CloseSerial()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x18;
 tx1buf[7] = netState.Net_Connet;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：HandShake
 * 描述  ：握手命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void HandShake(void)
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x58;
	for(i=2;i<18;i++)
		tx1buf[i] = 0;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：msg_crc
 * 描述  ：CRC校验
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
u16 msg_crc(u8 *ptr,u8 num)
{
	u16 crc=0xffff;
	u8 i;
	u16 gx=0x1021;
	while(num--)
	{
		for(i=0x01;i!=0;i<<=1)
		{
			if((crc&0x8000)!=0)
			{
				crc<<=1;
				crc^=gx;
			}
			else
			{
				crc<<=1;
			}
			if(((*ptr)&i)!=0)
			{
				crc^=gx;
			}
		}
		ptr++;
	}
	return ~crc;
}

/************************************************************
 * 函数名：Led_exception
 * 描述  ：故障闪灯
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void Led_exception(void)
{
		tim2flag = !tim2flag;
		
		if(tim2flag)
		{
			if(netState.Net_Insert[0] == 2)
			{
				Led_NetLeft_RedOn;
			}
			if(netState.Net_Insert[1] == 2)
			{
				Led_NetTail_RedOn;
			}
			if(netState.Net_Insert[2] == 2)
			{
				Led_NetRight_RedOn;
			}
		}
		else
		{
			if(netState.Net_Insert[0] == 2)
			{
				Led_NetLeft_RedOff;
			}
			if(netState.Net_Insert[1] == 2)
			{
				Led_NetTail_RedOff;
			}
			if(netState.Net_Insert[2] == 2)
			{
				Led_NetRight_RedOff;
			}
		}	
}

/************************************************************
 * 函数名：tmr1_callback
 * 描述  ：定时器1回调
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void tmr1_callback(OS_TMR *ptmr,void *p_arg)
{
	SilentTime++;
	if(SilentTime>300 && State==1)
	{
		CloseSerial();
	}
}

/************************************************************
 * 函数名：tmr2_callback
 * 描述  ：定时器2回调
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void tmr2_callback(OS_TMR *ptmr,void *p_arg)
{
	Led_exception();
}

/************************************************************
 * 函数名：handshake_task
 * 描述  ：握手任务
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void handshake_task(void *pdata)
{
	u8 *rxbuf;
	INT8U err;
	while(1)
	{
		OSTimeDlyHMSM(0, 0,1,0);
		if(Usart_flag)
		{
			HandShake();
			rxbuf = (u8*)OSMboxPend(msg_hand,200,&err);
			
			if(0x51 == rxbuf[1])//收到回复
			{
				UsartAskCnt = 0;
			}
			else //未收到回复
			{
				Usart_flag = 1;
				if(UsartAskCnt<=5)
					UsartAskCnt++;
				
				if(UsartAskCnt==5)
				{
					State = 0;
					netState.Net_Insert[0] = 0;
					netState.Net_Insert[1] = 0;
					netState.Net_Insert[2] = 0;
					DisableEncode();
					AllLedOff();
					NixieTube_AllOff();
				}
			}
		}
	}
}

/***********************************************************
 * 函数名: paramSet_task
 * 描述  ：第一次插上时初始化参数任务
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void paramSet_task(void *pdata)
{
	u8 *rxbuf;
	u8 *txbuf;
	u8 i;
	u8 numbering;
	while(1)
	{
		OSTimeDlyHMSM(0, 0,0,100);
		
		txbuf = OSQPend(msg_write,0,&err);
		for(i=0;i<18;i++)
			tx1buf[i] = txbuf[i];
		
		SendCnt = 0;
		if(err==OS_ERR_NONE)
		{
			//挂起任务
			OSTaskSuspend(KEY_TASK_PRIO);
			OSTaskSuspend(USARTSEND_TASK_PRIO);
			OSTaskSuspend(HANDSHAKE_TASK_PRIO);
			
			numbering = tx1buf[7];
			Com1SendData();
			SendCnt++;
			while(SendCnt!=0)
			{
				rxbuf = OSMboxPend(msg_receive,200,&err);
				if(err==OS_ERR_NONE)
				{
					if(rxbuf[1]==0x18)//收到关闭串口命令说明注入成功
					{
						SendCnt = 0;
						switch(rxbuf[7])
						{
							case 1:
												netState.Net_Insert[0] = 1;
												break;
							
							case 2:
												netState.Net_Insert[1] = 1;
												break;
							
							case 3:
												netState.Net_Insert[2] = 1;
												break;
							default:
												break;
						}
					}
				}
				else if(err==OS_ERR_TIMEOUT)//超时
				{
					SendCnt++;
					if(SendCnt<4)//发送次数未超过3次
					{
						Com1SendData();
					}
					else //发送次数达3次 
					{
						SendCnt = 0;
						switch(numbering)
						{
							case 1:
												netState.Net_Insert[0] = 2;
												break;
							
							case 2:
												netState.Net_Insert[1] = 2;
												break;
							
							case 3:
												netState.Net_Insert[2] = 2;
												break;
						}
					}
				}
			}
			OSTaskResume(KEY_TASK_PRIO);
			OSTaskResume(USARTSEND_TASK_PRIO);
			OSTaskResume(HANDSHAKE_TASK_PRIO);
			
			Nixie.Display = 1;
		}
	}
}
