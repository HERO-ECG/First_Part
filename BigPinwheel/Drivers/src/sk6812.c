#include "sk6812.h"
#include "systick.h"

//void SK6812_HIGH(LedGpio_t ledgpio)
//{
//	((ledgpio).gpio)->ODR |= ((ledgpio).pin);
//}
//void SK6812_LOW(LedGpio_t ledgpio)
//{
//	((ledgpio).gpio)->ODR &= ~((ledgpio).pin);
//}
/*发送一个rgb24位数据*/
void Sk6812_Sendone(Color_t color,LedGpio_t *led)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(color.g & 0x80)
		{CODE1((*led));}
		else
		{CODE0((*led));}
		color.g = color.g<<1;
	}
	for(i=8;i<16;i++)
	{
		if(color.r & 0x80)
		{CODE1((*led));}
		else
		{CODE0((*led));}
		color.r = color.r<<1;
	}
	for(i=16;i<24;i++)
	{
		if(color.b & 0x80)
		{CODE1((*led));}
		else
		{CODE0((*led));}
		color.b = color.b<<1;
	}
}

/*发送一个红色24位数据*/
void Sk6812_SendRed(uint8_t red,LedGpio_t *led)
{
	uint8_t i;Color_t color={0,0,0};
	if(red>0){color.r=20;color.g=0;color.b=0;}
	for(i=0;i<8;i++)
	{
		if(color.g & 0x80)
		{CODE1((*led));}
		else
		{CODE0((*led));}
		color.g = color.g<<1;
	}
	for(i=8;i<16;i++)
	{
		if(color.r & 0x80)
		{CODE1((*led));}
		else
		{CODE0((*led));}
		color.r = color.r<<1;
	}
	for(i=16;i<24;i++)
	{
		if(color.b & 0x80)
		{CODE1((*led));}
		else
		{CODE0((*led));}
		color.b = color.b<<1;
	}
	SK6812_LOW(*led);
}
/*发送一组rgb数据*/
void Sk6812_Sendbuff(Color_t *color_buff,uint8_t len,LedGpio_t *led)
{
	uint8_t i = 0;
	for(i=0;i<len;i++)
	{
		Sk6812_Sendone(color_buff[i],led);
	}
}
/*复位*/
void Sk6812_Reset(void)
{
			Systick_delay_us(70);
}
