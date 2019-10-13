#ifndef COMTOMASTER_H
#define COMTOMASTER_H

#include "Master_Data.h"
#include "Com_Functions.h"
#include "Half_Duplex.h"

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    MstrData_DataHead Head;
    
    //���ݿռ�
    union MstrRxUnionType
    {
        SampleCMDTypedef  CMD;       //����ָ��
        InitDataTypedef   InitData;  //������ֵ���޶�������ѹ�ķ�Χ����������Ϊ���е���ֵ��о�֮һ��

    }Data;
    
    //����β
    MstrData_DataTail Tail;
    
}Mstr_RxBufTypedef; 

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    MstrData_DataHead Head;
    
    //���ݿռ�
    union MstrTxUnionType
    {
        SampleCMDTypedef  Request;     //����λ��������Ϣ
        SampleDataTypedef SampleData;  //����Ĳ���ֵ
        
    }Data;
    
    //����β
    MstrData_DataTail Tail;
    
}Mstr_TxBufTypedef; 

#define Mstr_COM_UART USART1
#define Mstr_COM_HUART (huart1)
#define Mstr_RX_BUFFERSIZE sizeof(Mstr_RxBufTypedef)
    
#define Mstr_REQUEST_ERROR_MAX 50

extern COMInfoTypedef Mstr_COM_Module;
extern Hf_DuplexTypedef Mstr_HfCOM;

//��ʼ��PCͨ�������ģ���ʼ������
void Mstr_COM_ModuleInit(COMInfoTypedef *pModule);
void SendDataToMaster(void);
void RequestSend(uint8_t ID);

#endif
