#include "MasterDataDeal.h"
#include "ShootBoard.h"

/**
 *  ͨ�Ž����Կ�ָ�ʼ��
 *
 */

//����λ����ָ������
SampleCMDTypedef CtrlCMD = {0};

//��ͨ��У������ݻ������л������
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
                //����У׼ģʽ
                case Ctrl_EnterCorrect:
                    
                    isCorrectMode = true;
                    break;
                
                //�뿪У׼ģʽ
                case Ctrl_ExitCorrect:
                    
                    isCorrectMode = false;
                    
                    DeInitBoard();
                
                    //���������ֵ����
                    GetInitData();
                    break;
                
                default: break;
            }
            
            break;
            
        case MstrRx_InitCMD:    //���²�����ֵ
            BoundX_Max = pBuffer->Data.InitData.X_Max_Voltage;
            BoundX_Min = pBuffer->Data.InitData.X_Min_Voltage;
            BoundY_Max = pBuffer->Data.InitData.Y_Max_Voltage;
            BoundY_Min = pBuffer->Data.InitData.Y_Min_Voltage;
        
            Trig_X = pBuffer->Data.InitData.X_TrigValue;
            Trig_Y = pBuffer->Data.InitData.Y_TrigValue;
        
            //��ʼ����(��ֵȥȡ��С���Ǹ�����ֹ�󴥷�)
            StartScan((Trig_X < Trig_Y ? Trig_X : Trig_Y) - ERROR_BAND);
            break;
                        
        default: IsValidID = false;
                break;
    }

    return IsValidID;
}

//��ȡ����������
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
