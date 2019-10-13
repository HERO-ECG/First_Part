
#include "stm32f10x.h"
#include "sk6812.h"
#include "systick.h"
#include "can.h"
Color_t color={70,0,60};
uint8_t canbuff[8]={1,2,3,4,5,6,7,8};
uint8_t databuff[8]={0};
uint8_t status=0;
int main(void)
{
//	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_8tq,4,CAN_Mode_LoopBack);
//	if(CAN_SendMsg(canbuff,8))
//	{
//		status=1;
//	}
//	Can_Receive_Msg(databuff);
	Sk6812_Init();
	while(1)
	{
			/*点亮400个灯*/
			for(uint16_t i=0;i<2;i++)
			{
				Sk6812_Sendone(color);
			}
			/*控制一组后延时*/
			Sk6812_Reset();
			/*改变颜色*/
			if(color.r<123){color.r++;}
			else{color.r=0;}
			if(color.g<123){color.g++;}
			else{color.g=0;}
			if(color.b<123){color.b++;}
			else{color.b=0;}
	}
}
