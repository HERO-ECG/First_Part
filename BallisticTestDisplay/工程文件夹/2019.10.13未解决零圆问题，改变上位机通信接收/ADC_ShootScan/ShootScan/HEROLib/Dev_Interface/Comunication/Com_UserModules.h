#ifndef COM_USERMODULES_H
#define COM_USERMODULES_H

#include "ComStructDef.h"

//���ʵ���б���Ӧ��˳��ָ�����鸳ֵ�����ڹ���
typedef enum 
{
    //��ͨ������ṹ�壬���ڱ�ʶ��Map������δ�ҵ���������
    COM_NULL = 0x00,
    
    /*  ʹ�ô��ڵ�ͨ�����  */    
    //���ƶ�ͨ�����
    COM_MSTR,
    
    //ʹ�ô��ڵ�ͨ���������
    COM_UART_Num,
    
    /*  ʹ��CAN��ͨ�������˳���ȷǸ�ʽ�����ʽ��  */
#if defined(USING_CAN)
    
    //CAN�豸��1
    COM_DevGroup1 = COM_UART_Num,
    
    //CAN�豸��2
    COM_DevGroup2,
    
    //���ͨ�����
    COM_BSP,
#endif
    
    COM_ModulesNum
    
}COM_ModuleID;

#if defined(USING_CAN)
    
//���can����������ӳ�䣬������ʽ�����ݺͷǸ�ʽ������
typedef enum 
{
    //�Զ���ͨЭ������
    CAN_FormatData,
    
    //�豸�ش�����
    CAN_DevData,
    
    CAN_IDsNum
    
}CAN_DataID;   

//���Զ���ͨ��Э�鷢�͵�����ID����
extern const uint32_t CAN_FormatStdIDs[];
//�Է��Զ���ͨ��Э�鷢�͵�����ID����
extern const uint32_t CAN_UnformatStdIDs[];

COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle, CAN_RxHeaderTypeDef *pHeader, CAN_DataID *pDataID);
#endif

//����ͱ������printf��ͨ��������
#define COM_PRINTF_ID COM_MSTR

//���ڼ��й����ͨ���������
extern COMInfoTypedef *COM_Modules[COM_ModulesNum];

//�û����ӳ��
COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle);

#endif
