#include "FIFO.h"

//循环队列初始化
bool FIFO_Init(FIFO_Typedef *pFIFO, void* pBuffer, 
                uint32_t FIFO_Size, uint32_t ElementSize)
{
    //非法输入检查
    if(0 == pBuffer) { return false; }
    
    //初始化缓冲区相关信息
    pFIFO->pBuffer = pBuffer;
    pFIFO->FIFO_Size = FIFO_Size;
    
    //记录元素大小
    pFIFO->Element_Size = ElementSize;
    
    //复位记录器
    pFIFO->HeadPointer = 0;
    pFIFO->TailPointer = 0;
    pFIFO->isEmpty = true;
    pFIFO->isFull = false;
    
    //标记已初始化完成
    pFIFO->isInited = true;
    
    return true;
}

//向队列中存入一个数据,若存入成功，则返回真
//返回假的可能原因：1.队列满，2.传入数据指针为空,3.队列未被初始化
bool FIFO_SaveData(FIFO_Typedef *pFIFO, void *pData)
{
    uint32_t Cnt = 0;
    uint8_t *pTempBuf = 0, *pTempData = 0;
    
    //非法输入检查
    if(0 == pData || (!pFIFO->isInited) || pFIFO->isFull) { return false; }
    
    //计算临时待存入的数据地址
    pTempData = pData;
    pTempBuf = ((uint8_t *)pFIFO->pBuffer + (pFIFO->TailPointer * pFIFO->Element_Size));
    
    //按字节存入数据
    for(Cnt = 0; Cnt < pFIFO->Element_Size; Cnt++)
    {
        pTempBuf[Cnt] = pTempData[Cnt];
    }
    
    //存数据指针自增
    pFIFO->TailPointer++;
    
    //循环下标
    if(pFIFO->TailPointer >= pFIFO->FIFO_Size)
    {
        pFIFO->TailPointer = 0;
    }
    
    //如果头尾相遇了，说明满了
    pFIFO->isFull = (pFIFO->HeadPointer == pFIFO->TailPointer);
    
    //标记队列非空
    pFIFO->isEmpty = false;
    
    return true;
}

//从队列中读出一个数据，若读取成功，则返回真，读出的数据被存放在传入指针指向的内存空间中
//返回假的可能原因：1.队列空，2.传入数据指针为空,3.队列未被初始化
bool FIFO_ReadData(FIFO_Typedef *pFIFO, void *pData)
{
    uint32_t Cnt = 0;
    uint8_t *pTempBuf = 0, *pTempData = 0;
    
    //非法输入检查
    if(0 == pData || (!pFIFO->isInited) || pFIFO->isEmpty) { return false; }
    
    //计算临时待存入的数据地址
    pTempData = pData;
    pTempBuf = ((uint8_t *)pFIFO->pBuffer + (pFIFO->HeadPointer * pFIFO->Element_Size));
    
    //按字节读出数据
    for(Cnt = 0; Cnt < pFIFO->Element_Size; Cnt++)
    {
        pTempData[Cnt] = pTempBuf[Cnt];
    }
    
    //读数据指针自增
    pFIFO->HeadPointer++;
    
    //循环下标
    if(pFIFO->HeadPointer >= pFIFO->FIFO_Size)
    {
        pFIFO->HeadPointer = 0;
    }
    
    //如果头尾相遇了，说明空了
    pFIFO->isEmpty = (pFIFO->HeadPointer == pFIFO->TailPointer);
    
    //标记队列未满
    pFIFO->isFull = false;
    
    return true;
}

//获取FIFO中所存储的数据个数
int32_t GetDataNumInFIFO(FIFO_Typedef *pFIFO)
{
    int32_t Result = 0;
    
    Result = pFIFO->TailPointer - pFIFO->HeadPointer;
    
    Result = Result < 0 ? (Result + pFIFO->FIFO_Size) : Result;
    
    return Result;
}

