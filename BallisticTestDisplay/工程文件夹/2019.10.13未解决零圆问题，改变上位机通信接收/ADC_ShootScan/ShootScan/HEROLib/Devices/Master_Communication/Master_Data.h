#ifndef MASTER_DATA_H
#define MASTER_DATA_H

//���ļ��ж����û�ͨ����ص��������ͣ�������Ҫ���СΪ���ͱ�����������
//��Լ����0��ָ��Ϊ��ָ���ָʾ�����ָ��

#include <stdint.h>
#include <stdbool.h>

//�ṹ�嶨�壺ͨ������ͷ
typedef struct
{
    uint8_t DataHead1;  //�̶�����ͷ1:0xaa
    uint8_t DataHead2;  //�̶�����ͷ2:0xbb
    uint8_t MsgTypeID;  //�������ͱ�ʶ����ָ����������
    uint8_t IDFeedback; //����λ�������ո������յ���ָ������
    
    uint32_t Verification;  //��������λ����ʶͷ��ݵ�ͷУ�飨���У�飩
    
}MstrData_DataHead; 

//�ṹ�嶨�壺ͨ������β
typedef struct
{
    uint32_t Verification;  //У�飬��ʱΪ���
    
}MstrData_DataTail; 

/***************  ���¶����û���������  ********************/

//ö�ٶ��壺������������ID
enum Mstr_TxMsgTypes
{
    //0 - ����ָ�����
    MstrTx_ErrorData = 0,
    
    //1 - ��ָ���
    MstrTx_EmptyData,
    
    //2 - ����ָ��
    MstrTx_CtrlCMD,
    
    //3 - ��������
    MstrTx_SampleData,
    
    //4 - У׼��������
    MstrTx_CorrectData,
    
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    MstrTx_Num
};

//ö�ٶ��壺������������ID
enum MstrRxMsgTypes
{
    //0 - ����ָ�����
    MstrRx_ErrorCMD = 0,
    
    //1 - ��ָ��
    MstrRx_EmptyCMD,
    
    //2 - ����ָ��
    MstrRx_CtrlCMD,
    
    //3 - ��ʼ������
    MstrRx_InitCMD,
                
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    MstrRx_Num
};

/****************************  ����1 ����ָ��������  ********************************/

#define SEND_BUF_SIZE 6

//�ṹ�嶨�壺�������ݰ�
typedef struct
{
    uint16_t DataSize;      //���ݸ���
    uint16_t Reserve;       //Ԥ��ռλ��
    
    //0ΪX��1λ
    uint16_t SampleBuffer[SEND_BUF_SIZE][2];
    
}SampleDataTypedef;
 
//�ṹ�嶨�壺��ʼ�����ݰ�
typedef struct
{
    uint16_t X_Max_Voltage;    //X���
    uint16_t X_Min_Voltage;    //X��С
    uint16_t Y_Max_Voltage;    //Y���
    uint16_t Y_Min_Voltage;    //Y��С
    
    uint16_t X_TrigValue;   //X���򴥷�����
    uint16_t Y_TrigValue;   //X���򴥷�����
    
}InitDataTypedef;

//�ṹ�嶨�壺��������ָ��
typedef struct
{
    uint32_t CMD;       //ָ��
    
}SampleCMDTypedef;

/****************************  END -> ����ָ��������  ********************************/

#endif
