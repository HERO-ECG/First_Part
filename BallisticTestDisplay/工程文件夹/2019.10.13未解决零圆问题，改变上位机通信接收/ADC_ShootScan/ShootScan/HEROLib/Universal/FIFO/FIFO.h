#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool isInited;          //标记是否已经被初始化过
    
    uint32_t FIFO_Size;     //循环队列大小(按所包含的元素个数计)
    uint32_t Element_Size;  //每个元素的大小（所占字节数）
    
    void *pBuffer;          //指向数据缓冲区首地址的指针，一经初始化，再不允许修改
    
    uint32_t HeadPointer;   //队列头指针，用于读数，读一个加一（功能类似于数组下标）
    uint32_t TailPointer;   //队列尾指针，用于存数，读一个加一（功能类似于数组下标）
    
    bool isFull;    //队列满标识
    bool isEmpty;   //队列空标识
    
}FIFO_Typedef;

bool FIFO_Init(FIFO_Typedef *pFIFO, void* pBuffer, 
                uint32_t FIFO_Size, uint32_t ElementSize);
bool FIFO_SaveData(FIFO_Typedef *pFIFO, void *pData);
bool FIFO_ReadData(FIFO_Typedef *pFIFO, void *pData);
int32_t GetDataNumInFIFO(FIFO_Typedef *pFIFO);

#endif

