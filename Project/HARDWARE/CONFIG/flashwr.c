#include "main.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

//BKP_DR2 ����openflag,(ԭΪo_time)

//StartAddr[3] 0x0800B000 --- cogesl

/*
FLASHʹ�������0:ĸ��λ��; 1:����λ���ϵ�twoship,twonet
*/

//Ҫд��Flash�����ݵ��׵�ַ--FLASH��ʼ��ַ
u32 StartAddr[13]={0x08009800,0x0800A000,0x0800A800,0x0800B000,0x0800B800,
									 0x0800C000,0x0800C800,0x0800D000,0x0800D800,0x0800E000,
									 0x0800E800,0x0800F000,0x0800F800};
//Ҫд��Flash�����ݵ�ĩ��ַ--FLASH������ַ
u32 EndAddr[13] = {0x08009BFF,0x0800A3FF,0x0800ABFF,0x0800B3FF,0x0800BBFF,
									 0x0800C3FF,0x0800CBFF,0x0800D300,0x0800DB00,0x0800E300,
									 0x0800EB00,0x0800F300,0x0800FB00};

u32 FlashAddress=0x00;//Flash���ڲ���ַ	 
vu32 NbrOfPage = 0x00; //Ҫ������ҳ������
volatile FLASH_Status FLASHStatus;
volatile TestStatus MemoryProgramStatus;


/************************************************************
 * ��������WriteFlash_param
 * ����  ��������д��Flash��
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void WriteFlash_param()
{
	FLASHStatus = FLASH_COMPLETE;
	MemoryProgramStatus = PASSED; 
	FLASH_Unlock();	//FLASH����
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//���־λ
	
	NbrOfPage = (EndAddr[1] - StartAddr[1]) / FLASH_PAGE_SIZE; //ҳ������ӳ���
	FLASHStatus = FLASH_ErasePage(StartAddr[1] + (FLASH_PAGE_SIZE * NbrOfPage));
	FlashAddress = StartAddr[1];
	
	FLASHStatus = FLASH_ProgramWord(FlashAddress,netparam.left_x);
	FlashAddress += 4;
	FLASHStatus = FLASH_ProgramWord(FlashAddress,netparam.left_y);
	FlashAddress += 4;
	FLASHStatus = FLASH_ProgramWord(FlashAddress,netparam.tail_x);
	FlashAddress += 4;
	FLASHStatus = FLASH_ProgramWord(FlashAddress,netparam.tail_y);
	FlashAddress += 4;
	FLASHStatus = FLASH_ProgramWord(FlashAddress,netparam.right_x);
	FlashAddress += 4;
 FLASHStatus = FLASH_ProgramWord(FlashAddress,netparam.right_y);
}

/************************************************************
 * ��������ReadFlash_param
 * ����  ����Flash�ж�ȡ����
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void ReadFlash_param()
{
	FlashAddress = StartAddr[1];
	netparam.left_x = *(u16*)FlashAddress;
	FlashAddress += 4;
	netparam.left_y = *(u16*)FlashAddress;
	FlashAddress += 4;
	netparam.tail_x = *(u16*)FlashAddress;
	FlashAddress += 4;
	netparam.tail_y = *(u16*)FlashAddress;
	FlashAddress += 4;
	netparam.right_x = *(u16*)FlashAddress;
	FlashAddress += 4;
	netparam.right_y = *(u16*)FlashAddress;
	//��һ��ʹ��T88 Ĭ��ֵ
	if(netparam.left_x==0xffff)
	{
		netparam.left_x = 50;
		netparam.left_y = 400;
		netparam.tail_x = 0;
		netparam.tail_y = 500;
		netparam.right_x = 50;
		netparam.right_y = 400;
		WriteFlash_param();
	}
}
