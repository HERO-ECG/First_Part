#include "LightBar_logic.h"
#include "lightbar.h"
#include "adc.h"
#include <stdlib.h>

/*�����Ҷ״̬*/
Bigpinwheel_Status_e g_flabellum[5]={ACTIVATION_UNABLE,//ACTIVATION_UNABLE
																		ACTIVATION_UNABLE,
																		ACTIVATION_UNABLE,
																		ACTIVATION_UNABLE,
																		ACTIVATION_UNABLE};
/*�Ѽ�������*/
uint8_t activation_num=0;
/*��������ɣ�����adc�������ӣ��������������*/
static uint8_t GetRandomNum(uint8_t num)
{
	uint8_t random=0;
	uint8_t temp=Get_Adc(ADC_Channel_9);
	if(num==0){return 0;}
	srand(temp);
	random=rand()%num;
	return random;
}
/*������һ��������Ҷ*/
uint8_t SetActivation(void)
{
	uint8_t num;
	uint8_t i=0;
	if(activation_num>=5){return 0xFF;}
	/*��ȡ�����*/
	num=GetRandomNum(5-activation_num);
	for(i=0;i<5;i++)
	{
		if(g_flabellum[i]==ACTIVATION_ABLE)
		{
			if(num==0){break;}
			num--;
		}
	}
	/*��״̬Ϊ������*/
	g_flabellum[i]=ACTIVATION_ABLEING;
	activation_num++;
	return i;
}
/*��λ��������*/
void Reset_Activation(void)
{
	activation_num=0;
}
uint8_t Get_Activation(void)
{
	return activation_num;
}
