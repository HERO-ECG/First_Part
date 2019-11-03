#ifndef __LIGHT_BAR_LOGIC_H__
#define __LIGHT_BAR_LOGIC_H__

#include "stm32f10x.h"

typedef enum
{
	/*���ɼ���*/
	ACTIVATION_UNABLE,
	/*�ɼ���*/
	ACTIVATION_ABLE,
	/*������*/
	ACTIVATION_ABLEING,
	/*�Ѽ���*/
	ACTIVATION_ENABLE
}Bigpinwheel_Status_e;

extern Bigpinwheel_Status_e g_flabellum[5];

uint8_t SetActivation(void);
void Reset_Activation(void);
uint8_t Get_Activation(void);


#endif
