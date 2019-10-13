#include "stm32f10x.h"
#include "flash.h"
#include "delay.h"

/**
  * @brief  读取指定地址的半字(16 位数据)
  * @param  Faddr:读地址(此地址必须为 2 的倍数!!)
  * @retval 对应数据.
  */
uint16_t STMFLASH_ReadHalfWord(uint32_t Faddr)
{
	return *(vu16*)Faddr;
}

#if STM32_FLASH_WREN //如果使能了写
/**
  * @brief  不检查的写入
  * @param  WriteAddr:	起始地址
            pBuffer:		数据指针
            NumToWrite:	半字(16 位)数
  * @retval None
  */
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
	{
	FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	WriteAddr+=2;//地址增加 2.
	}
}

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else
#define STM_SECTOR_SIZE 2048
#endif
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是 2K 字节

/**
  * @brief  从指定地址开始读出指定长度的数据
  * @param  ReadAddr:起始地址
            pBuffer:数据指针
            NumToWrite:半字(16 位)数
  * @retval None
  */
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取 2 个字节.
		ReadAddr+=2; //偏移 2 个字节.
	}
}
/**
  * @brief  写指定地址的半字(16 位数据)
  * @param  ReadAddr:起始地址
            pBuffer:数据指针
            NumToWrite:半字(16 位)数
  * @retval None
  */
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	uint32_t secpos; //扇区地址
	uint16_t secoff; //扇区内偏移地址(16 位字计算)
	uint16_t secremain; //扇区内剩余地址(16 位字计算)
	uint16_t i;
	uint32_t offaddr; //去掉 0X08000000 后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock(); //解锁
	offaddr=WriteAddr-STM32_FLASH_BASE; //实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;
	secoff=(offaddr%STM_SECTOR_SIZE)/2; //在扇区内的偏移(2 个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff; //扇区剩余空间大小
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1)
	{
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2); //读出整个扇区的内容
		for(i=0;i<secremain;i++) //校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除
		}
		if(i<secremain) //需要擦除
		{ //擦除这个扇区
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,
			STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区
		}
		else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间.
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++; //扇区地址增 1
			secoff=0; //偏移位置为 0
			pBuffer+=secremain; //指针偏移
			WriteAddr+=secremain; //写地址偏移
			NumToWrite-=secremain; //字节(16 位)数递减
			//下一个扇区还是写不完
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;
			else secremain=NumToWrite;//下一个扇区可以写完了
		}
	};
	FLASH_Lock();//上锁
}
#endif
/**
  * @brief  测试写入
  * @param  WriteAddr:起始地址
            WriteData:要写入的数据
  * @retval None
  */
void Test_Write(uint32_t WriteAddr,uint16_t WriteData)
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字
}
