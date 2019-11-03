#include "tim.h"


/*tim3��ʼ��
*ms: ��ʱʱ��
*/
void TIM3_Init(uint16_t ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ�� TIM3 ʹ��
	//��ʱ�� TIM3 ��ʼ��
	TIM_TimeBaseStructure.TIM_Period = ms*10-1; //�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ�� TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //��������ж�
//	//�ж����ȼ� NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 �ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ� 2 ��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ� 1 ��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure); //��ʼ�� NVIC �Ĵ���
	TIM_Cmd(TIM3, ENABLE); //ʹ�� TIM3
	NVIC_DisableIRQ(TIM3_IRQn);										//��ֹ�ж�
}
void TIM2_Init(uint16_t ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ�� TIM3 ʹ��
	//��ʱ�� TIM3 ��ʼ��
	TIM_TimeBaseStructure.TIM_Period = ms*10-1; //�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ�� TIM3
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //��������ж�
	//�ж����ȼ� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ� 2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ� 1 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); //��ʼ�� NVIC �Ĵ���
	TIM_Cmd(TIM2, ENABLE); //ʹ�� TIM2
	NVIC_DisableIRQ(TIM3_IRQn);	
}
/*��ʱ2.5s*/
void TIM3_start(void)
{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //��� TIM3 �����жϷ������
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //��� TIM3 �����жϱ�־
//	}
	TIM3->CNT=0;
//	NVIC_EnableIRQ(TIM3_IRQn);	
}
