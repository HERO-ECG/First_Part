#ifndef _STM32_FLASH_H_
#define _STM32_FLASH_H_

#include "stm32f10x.h"
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 32 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值

void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);

#endif
