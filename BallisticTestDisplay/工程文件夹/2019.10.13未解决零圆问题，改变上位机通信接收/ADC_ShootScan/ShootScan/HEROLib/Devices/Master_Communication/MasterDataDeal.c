#include "MasterDataDeal.h"
#include "ShootBoard.h"

/**
 *  通信将会以空指令开始。
 *
 */

//向上位机的指令请求
SampleCMDTypedef CtrlCMD = {0};

//从通过校验的数据缓冲区中获得数据
bool GetMsgFromMstrBuffer(Mstr_RxBufTypedef *pBuffer)
{
    bool IsValidID = true;
    
    switch(pBuffer->Head.MsgTypeID)
    {
        case MstrRx_ErrorCMD: 
            break;
        
        case MstrRx_EmptyCMD: GetInitData();
            break;
        
        case MstrRx_CtrlCMD: 
            switch(pBuffer->Data.CMD.CMD)
            {
                //进入校准模式
                case Ctrl_EnterCorrect:
                    
                    isCorrectMode = true;
                    break;
                
                //离开校准模式
                case Ctrl_ExitCorrect:
                    
                    isCorrectMode = false;
                    
                    DeInitBoard();
                
                    //请求更新阈值数据
                    GetInitData();
                    break;
                
                default: break;
            }
            
            break;
            
        case MstrRx_InitCMD:    //更新采样阈值
            BoundX_Max = pBuffer->Data.InitData.X_Max_Voltage;
            BoundX_Min = pBuffer->Data.InitData.X_Min_Voltage;
            BoundY_Max = pBuffer->Data.InitData.Y_Max_Voltage;
            BoundY_Min = pBuffer->Data.InitData.Y_Min_Voltage;
        
            Trig_X = pBuffer->Data.InitData.X_TrigValue;
            Trig_Y = pBuffer->Data.InitData.Y_TrigValue;
        
            //开始采样(阈值去取较小的那个，防止误触发)
            StartScan((Trig_X < Trig_Y ? Trig_X : Trig_Y) - ERROR_BAND);
            break;
                        
        default: IsValidID = false;
                break;
    }

    return IsValidID;
}

//获取待发送数据
bool GetTxData(union MstrTxUnionType *pUnion, uint8_t *pDataType)
{
    switch(*pDataType)
    {
        case MstrTx_ErrorData: 
                            break;
        
        case MstrTx_EmptyData: 
                            break;
        
        case MstrTx_CtrlCMD: pUnion->Request = CtrlCMD;
                            break;
        
        case MstrTx_SampleData: pUnion->SampleData = GetSampleData();
                            break;
        
        case MstrTx_CorrectData: pUnion->SampleData = GetSampleData();
                            break;
                        
        default: break;
    }
    
    return true;
}
