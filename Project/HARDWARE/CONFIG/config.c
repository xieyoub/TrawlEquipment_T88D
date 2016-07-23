#include "config.h"

static u8 tim2flag = 0;
static INT8U err;
static u8 Netbuf[3][18];
NixieTub Nixie;

/************************************************************
 * ��������updateOffset
 * ����  �����²���
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������OffSetWrite
 * ����  ��ע�����
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void OffSetWrite()
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	tx1buf[2] = BackOffsetValue>>8;
	tx1buf[3] = BackOffsetValue;
	updateOffset();
	
	if(netState.Net_Connet==1 || LeftRightOffsetValue==0) //����
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
 * ��������OffSetInit
 * ����  ��������ʼ��
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void OffSetInit()
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	switch (nettemp)
	{
		case 1://����
		{
			tx1buf[2] = netparam.left_y>>8;
			tx1buf[3] = netparam.left_y;
			tx1buf[4] = 0;
			tx1buf[5] = netparam.left_x>>8;
			tx1buf[6] = netparam.left_x;
			tx1buf[7] = 1;
			tx1buf[8] = 1; //��һ��ע��
			for(i=0;i<18;i++)
			 Netbuf[0][i] = tx1buf[i];
			OSQPost(msg_write,(void*)Netbuf[0]);
		}
			break;
		
		case 2://��β
		{
			tx1buf[2] = netparam.tail_y>>8;
			tx1buf[3] = netparam.tail_y;
			tx1buf[4] = 0;
			tx1buf[5] = netparam.tail_x>>8;
			tx1buf[6] = netparam.tail_x;
			tx1buf[7] = 2;
			tx1buf[8] = 1; //��һ��ע��
			for(i=0;i<18;i++)
			 Netbuf[1][i] = tx1buf[i];
			OSQPost(msg_write,(void*)Netbuf[1]);
		}
			break;
		
		case 3://����
		{
			tx1buf[2] = netparam.right_y>>8;
			tx1buf[3] = netparam.right_y;
			tx1buf[4] = 1;
			tx1buf[5] = netparam.right_x>>8;
			tx1buf[6] = netparam.right_x;
			tx1buf[7] = 3;
			tx1buf[8] = 1; //��һ��ע��
			for(i=0;i<18;i++)
			 Netbuf[2][i] = tx1buf[i];
			OSQPost(msg_write,(void*)Netbuf[2]);
		}
			break;
	}
}

/************************************************************
 * ��������OpenSerial
 * ����  ���򿪴�������
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void OpenSerial()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x17;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * ��������OffSetRead
 * ����  ����ȡ����
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void OffSetRead()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x32;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * ��������CloseSerial
 * ����  ���رմ�������
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void CloseSerial()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x18;
 tx1buf[7] = netState.Net_Connet;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * ��������HandShake
 * ����  ����������
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������msg_crc
 * ����  ��CRCУ��
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������Led_exception
 * ����  ����������
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������tmr1_callback
 * ����  ����ʱ��1�ص�
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������tmr2_callback
 * ����  ����ʱ��2�ص�
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void tmr2_callback(OS_TMR *ptmr,void *p_arg)
{
	Led_exception();
}

/************************************************************
 * ��������handshake_task
 * ����  ����������
 * ����  ����
 * ���  ����
 * ����  ����
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
			
			if(0x51 == rxbuf[1])//�յ��ظ�
			{
				UsartAskCnt = 0;
			}
			else //δ�յ��ظ�
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
 * ������: paramSet_task
 * ����  ����һ�β���ʱ��ʼ����������
 * ����  : ��
 * ���  : ��
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
			//��������
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
					if(rxbuf[1]==0x18)//�յ��رմ�������˵��ע��ɹ�
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
				else if(err==OS_ERR_TIMEOUT)//��ʱ
				{
					SendCnt++;
					if(SendCnt<4)//���ʹ���δ����3��
					{
						Com1SendData();
					}
					else //���ʹ�����3�� 
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
