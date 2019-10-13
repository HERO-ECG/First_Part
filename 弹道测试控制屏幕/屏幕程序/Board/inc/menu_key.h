#ifndef _MENU_KEY_H_
#define _MENU_KEY_H_

#include "stm32f10x.h"

/////////////////////�����궨��////////////////////////
#define KEY_UP_ADC 0.1
#define KEY_UP 1

#define KEY_DOWN_ADC 0.83
#define KEY_DOWN 2

#define KEY_LEFT_ADC 1.68
#define KEY_LEFT 3

#define KEY_RIGHT_ADC 2.26
#define KEY_RIGHT 4

#define KEY_ENDER_ADC 2.82
#define KEY_ENDER 5

void Adc_Init(void);			//adc��ʼ��
uint16_t Get_Adc(uint8_t Ch);					//��ȡһ��adcֵ
uint16_t Get_Adc_Average(uint8_t Ch,uint8_t Times);		//��ȡ�����adc��ƽ��ֵ
int Key_detect(void);						//��ⰴ��

#endif
