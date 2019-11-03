#include "common.h"
#include "communicate_slave.h"
#include "lightbar.h"
#include "systick.h"
#include "can.h"
#include "tim.h"
#include "armour.h"
CanTxMsg slave_txmsg;

/*can初始化*/
void slave_init(void)
{
	uint8_t i=0;
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_4tq,CAN_BS2_4tq,4,CAN_Mode_Normal);
	slave_txmsg.IDE=CAN_Id_Standard; 						// 标准帧
	slave_txmsg.RTR=CAN_RTR_Data;								// 数据帧
	for(i=0;i<8;i++)
	{
		slave_txmsg.Data[i]=0;
	}
	LightBar_init();
	slave_light();
	//led显示中断
	TIM2_Init(80);
	/*2.5s定时器*/
	TIM3_Init(5000);
	/*装甲板初始化*/
	armour_init();
}
/*发送函数:发送自己ID和状态*/
static uint8_t slave_sendMsg(CanTxMsg* TxMessage,LightStatus_e status)
{
	uint8_t mbox;
	uint16_t i=0;
	TxMessage->StdId=MASTER_ID;	TxMessage->ExtId=0;
	TxMessage->DLC=8;
	TxMessage->Data[0]=(uint8_t)(SLAVE_ID-0x300);TxMessage->Data[1]=status;
	mbox= CAN_Transmit(CAN1, TxMessage);
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //等待结束
	if(i>=0xfff)return 0;
	return 1;
}
/*从机状态复位*/
void slave_reset(void)
{
	uint16_t i=0;uint8_t mbox;
	g_slave_status=LED_UNABLE;
	slave_txmsg.StdId=RESET_ID;	slave_txmsg.ExtId=0;
	slave_txmsg.DLC=8;
	slave_txmsg.Data[0]=(uint8_t)(RESET_ID-0x300);slave_txmsg.Data[1]=(uint8_t)(RESET_ID-0x300);
	mbox= CAN_Transmit(CAN1, &slave_txmsg);
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //等待结束
}
/*从机激活成功，发送消息*/
void slave_activate(void)
{
	g_slave_status=LED_ENABLE;
	slave_sendMsg(&slave_txmsg,LED_ENABLE);
}
/*根据状态显示不同灯效果*/
void slave_light(void)
{
	BigPinWheel_showlight(g_slave_status);
}
/*自动2.5s激活，测试用*/
void auto_activate(void)
{
	if(g_slave_status==LED_ABLEING)
	{
		Systick_delay_ms(2500);
		g_slave_status=LED_ENABLE;
		slave_sendMsg(&slave_txmsg,LED_ENABLE);
	}
}
