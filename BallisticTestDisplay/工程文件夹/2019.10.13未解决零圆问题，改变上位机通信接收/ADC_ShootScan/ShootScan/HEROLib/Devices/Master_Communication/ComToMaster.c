#include "ComToMaster.h"
#include "Comunication.h"
#include "MasterDataDeal.h"

#include "FIFO.h"

//与控制端通信的通信组件实体定义
COMInfoTypedef Mstr_COM_Module;
//半双工通信机制实现组件
Hf_DuplexTypedef Mstr_HfCOM;

//发送请求
uint8_t SendRequest = MstrTx_EmptyData;

/*  内部变量声明  */
//发送缓冲区
Mstr_TxBufTypedef Mstr_TxBuffer;
//接收缓冲区
Mstr_RxBufTypedef Mstr_RxBuffer[2];

/*  内部函数声明  */
static bool DealMstrData(void *pRxBuffer);
static bool SendDataToMstr(uint8_t SendCMD);
static void BlockHandler(void);

//初始化本接口对应的通信组件
void Mstr_COM_ModuleInit(COMInfoTypedef *pModule)
{    
    //初始化半双工结构体，绑定通信组件，不使用阻塞处理
    Hf_DuplexStructInit(&Mstr_HfCOM, pModule, Mstr_REQUEST_ERROR_MAX, BlockHandler);
    
    //以串口方式初始化通信组件
    COM_UART_StructInit(pModule, &Mstr_COM_HUART,
                        DealMstrData, SendDataToMstr,
                        Mstr_RxBuffer, Mstr_RX_BUFFERSIZE,
                        &Mstr_TxBuffer, sizeof(Mstr_TxBufTypedef));
    
    /* 使能接收，进入中断回调函数 */
    HAL_UART_Receive_DMA(&Mstr_COM_HUART, 
                        (uint8_t*)pModule->pRxBuffer[pModule->RxBufFlag], 
                        Mstr_COM_Module.RxBufSize);
}

//以半双工方式向上位机发送数据(定时调用即可，没特殊请求时发送的是空帧)
void SendDataToMaster(void)
{
    uint8_t TempID = 0x00;
    
    if(Mstr_HfCOM.isFdBckMatched)
    {
        TempID = SendRequest;
        SendRequest = MstrTx_EmptyData;
    }
    else
    {
        //再发一遍上次的指令
        TempID = Mstr_HfCOM.SendDataID;
    }
    
    //半双工发送数据
    Hf_SendData(&Mstr_HfCOM, TempID);
}

//产生数据请求，若该请求合法，那么它将在下一个发送周期被发出
void RequestSend(uint8_t ID)
{
    //如果此时空闲，则将指令写入,否则指令无效
    if(MstrTx_EmptyData == SendRequest)
    {
        SendRequest = ID;
    }
    else if(MstrTx_CtrlCMD == ID)
    {
        SendRequest = ID;
    }
}

//获取数据求和校验值
static uint32_t GetAddCheck(uint8_t *pData, uint32_t DataSize)
{
    uint32_t check = 0;
    
    //求和校验
    for(uint32_t i = 0; i < DataSize; i++)
    {
        check += pData[i];
    }
    return check;
}

//校验并获得数据头
static inline bool Check_FindHead(uint8_t *pData, uint32_t DataSize, 
                                        uint32_t TailSize, uint32_t *pHeadPos, uint32_t *pCheck)
{
    //计数器
    int32_t i;
    bool isFindDataHead = false;
    
    (*pCheck) = 0;
    
    //校验并寻找数据头
    for(i = 0, (*pHeadPos) = 0; i < DataSize; i++)	
    {
        //求和范围是除数据尾外的所有数据
        if(i < DataSize - TailSize)
        {
            (*pCheck) += pData[i];			    //求和校验
        }
        
        if(0xaa == pData[i])				//检索数据头位置
        {
            (*pHeadPos) = i;
        }
        if((0xbb == pData[i]) && ((*pHeadPos) == (i-1)))	//如果这一位是0xBB,且前一位是0xAA，
        {											//那么数据头在缓冲区的位置就可以确定
            isFindDataHead = true;
        }
    }
    
    return isFindDataHead;
}

//处理接收到控制端的数据
static bool DealMstrData(void *pRxBuffer)
{
    //获取缓冲区地址
    Mstr_RxBufTypedef *pBuffer = (Mstr_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  处理数据  */
    
    //数据头位置、校验值
    uint32_t HeadPos = 0;
    uint32_t check = 0;

	if(Mstr_COM_Module.RxBufSize != Mstr_RX_BUFFERSIZE)
	{	
        //此种情况是在进行补偿操作，纠正数据头位置
		Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
	}
	else
	{
        //解决相位滞后一个节拍的问题
        if(Mstr_COM_Module.isCorrectHead)
        {
            //此种情况是在进行补偿操作，纠正数据头位置
            Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
            
            Mstr_COM_Module.isCorrectHead = false;
            
            return false;
        }
        
        //求校验值并寻找数据头
        bool isFindDataHead = Check_FindHead(pData, Mstr_RX_BUFFERSIZE, sizeof(pBuffer->Tail), 
                                &HeadPos, &check);
        
        //处理数据头位置异常的情况（如果正常，应当在最前端）
        if(!isFindDataHead)
        {
            //防止下面的if被优化掉
            volatile uint8_t BufTail = pData[Mstr_RX_BUFFERSIZE - 1];
            
            if(0xAA == BufTail)
            {
                //如果出现了头尾错位的情形，此时数据头在数据尾处
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE - 1;
            }
            else
            {
                //数据中压根就不存在数据头,就继续按原长接收，直到缓冲区中出现了数据头
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
            }
        }
        else
        {
            //若数据头不在首位
            if(0 != HeadPos)
            {
                //下一次接收这么多位就可以消除数据错位
                Mstr_COM_Module.Rx_NextRcvLength = HeadPos;
                
                Mstr_COM_Module.isCorrectHead = true;
            }
            else
            {            
                //正常情况下就应当按照原长接收
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
                
                //最后的关卡：校验值
                if(pBuffer->Tail.Verification == check)				
                {
                    Hf_HaveReceived(&Mstr_HfCOM, pBuffer->Head.IDFeedback, pBuffer->Head.MsgTypeID);
                    
                    //调用数据处理函数
                    GetMsgFromMstrBuffer(pBuffer);
                    
                    return true;
                }
            }
        }
    }    
    return false;
}

//向上位机发送数据
static bool SendDataToMstr(uint8_t SendCMD)
{
    bool isSendThisCMD = true;
    //获取发送缓冲区地址
    Mstr_TxBufTypedef *TxBuffer = Mstr_COM_Module.pTxBuffer;

    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2 = 0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.IDFeedback = Mstr_HfCOM.RcvDataID;

    //获得头校验值(相乘校验)
    TxBuffer->Head.Verification = TxBuffer->Head.DataHead1 
                                 * TxBuffer->Head.DataHead2
                                 * TxBuffer->Head.MsgTypeID
                                 * TxBuffer->Head.IDFeedback;

    //写入待发送数据并获悉本次发送请求是否因上次的包对方没接到而被驳回
    isSendThisCMD = GetTxData(&(TxBuffer->Data), &(TxBuffer->Head.MsgTypeID));

    //获取校验值
    TxBuffer->Tail.Verification = GetAddCheck((uint8_t*)TxBuffer, 
                                            Mstr_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));

    //本次将消息发出后进入等待状态
    if(COM_SendDataInTxBuffer(&Mstr_COM_Module))
    {
        return isSendThisCMD;
    }
    return false;
}

//阻塞处理函数:检查计数值并重连
static void BlockHandler(void)
{
    //如果通信阻塞了，则进行重连
    if(Mstr_HfCOM.isBlocked)
    {
        Hf_Reconnect(&Mstr_HfCOM);
    }
}
