#include "lightbar.h"
#include "systick.h"
/*中间灯引脚定义*/
LedGpio_t led_middle[7]={{GPIOA,GPIO_Pin_10},
												{GPIOA,GPIO_Pin_9},
												{GPIOA,GPIO_Pin_8},
												{GPIOB,GPIO_Pin_15},
												{GPIOB,GPIO_Pin_14},
												{GPIOB,GPIO_Pin_13},
												{GPIOB,GPIO_Pin_12}};
LedGpio_t led_out[2]={{GPIOA,GPIO_Pin_0},
												{GPIOA,GPIO_Pin_1}};
void test(void)
{
	while(1){
	((led_middle[0]).gpio)->ODR |= ((led_middle[0]).pin);
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	((led_middle[0]).gpio)->ODR &= ~((led_middle[0]).pin);							
	}
}							
/*灯条端口初始化*/
void LightBar_init(void)
{
	uint8_t i=0;
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);								//使能端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);								//使能端口时钟
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 										//复用推挽
	for(i=0;i<7;i++)
	{
		GPIO_InitStructure.GPIO_Pin = led_middle[i].pin;
		GPIO_Init(led_middle[i].gpio, &GPIO_InitStructure); 							//初始化 IO
	}
	for(i=0;i<2;i++)
	{
		GPIO_InitStructure.GPIO_Pin = led_out[i].pin;
		GPIO_Init(led_out[i].gpio, &GPIO_InitStructure); 									//初始化 IO
	}
}



/*显示一个灯条对应的图案*/
static void LightBar_show_arrow(uint8_t status,LedGpio_t *led)
{
	uint8_t i=0;
	/*6个灯为循环*/
	for(;i<64/6+1;i++)
	{
		Sk6812_SendRed(status&0x20,led);
		Sk6812_SendRed(status&0x10,led);
		Sk6812_SendRed(status&0x08,led);
		Sk6812_SendRed(status&0x04,led);
		Sk6812_SendRed(status&0x02,led);
		Sk6812_SendRed(status&0x01,led);
//		if(status&0x20){status=status<<1;	status |= 0x01;}
//		else{status=status<<1;}
	}
}
/*显示流水箭头*/
void BigPinWheel_show_runarrow(void)
{
	/*每个灯条对应显示的图案*/
	static uint8_t status[7]={0x07,0x0E,0x1C,0x38,0x1C,0x0E,0x07};
	uint8_t i=0;
	LightBar_show_arrow(status[0],&led_middle[0]);
	LightBar_show_arrow(status[1],&led_middle[1]);
	LightBar_show_arrow(status[2],&led_middle[2]);
	LightBar_show_arrow(status[3],&led_middle[3]);
	LightBar_show_arrow(status[4],&led_middle[4]);
	LightBar_show_arrow(status[5],&led_middle[5]);
	LightBar_show_arrow(status[6],&led_middle[6]);
	//图案平移
	for(i=0;i<7;i++)
	{
		if(status[i]&0x20){status[i]=status[i]<<1;	status[i] |= 0x01;}
		else{status[i]=status[i]<<1;}
	}
	/*目前中断调用此函数，不用延时*/
	//Systick_delay_ms(80);
}

/*大风车扇叶全亮或全灭*/
void BigPinWheel_showlight(LightStatus_e status)
{
	uint16_t i=0,j=0;
	Color_t led_color={0,0,0};
	if(status==LED_ABLEING)
	{
		BigPinWheel_show_runarrow();
		for(i=0;i<300;i++)
		{
			Sk6812_Sendone(led_color,&led_out[0]);
		}
		led_color.r=20;
	}
	else
	{
		if(status==LED_UNABLE){}
		else if(status==LED_ENABLE || status==LED_FINISH){led_color.r=20;}
		/*熄灭中间灯条*/
		for(i=0;i<7;i++)
		{
			for(j=0;j<64;j++)
			{
				Sk6812_Sendone(led_color,&led_middle[i]);
			}
		}
				/*外部灯条*/
		for(i=0;i<300;i++)
		{
			Sk6812_Sendone(led_color,&led_out[0]);
		}
	}
		for(i=0;i<14;i++)
	{
		Sk6812_Sendone(led_color,&led_out[1]);
	}
}
