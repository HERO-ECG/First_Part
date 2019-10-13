#ifndef __SK6812_H__
#define __SK6812_H__

#include "stm32f10x.h"
/*引脚宏定义*/
#define SK6812_APB	RCC_APB2Periph_GPIOB
#define SK6812_PORT	GPIOB
#define SK6812_OUT	GPIO_Pin_10
/*接口宏*/

#define CODE0								do{SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
															}while(0)
#define CODE1								do{SK6812_PORT->ODR|=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR |=	SK6812_OUT;	\
															SK6812_PORT->ODR &=	~SK6812_OUT;	\
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

#define BLUE 		(Color_t){0,0,15}
#define YELLOW 	(Color_t){15,12,0}
#define ORANGE 	(Color_t){15,3,10}
#define RED 		(Color_t){15,0,0}
#define GREEN 		(Color_t){0,15,0}
#define PURPLE 		(Color_t){60,0,15}
#define PINK 		(Color_t){15,2,8}
#define WEAK 		(Color_t){1,1,1}
	
#endif
