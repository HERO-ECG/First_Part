#ifndef MASTER_DATA_H
#define MASTER_DATA_H

//此文件中定义用户通信相关的数据类型，各类型要求大小为整型变量的整数倍
//并约定：0号指令为空指令或指示错误的指令

#include <stdint.h>
#include <stdbool.h>

//结构体定义：通用数据头
typedef struct
{
    uint8_t DataHead1;  //固定数据头1:0xaa
    uint8_t DataHead2;  //固定数据头2:0xbb
    uint8_t MsgTypeID;  //数据类型标识符：指明数据类型
    uint8_t IDFeedback; //向上位机反馈刚刚所接收到的指令类型
    
    uint32_t Verification;  //用于向上位机标识头身份的头校验（相乘校验）
    
}MstrData_DataHead; 

//结构体定义：通用数据尾
typedef struct
{
    uint32_t Verification;  //校验，此时为求和
    
}MstrData_DataTail; 

/***************  以下定义用户数据内容  ********************/

//枚举定义：发送数据类型ID
enum Mstr_TxMsgTypes
{
    //0 - 接收指令错误
    MstrTx_ErrorData = 0,
    
    //1 - 空指令反馈
    MstrTx_EmptyData,
    
    //2 - 控制指令
    MstrTx_CtrlCMD,
    
    //3 - 采样数据
    MstrTx_SampleData,
    
    //4 - 校准采样数据
    MstrTx_CorrectData,
    
    //发送ID的数值上限（辅助合理性校验和数组定义等）
    MstrTx_Num
};

//枚举定义：接收数据类型ID
enum MstrRxMsgTypes
{
    //0 - 接收指令错误
    MstrRx_ErrorCMD = 0,
    
    //1 - 空指令
    MstrRx_EmptyCMD,
    
    //2 - 控制指令
    MstrRx_CtrlCMD,
    
    //3 - 初始化参数
    MstrRx_InitCMD,
                
    //接收ID的数值上限（辅助合理性校验和数组定义等）
    MstrRx_Num
};

/****************************  数据1 采样指令与数据  ********************************/

#define SEND_BUF_SIZE 6

//结构体定义：采样数据包
typedef struct
{
    uint16_t DataSize;      //数据个数
    uint16_t Reserve;       //预留占位符
    
    //0为X，1位
    uint16_t SampleBuffer[SEND_BUF_SIZE][2];
    
}SampleDataTypedef;
 
//结构体定义：初始化数据包
typedef struct
{
    uint16_t X_Max_Voltage;    //X最大
    uint16_t X_Min_Voltage;    //X最小
    uint16_t Y_Max_Voltage;    //Y最大
    uint16_t Y_Min_Voltage;    //Y最小
    
    uint16_t X_TrigValue;   //X方向触发阈限
    uint16_t Y_TrigValue;   //X方向触发阈限
    
}InitDataTypedef;

//结构体定义：采样控制指令
typedef struct
{
    uint32_t CMD;       //指令
    
}SampleCMDTypedef;

/****************************  END -> 采样指令与数据  ********************************/

#endif
