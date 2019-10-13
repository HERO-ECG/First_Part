#ifndef SHOOTBOARD_H
#define SHOOTBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "MasterDataDeal.h"

//12λADC���ֵΪ4095
#define ADC_MAX_VALUE 4095
#define ADC_MIN_VALUE 0

//�ͻرȽ�������+-ERROR_BAND,0.1V,��Ӧ��Χ����1cm��
#define ERROR_BAND 60

//���԰�ָ������
typedef enum 
{
    Board_Start,        //��������������λ��������ֵ����
    Ctrl_EnterCorrect,  //����У׼ģʽ��ָ��
    Ctrl_ExitCorrect,   //�뿪У׼ģʽ��ָ��
    
}BoardCMD;

//�߽���ֵ����
extern uint16_t BoundX_Min;
extern uint16_t BoundX_Max;
extern uint16_t BoundY_Min;
extern uint16_t BoundY_Max;

extern uint16_t Trig_X;
extern uint16_t Trig_Y;

extern uint16_t ConvertBuffer[2];

extern bool isCorrectMode;

void DeInitBoard(void);
void GetInitData(void);
void StartScan(uint16_t TrigValue);
void ADC_DMA_NormalModeEnable(bool isNormal);
SampleDataTypedef GetSampleData(void);
void SaveSampleData(void);
void DealSampleData(void);
void SetTrigValue(uint16_t LowTrigValue, uint16_t HighTrigValue);
void ResetDataLog(void);

#endif

