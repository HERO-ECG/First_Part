#ifndef __SK6812_H__
#define __SK6812_H__

#include "stm32f10x.h"
/*Òý½Åºê¶¨Òå*/
#define SK6812_APB	RCC_APB2Periph_GPIOA
#define SK6812_PORT	GPIOA
#define SK6812_OUT	GPIO_Pin_0

#define SK6812_HIGH(ledgpio)	((ledgpio).gpio)->ODR |= ((ledgpio).pin)
#define SK6812_LOW(ledgpio)		((ledgpio).gpio)->ODR &= ~((ledgpio).pin)


#define CODE0(ledgpio)								do{SK6812_HIGH(ledgpio);	\
																			SK6812_LOW(ledgpio);	\
																			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
																			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
																			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
																			}while(0)
#define CODE1(ledgpio)								do{SK6812_HIGH(ledgpio);	\
																			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
																			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
																			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
																			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
																			SK6812_LOW(ledgpio);	\
																			}while(0)
typedef struct
{
	GPIO_TypeDef * gpio;
	uint16_t pin;
}LedGpio_t;

typedef struct 
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}Color_t;


extern void Sk6812_Sendone(Color_t color,LedGpio_t *led);
extern void Sk6812_SendRed(uint8_t red,LedGpio_t *led);
extern void Sk6812_Sendbuff(Color_t *color_buff,uint8_t len,LedGpio_t *led);
extern void Sk6812_Reset(void);
	
#endif
