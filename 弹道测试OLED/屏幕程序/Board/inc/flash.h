#ifndef _STM32_FLASH_H_
#define _STM32_FLASH_H_

#include "stm32f10x.h"
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 32 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ

void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);

#endif
