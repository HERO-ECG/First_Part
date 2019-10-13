#ifndef COMSTRUCTDEF_H
#define COMSTRUCTDEF_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32f1xx_hal.h"

#define USING_UART
//#define USING_CAN

//������ʹ�ù����еĴ�����м��й���
typedef enum 
{
    //�޴���
    COM_NoError = 0,
    
    //δ��ʼ��ͨ�����
    COM_Error_UnInited,
    
    //δ��ͨ�����ӳ�亯�����ҵ���Ӧ��������
    COM_Error_UnFindModule,
    
    COM_ErrorCodeNum
    
}COM_ErrorCode;

//���ﶨ����ͨ������ṹ���Լ�ͨ������ö��

typedef enum
{
    SPPRTR_UART,    //ͨ��֧�֣�supporter�� - UART
    
#if defined(USING_CAN)
    
    SPPRTR_CAN,     //ͨ��֧�֣�supporter�� - CAN  
#endif
    
}COM_Supporter;

#if defined(USING_CAN)
    
    
#endif

//ͨ������ṹ�嶨��
typedef struct
{
    //�����ѡȡ��ͨ������
    COM_Supporter   COM_type;
        
    /***************  �쳣����  ****************/
    //������
    COM_ErrorCode ErrorCode;
    
    //�Ա�����쳣������������
    char* ErrorDescription;
    
    /*************  END -> �쳣����  ************/
    
    /***************  ͨ�����ͨ��  ****************/
    //��Ǳ�����Ƿ񱻳�ʼ��������������ȫ��������Ȼ��������ʽ�ֹ���������HardFault��
    bool isInited;
    
    //�������������Ϊprintf����������ʱ������Ƿ����ڴ�ӡ����ӡ�ڼ��ֹ�����������ݡ�
    bool isPrintingData;
    
    //���ջ�������ַ(Ԥ��˫����)
    void *pRxBuffer[2];
    
    //���ջ�������־��ָʾ��ǰ���ڽ������ݵĻ�������ţ�
    uint8_t RxBufFlag;
    
    //���ͻ�������ַ
    void *pTxBuffer;
    
    //���ͻ�������С
    int32_t TxBufSize;
    
    //���ݴ�����(����ָ��)
    bool (*DealData)(void *pRxBuffer);
    
    //���ݷ��ͷ���(����ָ��)
    bool (*SendData)(uint8_t SendCMD);
    
    //���ʹ���������
    uint32_t SendCnt;
    
    //���ݰ�����������ȷ��
    uint32_t RxPackRcvCnt;
    
    //���յ��Ĵ����������
    uint32_t RxErrorPackCnt;
    
    /*********  END -> ͨ�����ͨ��  *********/
    
    /************  ����ͨ��ר��  **************/
    
    //����������� - UART
    UART_HandleTypeDef *UartHandle;
    
    //��ǰ�Ľ��ջ���������
    int32_t RxBufSize;
    
    //�´�Ӧ�����ܵ����ݳ��ȣ���������������ͷ���룩
    int32_t Rx_NextRcvLength;
    
    //��־�Ƿ����ڽ��н�������ͷ����
    bool isCorrectHead;
    
    /*********  END -> ����ͨ��ר��  *********/

    /************  CANͨ��ר��  **************/
#if defined(USING_CAN)

    //����������� - CAN
    CAN_HandleTypeDef  *CanHandle;
    
    //CAN�ķ�������֡ͷ
    CAN_TxHeaderTypeDef *pTxHeader;
    
    //CAN�Ľ�������֡ͷ
    CAN_RxHeaderTypeDef *pRxHeader;
    
    //����Ƿ����ڽ��շ�ͷ����(����Ƿ�ʹ��ͷУ�麯��)
    bool IsRcvingUnhead;
    
    //CAN�ж�ĳһ֡�����Ƿ�Ϊ���ݰ���ͷ�ķ���(����ָ��)
    bool (*Can_IsHeadData)(void *pData, uint32_t DataLength);
    
    //�������ݳ��ȼ�����
    uint32_t RxlengthCnt;
#endif
    /*********  END -> CANͨ��ר��  *********/
    
}COMInfoTypedef;

#endif
