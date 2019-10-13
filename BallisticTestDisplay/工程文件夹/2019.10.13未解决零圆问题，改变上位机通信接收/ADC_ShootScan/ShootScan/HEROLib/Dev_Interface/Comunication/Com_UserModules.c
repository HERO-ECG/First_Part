#include "Com_UserModules.h"
#include "Com_CanDriver.h"
#include "ComToMaster.h"

//�ڲ������Ŀ�����ṹ�壬���ڱ�ʶδ�ҵ���Ӧ��������
COMInfoTypedef Null_COM_Module;

//ͨ���������
COMInfoTypedef *COM_Modules[COM_ModulesNum] = 
{
    //������ṹ��
    &Null_COM_Module,
    
    /*  ʹ�ô��ڵ�ͨ�����  */    
    //����ƶ˵�ͨ������ӿ�
    &Mstr_COM_Module,
    
    /*  ʹ��CAN��ͨ�����,˳���ȷǸ�ʽ�����ʽ��,��CAN1��CAN2  */
#if defined(USING_CAN)
    
    //�豸��1��ͨ�����
    &DevGroup1_COM_Module,
    
    //�豸��2��ͨ�����
    &DevGroup2_COM_Module,
    
    /* CAN1��� */
    //���������ӵ�ͨ������ӿ�
    &BSP_COM_Module,
    
    /* CAN2��� */
#endif
};

#if defined(USING_CAN)
    
//���Զ���ͨ��Э�鷢�͵�����ID���ϣ������桰ʹ��CAN��ͨ�������˳��һһ��Ӧ��
const uint32_t CAN1_FormatStdIDs[] = 
{
    0x2FF,           //���ͨ��ID
};
uint32_t CAN1_FormatStdIDNum = (sizeof(CAN1_FormatStdIDs) / sizeof(uint32_t));

const uint32_t CAN2_FormatStdIDs[] = 
{
    0x2FF,           //���ͨ��ID
};
uint32_t CAN2_FormatStdIDNum = (sizeof(CAN2_FormatStdIDs) / sizeof(uint32_t));    
#endif


/*******************  Map Of Modules  ***********************/

//ѡ������
inline COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle)
{
    COM_ModuleID result = COM_NULL;
    
    if(Mstr_COM_UART == UartHandle->Instance)
    {
        result = COM_MSTR;
    }
    
    return result;
}

#if defined(USING_CAN)
    
//ѡ������,���һ�����������ȡ����ʽ
inline COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle, CAN_RxHeaderTypeDef *pHeader, CAN_DataID *pDataID)
{
    COM_ModuleID result = COM_NULL;
    uint32_t Cnt = 0, StdID = pHeader->StdId;
    bool isFindID = false;
    
    //������CAN1�µ����
    if(CAN1 == CanHandle->Instance)
    {
        //������ѯID
        for(Cnt = 0; Cnt < CAN1_FormatStdIDNum; Cnt++)
        {
            if(CAN1_FormatStdIDs[Cnt] == StdID)
            {
                //���Ϊ��ʽ������
                (*pDataID) = CAN_FormatData;
                
                isFindID = true;
                
                break;
            }
        }
        //����ڸ�ʽ������ID������û���ҵ�ƥ����
        if(isFindID)
        {
            //����ҵ��˸�ʽ������ID
            //����Ӧ�Ǵ��ںż������Ǹ�ʽ������ټ��ϸ�ʽ��ID�±�
            result = (COM_ModuleID)(Cnt + 2 + COM_UART_Num);
        }
        else
        {
            //������ѯID
            for(Cnt = 0; Cnt < DevGroup1StdIDNum; Cnt++)
            {
                if(CAN_DevGroup1StdIDs[Cnt] == StdID)
                {
                    //���Ϊ�豸����
                    (*pDataID) = CAN_DevData;
                    result = COM_DevGroup1;
                    isFindID = true;
                    break;
                }
            }
        }
        
    }
    else if(CAN2 == CanHandle->Instance)
    {
        //������ѯID
        for(Cnt = 0; Cnt < CAN2_FormatStdIDNum; Cnt++)
        {
            if(CAN2_FormatStdIDs[Cnt] == StdID)
            {
                //���Ϊ��ʽ������
                (*pDataID) = CAN_FormatData;
                
                isFindID = true;
                
                break;
            }
        }
        //����ڸ�ʽ������ID������û���ҵ�ƥ����
        if(isFindID)
        {
            //����ҵ��˸�ʽ������ID
            //����Ӧ�Ǵ��ںż������Ǹ�ʽ�������CAN1��ʽ�������Ŀ�ټ��ϸ�ʽ��ID�±�
            result = (COM_ModuleID)(Cnt + 2 + COM_UART_Num + CAN1_FormatStdIDNum);
        }
        else
        {
            //������ѯID
            for(Cnt = 0; Cnt < DevGroup2StdIDNum; Cnt++)
            {
                if(CAN_DevGroup2StdIDs[Cnt] == StdID)
                {
                    //���Ϊ�豸����
                    (*pDataID) = CAN_DevData;
                    result = COM_DevGroup1;
                    isFindID = true;
                    break;
                }
            }
        }
    }
    else
    {
        //�����ܵ����
    }
    
    return result;
}    
#endif


/*******************  END -> Map Of Modules  ***********************/
