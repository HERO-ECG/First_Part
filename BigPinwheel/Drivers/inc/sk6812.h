#ifndef __SK6812_H__
#define __SK6812_H__

#include "stm32f10x.h"
/*引脚宏定义*/
#define SK6812_APB	RCC_APB2Periph_GPIOB
#define SK6812_PORT	GPIOB
#define SK6812_OUT	GPIO_Pin_10
/*接口宏*/
#define Delay_sk6812(time)	Systick_delay(time)									//72为1us									

#define CODE0								do{sk6812_High();	\
															sk6812_Low();	\
															sk6812_Low();	\
															sk6812_Low();	\
															}while(0)
#define CODE1								do{sk6812_High();	\
															sk6812_High();	\
															sk6812_Low();	\
															sk6812_Low();	\
															}while(0)

typedef struct 
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}Color_t;
extern void sk6812_High(void);
extern void sk6812_Low(void);
extern void Sk6812_Init(void);
extern void Sk6812_Sendone(Color_t color);
extern void Sk6812_Sendbuff(Color_t *color_buff,uint8_t len);
extern void Sk6812_Reset(void);
	
#endif
