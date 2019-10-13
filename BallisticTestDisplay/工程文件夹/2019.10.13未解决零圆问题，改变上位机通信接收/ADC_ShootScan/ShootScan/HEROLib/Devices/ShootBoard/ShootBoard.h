#ifndef SHOOTBOARD_H
#define SHOOTBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "MasterDataDeal.h"

//12位ADC最大值为4095
#define ADC_MAX_VALUE 4095
#define ADC_MIN_VALUE 0

//滞回比较误差带（+-ERROR_BAND,0.1V,对应范围大于1cm）
#define ERROR_BAND 60

//测试板指令请求
typedef enum 
{
    Board_Start,        //启动操作，向上位机请求阈值数据
    Ctrl_EnterCorrect,  //进入校准模式的指令
    Ctrl_ExitCorrect,   //离开校准模式的指令
    
}BoardCMD;

//边界阈值数据
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

