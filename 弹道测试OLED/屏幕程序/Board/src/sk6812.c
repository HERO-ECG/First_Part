#include "sk6812.h"
#include "delay.h"
int a;
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
		if(color.g<<i & 0x80)
		{
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
		}
		else
		{
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
		}
	}
	for(i=0;i<8;i++)
	{
		if(color.r<<i & 0x80)
			{
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
		}
		else
		{
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
		}
}
	for(i=0;i<8;i++)
	{
		if(color.b<<i & 0x80)
			{
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
		}
		else
		{
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR |=	SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
			SK6812_PORT->ODR &=	~SK6812_OUT;
		}
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
			delay_us(1);
		}
}
