#ifndef COM_CANDRIVER_H
#define COM_CANDRIVER_H

#include "ComStructDef.h"

#if defined(USING_CAN)
    
#include <stdbool.h>

#include "Com_Functions.h"
#include "Com_UserModules.h"

//CAN单次数据头缓存
CAN_RxHeaderTypeDef RxHeader;
//CAN单次数据接收缓存,前8个字用于存储数据，后四个字用于存储数据ID
uint8_t CANDataBuf[12];

//处理CAN信息的方法集合
extern void (*pCANDataDealFuncs[CAN_IDsNum])(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);

//CAN相关的收发处理
void RcvUnformatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
void RcvFormatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
bool Can_SendData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size); 

#endif

#endif
