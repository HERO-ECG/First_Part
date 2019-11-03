#include "armour.h"
/*装甲板引脚初始化*/
void armour_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

	/*GPIO初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//使能 PORTA 时钟
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 						//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); 										//初始化 IO
//	
//	//中断线以及中断初始化配置,下降沿触发
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
//	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure); //④初始化中断线参数
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //使能按键外部中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级 1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//	NVIC_Init(&NVIC_InitStructure);									//初始化 NVIC
}
