#include "FIFO.h"

//ѭ�����г�ʼ��
bool FIFO_Init(FIFO_Typedef *pFIFO, void* pBuffer, 
                uint32_t FIFO_Size, uint32_t ElementSize)
{
    //�Ƿ�������
    if(0 == pBuffer) { return false; }
    
    //��ʼ�������������Ϣ
    pFIFO->pBuffer = pBuffer;
    pFIFO->FIFO_Size = FIFO_Size;
    
    //��¼Ԫ�ش�С
    pFIFO->Element_Size = ElementSize;
    
    //��λ��¼��
    pFIFO->HeadPointer = 0;
    pFIFO->TailPointer = 0;
    pFIFO->isEmpty = true;
    pFIFO->isFull = false;
    
    //����ѳ�ʼ�����
    pFIFO->isInited = true;
    
    return true;
}

//������д���һ������,������ɹ����򷵻���
//���ؼٵĿ���ԭ��1.��������2.��������ָ��Ϊ��,3.����δ����ʼ��
bool FIFO_SaveData(FIFO_Typedef *pFIFO, void *pData)
{
    uint32_t Cnt = 0;
    uint8_t *pTempBuf = 0, *pTempData = 0;
    
    //�Ƿ�������
    if(0 == pData || (!pFIFO->isInited) || pFIFO->isFull) { return false; }
    
    //������ʱ����������ݵ�ַ
    pTempData = pData;
    pTempBuf = ((uint8_t *)pFIFO->pBuffer + (pFIFO->TailPointer * pFIFO->Element_Size));
    
    //���ֽڴ�������
    for(Cnt = 0; Cnt < pFIFO->Element_Size; Cnt++)
    {
        pTempBuf[Cnt] = pTempData[Cnt];
    }
    
    //������ָ������
    pFIFO->TailPointer++;
    
    //ѭ���±�
    if(pFIFO->TailPointer >= pFIFO->FIFO_Size)
    {
        pFIFO->TailPointer = 0;
    }
    
    //���ͷβ�����ˣ�˵������
    pFIFO->isFull = (pFIFO->HeadPointer == pFIFO->TailPointer);
    
    //��Ƕ��зǿ�
    pFIFO->isEmpty = false;
    
    return true;
}

//�Ӷ����ж���һ�����ݣ�����ȡ�ɹ����򷵻��棬���������ݱ�����ڴ���ָ��ָ����ڴ�ռ���
//���ؼٵĿ���ԭ��1.���пգ�2.��������ָ��Ϊ��,3.����δ����ʼ��
bool FIFO_ReadData(FIFO_Typedef *pFIFO, void *pData)
{
    uint32_t Cnt = 0;
    uint8_t *pTempBuf = 0, *pTempData = 0;
    
    //�Ƿ�������
    if(0 == pData || (!pFIFO->isInited) || pFIFO->isEmpty) { return false; }
    
    //������ʱ����������ݵ�ַ
    pTempData = pData;
    pTempBuf = ((uint8_t *)pFIFO->pBuffer + (pFIFO->HeadPointer * pFIFO->Element_Size));
    
    //���ֽڶ�������
    for(Cnt = 0; Cnt < pFIFO->Element_Size; Cnt++)
    {
        pTempData[Cnt] = pTempBuf[Cnt];
    }
    
    //������ָ������
    pFIFO->HeadPointer++;
    
    //ѭ���±�
    if(pFIFO->HeadPointer >= pFIFO->FIFO_Size)
    {
        pFIFO->HeadPointer = 0;
    }
    
    //���ͷβ�����ˣ�˵������
    pFIFO->isEmpty = (pFIFO->HeadPointer == pFIFO->TailPointer);
    
    //��Ƕ���δ��
    pFIFO->isFull = false;
    
    return true;
}

//��ȡFIFO�����洢�����ݸ���
int32_t GetDataNumInFIFO(FIFO_Typedef *pFIFO)
{
    int32_t Result = 0;
    
    Result = pFIFO->TailPointer - pFIFO->HeadPointer;
    
    Result = Result < 0 ? (Result + pFIFO->FIFO_Size) : Result;
    
    return Result;
}

