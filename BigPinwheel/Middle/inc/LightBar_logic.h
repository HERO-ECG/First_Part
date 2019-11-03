#ifndef __LIGHT_BAR_LOGIC_H__
#define __LIGHT_BAR_LOGIC_H__

#include "stm32f10x.h"

typedef enum
{
	/*不可激活*/
	ACTIVATION_UNABLE,
	/*可激活*/
	ACTIVATION_ABLE,
	/*激活中*/
	ACTIVATION_ABLEING,
	/*已激活*/
	ACTIVATION_ENABLE
}Bigpinwheel_Status_e;

extern Bigpinwheel_Status_e g_flabellum[5];

uint8_t SetActivation(void);
void Reset_Activation(void);
uint8_t Get_Activation(void);


#endif
