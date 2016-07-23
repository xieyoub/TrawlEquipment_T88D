#ifndef _KEY_H
#define _KEY_H
#include "main.h"

void Key_Init(void);
void EXTIX_Init(void);
u8 Get_KeyValue(void);

#define NO_KEY_PRESS	0		//无按键按下的键值

#define KEY_STAT_noKeyPress 0
#define KEY_STAT_filter     1
#define KEY_PRESS           2

#endif

