#include "communicate_master.h"
#include "can.h"
#include "systick.h"
#include "LightBar_logic.h"
#include "lightbar.h"
#include "adc.h"


CanTxMsg master_txmsg;
/*can��ʼ��*/
static void master_init(void)
{
	uint8_t i=0;
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_4tq,CAN_BS2_4tq,4,CAN_Mode_Normal);
	Adc_Init();
	master_txmsg.IDE=CAN_Id_Standard; 						// ��׼֡
	master_txmsg.RTR=CAN_RTR_Data;								// ����֡
	for(i=0;i<8;i++)
	{
		master_txmsg.Data[i]=0;
	}
	
}
/*���ͺ���*/
static uint8_t master_sendMsg(CanTxMsg* TxMessage,uint8_t slave_id,LightStatus_e status)
{
	uint8_t mbox;
	uint16_t i=0;
	TxMessage->Data[0]=slave_id;	TxMessage->Data[1]=status;
	TxMessage->DLC=8;
	master_txmsg.StdId=slave_id+0x300;	master_txmsg.ExtId=0; 	//IDΪ��0��1��2��3,4��+1
	mbox= CAN_Transmit(CAN1, TxMessage);
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //�ȴ�����
	if(i>=0xfff)return 0;
	return 1;
}
/*�������Ƴ�ʼ��*/

void master_control_init(void)
{
	uint16_t temp=0,mbox=0,i=0;
	/*can��ʼ��*/
	master_init();
	Systick_delay_ms(2500);
	/*������*/
	g_flabellum[0]=ACTIVATION_ABLE;g_flabellum[1]=ACTIVATION_ABLE;g_flabellum[2]=ACTIVATION_ABLE;
	g_flabellum[3]=ACTIVATION_ABLE;g_flabellum[4]=ACTIVATION_ABLE;
	
	master_txmsg.StdId=RESET_ID;	master_txmsg.ExtId=0;
	master_txmsg.DLC=8;
	master_txmsg.Data[0]=(uint8_t)(RESET_ID-0x300);master_txmsg.Data[1]=(uint8_t)(RESET_ID-0x300);
	mbox= CAN_Transmit(CAN1, &master_txmsg);
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //�ȴ�����
	
	/*��ȡ��һ���������Ҷ*/
	temp=SetActivation();
	/*���������Ϣ*/
	master_sendMsg(&master_txmsg,temp+1,LED_ABLEING);
}
/*������Ҷ״̬*/
void master_setstatus(uint8_t slave_id,LightStatus_e state)
{
	if(slave_id>0&&slave_id<6)
	{
		if(state==LED_ENABLE)
			g_flabellum[slave_id-1]=ACTIVATION_ENABLE;
	}
}

/*����ʧ������*/
void master_reset(void)
{
	uint8_t i=0;
	/*״̬��λ*/
	for(i=0;i<5;i++)
	{
		g_flabellum[i]=ACTIVATION_ABLE;
	}
	/*��������λ*/
	Reset_Activation();
}
static void master_sendfinish(void)
{
	uint8_t mbox;
	uint16_t i=0;
	master_txmsg.Data[0]=FINISH_ID-0x300;
	master_txmsg.DLC=8;
	master_txmsg.StdId=FINISH_ID;	master_txmsg.ExtId=0;
	mbox= CAN_Transmit(CAN1, &master_txmsg);
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //�ȴ�����
}
/*�����¸�������Ҷ��ͨ��*/
void master_setactivation(void)
{
	if(Get_Activation()>=5){master_sendfinish();}
	else
	{
		master_sendMsg(&master_txmsg,SetActivation()+1,LED_ABLEING);
	}
}
