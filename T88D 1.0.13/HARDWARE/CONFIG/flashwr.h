#ifndef __FLASHWR_H
#define	__FLASHWR_H

#include "stm32f10x.h"
#include <stdio.h>

#define FLASH_PAGE_SIZE    ((u16)0x400) //1024  1K 

void WriteFlash_Momship(void);
void ReadFlash_Momship(void);
void WriteFlash_Dis(void);
void netParamInit(void);
void WriteLeftParam(void);
void WriteTailParam(void);
void WriteRightParam(void);
#endif /* __FLASHWR_H */
