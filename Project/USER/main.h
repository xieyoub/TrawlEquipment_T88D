#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "delay.h"
#include "Led.h"
#include "encode.h"
#include "key.h"
#include "usart.h"
#include "config.h"
#include "sys.h"
#include "includes.h"
#include "flashwr.h"

extern u32 LeftRightOffsetValue;
extern u16 BackOffsetValue;

extern u16 SilentTime;
extern u8 State;          
extern u8 UsartUsingTime;
extern u8 UsartAskCnt;
extern u8 faultCnt[3];
extern u8 Usart_flag;
extern u8 nettemp;
extern u32 MMSI[3];
extern u8 SendCnt;

extern OS_Q_DATA q_data;
/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//任务优先级
#define START_TASK_PRIO										10
//设置任务堆栈大小
#define START_TASK_SIZE          64
//任务函数
void start_task(void *pdata);

//按键任务
//任务优先级
#define KEY_TASK_PRIO										3
//任务堆栈大小
#define KEY_STK_SIZE										64
//任务函数
void key_task(void *pdata);

//T81握手任务
//任务优先级
#define HANDSHAKE_TASK_PRIO										8
//堆栈大小
#define HANDSHAKE_STK_SIZE										64
//任务函数
void handshake_task(void *pdata);

//数码管显示任务
//任务优先级
#define NixieTube_TASK_PRIO										6
//堆栈大小
#define NixieTube_STK_SIZE										64
//任务函数
void NixieTube_task(void *pdata);

//串口发送任务
//任务优先级
#define USARTSEND_TASK_PRIO									4
//堆栈大小
#define USARTSEND_STK_SIZE									256
//任务函数
void usartsend_task(void *pdata);

//插入时注入的任务
//任务优先级
#define PARAMSET_TASK_PRIO          5
//堆栈大小
#define PARAMSET_STK_SIZE          256
//任务函数
void paramSet_task(void *pdata);

//软件定时器回调函数
void tmr1_callback(OS_TMR *ptmr,void *p_arg);
void tmr2_callback(OS_TMR *ptmr,void *p_arg);

//数码管
typedef struct _NixieTub NixieTub;

struct _NixieTub
{
 u8 Display;
	u8 valueChange;
};

extern NixieTub Nixie;

//串口
typedef struct _Usartsend Usartsend;

struct _Usartsend
{
	u8 type_UsartData;
};



typedef struct _NetState NetState;

struct _NetState 
{
	u8 Net_Sel;     //选中的网位仪
	u8 Net_Connet;  //当前写码的网位仪
	u8 Net_Insert[3]; //三台网位仪的状态
	u8 fault;         //故障
};

extern NetState netState;

typedef struct _NetParam NetParam;

struct _NetParam
{
	u16 left_x,left_y;//左舷参数
	u16 tail_x,tail_y;//网尾参数
	u16 right_x,right_y;//右舷参数
};
extern NetParam netparam;

extern OS_EVENT * q_msg;			//消息队列
extern OS_EVENT * msg_hand;	
extern OS_EVENT *msg_uart;
extern OS_EVENT * msg_write;
extern OS_EVENT * msg_receive;
#endif 
