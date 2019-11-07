#include "stm32f10x.h"
#include "flash.h"
#include "delay.h"

/**
  * @brief  ��ȡָ����ַ�İ���(16 λ����)
  * @param  Faddr:����ַ(�˵�ַ����Ϊ 2 �ı���!!)
  * @retval ��Ӧ����.
  */
uint16_t STMFLASH_ReadHalfWord(uint32_t Faddr)
{
	return *(vu16*)Faddr;
}

#if STM32_FLASH_WREN //���ʹ����д
/**
  * @brief  ������д��
  * @param  WriteAddr:	��ʼ��ַ
            pBuffer:		����ָ��
            NumToWrite:	����(16 λ)��
  * @retval None
  */
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
	{
	FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	WriteAddr+=2;//��ַ���� 2.
	}
}

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else
#define STM_SECTOR_SIZE 2048
#endif
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//����� 2K �ֽ�

/**
  * @brief  ��ָ����ַ��ʼ����ָ�����ȵ�����
  * @param  ReadAddr:��ʼ��ַ
            pBuffer:����ָ��
            NumToWrite:����(16 λ)��
  * @retval None
  */
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ 2 ���ֽ�.
		ReadAddr+=2; //ƫ�� 2 ���ֽ�.
	}
}
/**
  * @brief  дָ����ַ�İ���(16 λ����)
  * @param  ReadAddr:��ʼ��ַ
            pBuffer:����ָ��
            NumToWrite:����(16 λ)��
  * @retval None
  */
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	uint32_t secpos; //������ַ
	uint16_t secoff; //������ƫ�Ƶ�ַ(16 λ�ּ���)
	uint16_t secremain; //������ʣ���ַ(16 λ�ּ���)
	uint16_t i;
	uint32_t offaddr; //ȥ�� 0X08000000 ��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock(); //����
	offaddr=WriteAddr-STM32_FLASH_BASE; //ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;
	secoff=(offaddr%STM_SECTOR_SIZE)/2; //�������ڵ�ƫ��(2 ���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff; //����ʣ��ռ��С
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1)
	{
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2); //������������������
		for(i=0;i<secremain;i++) //У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����
		}
		if(i<secremain) //��Ҫ����
		{ //�����������
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,
			STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������
		}
		else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++; //������ַ�� 1
			secoff=0; //ƫ��λ��Ϊ 0
			pBuffer+=secremain; //ָ��ƫ��
			WriteAddr+=secremain; //д��ַƫ��
			NumToWrite-=secremain; //�ֽ�(16 λ)���ݼ�
			//��һ����������д����
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;
			else secremain=NumToWrite;//��һ����������д����
		}
	};
	FLASH_Lock();//����
}
#endif
/**
  * @brief  ����д��
  * @param  WriteAddr:��ʼ��ַ
            WriteData:Ҫд�������
  * @retval None
  */
void Test_Write(uint32_t WriteAddr,uint16_t WriteData)
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ����
}
