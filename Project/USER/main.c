#include "main.h"

u32 LeftRightOffsetValue = 0; //左舷/右舷偏移量
u16 BackOffsetValue;          //后方偏移量

u16 SilentTime=0;             //静默时间
u8 State=0;                   //当前状态，0:无写码状态，1：正在写码     
u8 UsartUsingTime = 0;        //串口等待时间
u8 UsartAskCnt = 0;           //串口询问计次
u8 faultCnt[3]={0};           //故障计次
u8 nettemp;
u8 Usart_flag = 1;
u32 MMSI[3]={0};

u8 SendCnt=0;  //发送注入命令的次数

NetState netState;
NetParam netparam;
OS_Q_DATA q_data; 

//任务堆栈
OS_STK START_TASK_STK[START_TASK_SIZE];
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_STK HANDSHAKE_TASK_STK[HANDSHAKE_STK_SIZE];
OS_STK NixieTube_TASK_STK[NixieTube_STK_SIZE];
OS_STK USARTSEND_TASK_STK[USARTSEND_STK_SIZE];
OS_STK PARAMSET_TASK_STK[PARAMSET_STK_SIZE];

void * MsgGrp[256];			//消息队列存储地址,最大支持256个消息
void * MsgWrite[10];  
OS_EVENT * q_msg;			//消息队列
OS_EVENT * msg_hand;	
OS_EVENT * msg_uart;
OS_EVENT * msg_receive;
OS_EVENT * msg_write;

//软件定时器
OS_TMR   * tmr1;			//静默时间判断
OS_TMR   * tmr2;   //故障闪灯

int main(void)
{	
	delay_init();
	NVIC_Configuration();
	//禁用JTAG PA15作为普通IO	
	DisableJTAG();   
	TM1652_Init(19200); //USART2，3配置
	Encode_Init();      //编码器功能，定时器2，3
	Led_Init();   
	USART1_Config(); //与T81通信
	Key_Init();
	ReadFlash_param(); //读取flash中的参数
	
 OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_TASK_SIZE-1],START_TASK_PRIO);//开始任务
	OSStart();
}

//开始任务
void start_task(void *pdata)
{
	u8 err;
	OS_CPU_SR cpu_sr=0;
	
	q_msg=OSQCreate(&MsgGrp[0],256);	//串口发送任务消息队列
	msg_hand = OSMboxCreate((void*) 0); //握手串口接收邮箱
	msg_uart = OSMboxCreate((void*) 0); //其他串口邮箱
	msg_receive = OSMboxCreate((void*) 0); //第一次注入邮箱
	msg_write=OSQCreate(&MsgWrite[0],10);	//创建消息队列
	//静默时间计时
	tmr1=OSTmrCreate(10,100,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);		//1s执行一次
	OSTmrStart(tmr1,&err);//启动软件定时器1	
	//故障灯
	tmr2=OSTmrCreate(10,100,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr2_callback,0,"tmr2",&err);		//1s执行一次
	OSTmrStart(tmr2,&err);//启动软件定时器2
	
	OS_ENTER_CRITICAL();	//进入临界区(无法被中断打断)  
	OSTaskCreate(paramSet_task,(void *)0,(OS_STK*)&PARAMSET_TASK_STK[PARAMSET_STK_SIZE-1],PARAMSET_TASK_PRIO);//插上网位仪注入任务
	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	                       //按键任务
	OSTaskCreate(usartsend_task,(void *)0,(OS_STK*)&USARTSEND_TASK_STK[USARTSEND_STK_SIZE-1],USARTSEND_TASK_PRIO);//串口发送任务
	OSTaskCreate(NixieTube_task,(void *)0,(OS_STK*)&NixieTube_TASK_STK[NixieTube_STK_SIZE-1],NixieTube_TASK_PRIO);//数码管显示任务						   					   
 OSTaskCreate(handshake_task,(void *)0,(OS_STK*)&HANDSHAKE_TASK_STK[HANDSHAKE_STK_SIZE-1],HANDSHAKE_TASK_PRIO);//握手任务	 				   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}

