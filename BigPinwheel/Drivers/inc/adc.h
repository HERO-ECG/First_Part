#ifndef __ADC_H__
#define __ADC_H__
#include "stm32f10x.h"

void Adc_Init(void);
uint16_t Get_Adc(uint8_t ch);


#endif
