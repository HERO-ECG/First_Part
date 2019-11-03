#ifndef __CAN_H__
#define __CAN_H__
#include "stm32f10x_can.h"

#define CAN_RX0_INT_ENABLE 1

extern uint8_t databuff[8];


extern uint8_t CAN_Mode_Init(uint8_t sjw, uint8_t bs1, uint8_t bs2, uint8_t brp, uint8_t mode);
extern uint8_t CAN_SendMsg(uint8_t *msg, uint8_t length);
extern uint8_t Can_Receive_Msg(uint8_t *buf);




#endif
