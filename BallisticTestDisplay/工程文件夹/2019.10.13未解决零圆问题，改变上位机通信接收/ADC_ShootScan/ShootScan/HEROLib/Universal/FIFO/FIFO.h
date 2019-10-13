#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool isInited;          //����Ƿ��Ѿ�����ʼ����
    
    uint32_t FIFO_Size;     //ѭ�����д�С(����������Ԫ�ظ�����)
    uint32_t Element_Size;  //ÿ��Ԫ�صĴ�С����ռ�ֽ�����
    
    void *pBuffer;          //ָ�����ݻ������׵�ַ��ָ�룬һ����ʼ�����ٲ������޸�
    
    uint32_t HeadPointer;   //����ͷָ�룬���ڶ�������һ����һ�����������������±꣩
    uint32_t TailPointer;   //����βָ�룬���ڴ�������һ����һ�����������������±꣩
    
    bool isFull;    //��������ʶ
    bool isEmpty;   //���пձ�ʶ
    
}FIFO_Typedef;

bool FIFO_Init(FIFO_Typedef *pFIFO, void* pBuffer, 
                uint32_t FIFO_Size, uint32_t ElementSize);
bool FIFO_SaveData(FIFO_Typedef *pFIFO, void *pData);
bool FIFO_ReadData(FIFO_Typedef *pFIFO, void *pData);
int32_t GetDataNumInFIFO(FIFO_Typedef *pFIFO);

#endif

