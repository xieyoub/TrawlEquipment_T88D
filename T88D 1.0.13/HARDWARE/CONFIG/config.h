#ifndef _CONFIG_H
#define _CONFIG_H
#include "main.h"

void Time1_Config(void);
void AskConnect(void);
void OffSetRead(void);
void OffSetWrite(void);
void OpenSerial(void);
void CloseSerial(void);

u16 msg_crc(u8 *ptr,u8 num);
void updateOffset(void);
void exception(void);
void OffSetInit(void);

#endif
