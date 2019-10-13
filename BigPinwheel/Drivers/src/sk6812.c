#include "sk6812.h"
#include "systick.h"

void sk6812_High(void)
{
	SK6812_PORT->ODR |=	SK6812_OUT;
}
void sk6812_Low(void)
{
	SK6812_PORT->ODR &=	~SK6812_OUT;
}
/*SK6812���ų�ʼ��*/
void Sk6812_Init(void)
{
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SK6812_APB, ENABLE);								//ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = SK6812_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 					//��������
	GPIO_Init(SK6812_PORT, &GPIO_InitStructure); 							//��ʼ�� IO
}
/*����һ��rgb24λ����*/
void Sk6812_Sendone(Color_t color)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(color.r & 0x80)
			CODE1;
		else
			CODE0;
		color.r = color.r<<1;
	}
	for(i=8;i<16;i++)
	{
		if(color.g & 0x80)
			CODE1;
		else
			CODE0;
		color.g = color.g<<1;
	}
	for(i=16;i<24;i++)
	{
		if(color.b & 0x80)
			CODE1;
		else
			CODE0;
		color.b = color.b<<1;
	}
}
/*����һ��rgb����*/
void Sk6812_Sendbuff(Color_t *color_buff,uint8_t len)
{
	uint8_t i = 0;
	for(i=0;i<len;i++)
	{
		Sk6812_Sendone(color_buff[i]);
	}
}
/*��λ*/
void Sk6812_Reset(void)
{
    uint8_t  i;
    for(i=0;i<100;i++)
    {
			Systick_delay(72);
		}
}
