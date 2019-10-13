#ifndef HALF_DUPLEX_H
#define HALF_DUPLEX_H

#include "ComStructDef.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum 
{
    //无错误
    Hf_NoError = 0,
    
    //未初始化结构体
    Hf_Error_UnInited,
    
    Hf_ErrorCodeNum
    
}Hf_ErrorCode;

//结构体定义：半双工机制实现结构体
typedef struct 
{
    //标识是否已经初始化该结构体
    bool isInited;
    
    /***************  异常处理  ****************/
    //错误码
    Hf_ErrorCode ErrorCode;
    
    //对本组件异常的文字性描述
    char* ErrorDescription;
    
    /*************  END -> 异常处理  ************/
    
    /************  链接状态标识  ************/
    //标识当前本组件是否处于阻塞状态
    bool isBlocked;
    
    /*******  发送和接收标志  *******/
    //(is Get Message)    
    //标识本机是否已经接收到了消息
    bool isGetMsg;
    
    //标识这次接收到的反馈ID与上次发出的匹配，这说明上条消息已经发送成功，不用再重复发送了。
    bool isFdBckMatched;
    
    //发送和接收数据ID记录器，用于实现握手操作
    uint8_t SendDataID;
    uint8_t RcvDataID;
    
    /******  通信组件引用  ******/
    //指向通信组件的结构体指针
    COMInfoTypedef *pCOM;
    
    /******  计数器  ******/    
    //(Request Error Count)
    //请求驳回计数器
    //发送请求被拒绝，该计数器加一，
    //当累加到ErrorCntMax后触发异常处理函数
    uint32_t RqstErrorCnt; 
    
    //(Error Count Max)
    //驳回计数器的最大值，用于判定通讯中断
    //初始化为0，则不启用异常处理函数
    uint32_t ErrorCntMax;

    /******  方法集合  ******/    
    //通信堵塞的异常处理函数
    void (*pBlockError)(void);
    
}Hf_DuplexTypedef;

void Hf_DuplexStructInit(Hf_DuplexTypedef *pModule, COMInfoTypedef *pCOM, uint32_t ErrorCntMax, void (*pHandler)(void));
void Hf_HaveReceived(Hf_DuplexTypedef *pModule, uint8_t FeedBackID, uint8_t RcvID);
void Hf_SendData(Hf_DuplexTypedef *pModule, uint8_t CMD);
void Hf_Reconnect(Hf_DuplexTypedef *pModule);

#endif

