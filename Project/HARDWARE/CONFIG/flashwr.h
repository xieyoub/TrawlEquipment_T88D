#ifndef __FLASHWR_H
#define	__FLASHWR_H

#include "stm32f10x.h"
#include <stdio.h>

#define FLASH_PAGE_SIZE    ((u16)0x400) //1024  1K 

void WriteFlash_param(void);
void ReadFlash_param(void);
#endif /* __FLASHWR_H */
