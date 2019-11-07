#include "moto_6020.h"
#include "can.h"

/*读取电机信号*/
void Get_3508(Moto6020_par_t* moto,CanRxMsg *RxMessage)
{
	moto->angle=(uint16_t)(RxMessage->Data[0]<<8 | RxMessage->Data[1]);
	moto->speed=(uint16_t)(RxMessage->Data[2]<<8 | RxMessage->Data[3]);
	moto->current=(uint16_t)(RxMessage->Data[4]<<8 | RxMessage->Data[5]);
	moto->temperature=RxMessage->Data[6];
}
void Pid_ControlSpeed(void)
{
	
}
void Pid_ControlCurrent(void)
{
	
}
