
#include "stm32f10x.h"
#include "sk6812.h"
#include "systick.h"
#include "tim.h"
#include "can.h"
#include "lightbar.h"
#include "LightBar_logic.h"
#include "adc.h"
#include "communicate_master.h"
#include "communicate_slave.h"
#include "common.h"
Color_t color={127,50,60};
uint8_t canbuff[8]={1,2,3,4,5,6,7,8};
int main(void)
{
	#ifdef SLAVE
	uint32_t time_50ms=0;
	slave_init();
	while(1)
	{
		//auto_activate();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0)
		{
			Systick_delay_ms(1);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0)
			{
//					TIM3->CNT=0;
//					NVIC_DisableIRQ(TIM3_IRQn);;
				switch (g_slave_status)
				{
					case LED_UNABLE:
						slave_reset();
						Systick_delay_ms(15);
						break;
					case LED_ABLEING:
						if((TIM3->CNT)<=25000-1)
							{
								slave_activate();
								TIM3->CNT=0;
								time_50ms=1;
							}
						else{slave_reset();}
						break;
					case LED_ENABLE:
						if(time_50ms==1 && TIM3->CNT<=300){break;}
						slave_reset();
						break;
					case LED_FINISH:break;
				}
			}
		}
		else if(g_slave_status==LED_ABLEING && (TIM3->CNT)>25000-1)
		{
			slave_reset();
		}
		if(time_50ms==1 && TIM3->CNT>300){time_50ms=0;}
	}
	#else
	master_control_init();
	while(1)
	{
	}
	#endif
//	LightBar_init();
//	BigPinWheel_showlight(LED_UNABLE);
//	while(1);
}

//#if CAN_RX0_INT_ENABLE 															//使能 RX0 中断
//uint8_t databuff[8]={0};
////中断服务函数
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//	Can_Receive_Msg(databuff);
//}
//#endif
#ifdef SLAVE
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return;//没有接收到数据,直接退出
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); //读取数据
	if(RxMessage.IDE == CAN_Id_Standard)
	{
		if(RxMessage.StdId==FINISH_ID){g_slave_status=LED_FINISH;}							//能量机关激活完成
		else if(RxMessage.StdId==RESET_ID)						//激活失败
		{
			g_slave_status=LED_UNABLE;
			slave_light();
		}
		else if(RxMessage.StdId==SLAVE_ID)
		{
			g_slave_status=(LightStatus_e)RxMessage.Data[1];
			/*开始计时*/
			TIM3_start();
		}
	}
}
///*TIM3 中断
//*2.5s计时
//*/
//void TIM3_IRQHandler(void) 
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
//		slave_reset();
//		NVIC_DisableIRQ(TIM3_IRQn);	
//	}
//}
void TIM2_IRQHandler(void) 
{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查 TIM2 更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); //清除 TIM2 更新中断标志
		/*根据状态显示不同灯*/
		slave_light();
	}
}
///*中断*/
//void EXTI9_5_IRQHandler(void)
//{
//	EXTI_ClearITPendingBit(EXTI_Line9); //清除 LINE9 上的中断标志位
//	TIM3->CNT=0;
//	NVIC_DisableIRQ(TIM3_IRQn);	
//	if(g_slave_status==LED_FINISH){}
//	else if(g_slave_status==LED_ABLEING)			//如果可处于正在激活
//	{
//		slave_activate();									//告诉主机激活成功
//	}
//	else
//	{
//		slave_reset();										//复位,并发送复位消息
//	}
//}
#else
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return;//没有接收到数据,直接退出
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); //读取数据
	if(RxMessage.IDE == CAN_Id_Standard)
	{
		if(RxMessage.StdId==RESET_ID)						//激活失败
		{
			master_reset();										//复位主机状态
			master_setactivation();						//启动下一个待激活
		}
		else if(RxMessage.StdId==MASTER_ID)		//状态传输信号
		{
			//设置主机内单个扇叶状态
			master_setstatus(RxMessage.Data[0],(LightStatus_e)RxMessage.Data[1]);
			master_setactivation();
		}
	}
}
#endif
