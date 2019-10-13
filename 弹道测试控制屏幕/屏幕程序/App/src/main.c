#include "stm32f10x.h"
#include "delay.h"
#include "oled.h"
#include "menu.h"
#include "menu_key.h"
#include "can.h"
#include "sk6812.h"
void TIM3_Int_Init(uint16_t arr,uint16_t psc);
void TIM3_IRQHandler(void);
/**********************718创新实验室开发板例程*********************
*  编写：718创新实验室
*  平台：STM32F103ZET6
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。
******************************************************************/
Color_t color[2];
int req=0;
/*************************功能说明********************************
	将变量建立菜单，存入flash，通过菜单和按键调整参数

	单片机里的程序和数据都是存储在flash里的,读写flash一定要谨慎,
	否则容易出现严重的故障，故对菜单的使用一定要搞清楚
*************************功能说明********************************/
Menu roSpd,bltSpd, FIRE, Count, bltNum,freq;
int main()
{
	   delay_init();							//初始化延时
	Sk6812_Init();
	color[0]=YELLOW;
	color[1]=PINK;
    //这些是我们假定要调的参数
    double rollSpeed = 18.1;
    double *bulletSpeed ;
    double fire = 0;
    double count = 0;
    double Bullet_num=1.0;
		double bltfreq=1.0;
	TIM3_Int_Init(999,7199);																//定时器中断，握手信号
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级

    //有几个参数就创建几个相应的结构体

    Menu_Init();																					//初始化菜单
    //对这些结构体依次赋值并建立连接（具体去函数定义处menu.c看说明！！！）
	
    //只读参数
    AddMenu("show_Spd",bulletSpeed,0.05,&bltSpd);					//弹丸速度显示
    AddMenu("show_num",&count,0.56,&Count);								//弹丸计数

    //可读写参数
    AddMenu("set_Spd",&rollSpeed,1,&roSpd);								//摩擦轮速度设定
    AddMenu("fire",&fire,1,&FIRE);												//发射弹丸
    AddMenu("set_Num",&Bullet_num,1.0,&bltNum);						//每次发射弹丸数量
		AddMenu("set_freq",&bltfreq,1,&freq);									//射频设定
		
		
    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0);////CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式波特率500Kbps
    bulletSpeed=(double*)malloc(8);
		
		Sk6812_Sendone(color[0]);
		Sk6812_Sendone(color[1]);
    Menu_ShowPar();																				//菜单显示及调整参数
}
    /*************************说明********************************
    	有几个需要调的参数就使用Menu建立几个结构体.并使用AddMenu函数
    将结构体与参数一对一连接起来；
    	记得用Menu_Init()先初始化，做好上述准备后，调用Menu_ShowPar()
    函数即可进入菜单调参模式；

    **************************说明********************************/
void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


    TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	static Color_t tmp_color[2];
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志
        Can_Send_Msg("abcd",4);
			if(req<10)
			req++;
			if(req>3)
			{
				color[0]=YELLOW;
			color[1]=PINK;
			}
			if((color[1].r==tmp_color[1].r)&&(color[0].r==tmp_color[0].r)
				&&(color[1].g==tmp_color[1].g)&&(color[0].g==tmp_color[0].g)
			&&(color[1].b==tmp_color[1].b)&&(color[0].b==tmp_color[0].b))
			return;
		Sk6812_Sendone(color[0]);
		Sk6812_Sendone(color[1]);
			Sk6812_Reset();
			tmp_color[0]=color[0];
			tmp_color[1]=color[1];
    }
}
