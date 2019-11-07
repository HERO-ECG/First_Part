#include "common.h"
#include "communicate_slave.h"
#include "lightbar.h"
#include "systick.h"
#include "can.h"
#include "tim.h"
#include "armour.h"
CanTxMsg slave_txmsg;

/*can��ʼ��*/
void slave_init(void)
{
	uint8_t i=0;
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_4tq,CAN_BS2_4tq,4,CAN_Mode_Normal);
	slave_txmsg.IDE=CAN_Id_Standard; 						// ��׼֡
	slave_txmsg.RTR=CAN_RTR_Data;								// ����֡
	for(i=0;i<8;i++)
	{
		slave_txmsg.Data[i]=0;
	}
	LightBar_init();
	slave_light();
	//led��ʾ�ж�
	TIM2_Init(80);
	/*2.5s��ʱ��*/
	TIM3_Init(5000);
	/*װ�װ��ʼ��*/
	armour_init();
}
/*���ͺ���:�����Լ�ID��״̬*/
static uint8_t slave_sendMsg(CanTxMsg* TxMessage,LightStatus_e status)
{
	uint8_t mbox;
	uint16_t i=0;
	TxMessage->StdId=MASTER_ID;	TxMessage->ExtId=0;
	TxMessage->DLC=8;
	TxMessage->Data[0]=(uint8_t)(SLAVE_ID-0x300);TxMessage->Data[1]=status;
	mbox= CAN_Transmit(CAN1, TxMessage);
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //�ȴ�����
	if(i>=0xfff)return 0;
	return 1;
}
/*�ӻ�״̬��λ*/
void slave_reset(void)
{
	uint16_t i=0;uint8_t mbox;
	g_slave_status=LED_UNABLE;
	slave_txmsg.StdId=RESET_ID;	slave_txmsg.ExtId=0;
	slave_txmsg.DLC=8;
	slave_txmsg.Data[0]=(uint8_t)(RESET_ID-0x300);slave_txmsg.Data[1]=(uint8_t)(RESET_ID-0x300);
	mbox= CAN_Transmit(CAN1, &slave_txmsg);
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //�ȴ�����
}
/*�ӻ�����ɹ���������Ϣ*/
void slave_activate(void)
{
	g_slave_status=LED_ENABLE;
	slave_sendMsg(&slave_txmsg,LED_ENABLE);
}
/*����״̬��ʾ��ͬ��Ч��*/
void slave_light(void)
{
	BigPinWheel_showlight(g_slave_status);
}
/*�Զ�2.5s���������*/
void auto_activate(void)
{
	if(g_slave_status==LED_ABLEING)
	{
		Systick_delay_ms(2500);
		g_slave_status=LED_ENABLE;
		slave_sendMsg(&slave_txmsg,LED_ENABLE);
	}
}
