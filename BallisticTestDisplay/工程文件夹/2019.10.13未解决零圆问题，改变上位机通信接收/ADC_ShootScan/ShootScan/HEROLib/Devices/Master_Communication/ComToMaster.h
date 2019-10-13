#ifndef COMTOMASTER_H
#define COMTOMASTER_H

#include "Master_Data.h"
#include "Com_Functions.h"
#include "Half_Duplex.h"

//结构体定义：接收数据缓冲区
typedef struct
{
    //数据头
    MstrData_DataHead Head;
    
    //数据空间
    union MstrRxUnionType
    {
        SampleCMDTypedef  CMD;       //控制指令
        InitDataTypedef   InitData;  //采样阈值（限定采样电压的范围，可用于作为击中点出现的判据之一）

    }Data;
    
    //数据尾
    MstrData_DataTail Tail;
    
}Mstr_RxBufTypedef; 

//结构体定义：发送数据缓冲区
typedef struct
{
    //数据头
    MstrData_DataHead Head;
    
    //数据空间
    union MstrTxUnionType
    {
        SampleCMDTypedef  Request;     //向上位机请求信息
        SampleDataTypedef SampleData;  //捕获的采样值
        
    }Data;
    
    //数据尾
    MstrData_DataTail Tail;
    
}Mstr_TxBufTypedef; 

#define Mstr_COM_UART USART1
#define Mstr_COM_HUART (huart1)
#define Mstr_RX_BUFFERSIZE sizeof(Mstr_RxBufTypedef)
    
#define Mstr_REQUEST_ERROR_MAX 50

extern COMInfoTypedef Mstr_COM_Module;
extern Hf_DuplexTypedef Mstr_HfCOM;

//初始化PC通信组件的模块初始化函数
void Mstr_COM_ModuleInit(COMInfoTypedef *pModule);
void SendDataToMaster(void);
void RequestSend(uint8_t ID);

#endif
