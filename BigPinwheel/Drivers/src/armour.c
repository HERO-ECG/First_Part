#include "armour.h"
/*װ�װ����ų�ʼ��*/
void armour_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

	/*GPIO��ʼ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//ʹ�� PORTA ʱ��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 						//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 										//��ʼ�� IO
//	
//	//�ж����Լ��жϳ�ʼ������,�½��ش���
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
//	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure); //�ܳ�ʼ���ж��߲���
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //ʹ�ܰ����ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ� 1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ� 2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);									//��ʼ�� NVIC
}
