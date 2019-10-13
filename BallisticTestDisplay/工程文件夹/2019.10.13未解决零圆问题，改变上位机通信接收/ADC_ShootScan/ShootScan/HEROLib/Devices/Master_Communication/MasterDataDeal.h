#ifndef MASTERDATADEAL_H
#define MASTERDATADEAL_H

#include "ComToMaster.h"

//·¢ËÍÊý¾Ý»º´æ
extern SampleCMDTypedef CtrlCMD;
extern SampleDataTypedef SampleData;

bool GetTxData(union MstrTxUnionType *pUnion, uint8_t *pDataType);
bool GetMsgFromMstrBuffer(Mstr_RxBufTypedef *pBuffer);

#endif
