/**********************718创新实验室开发板例程*********************
*  编写：718创新实验室
*  平台：STM32F103ZET6
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。
******************************************************************/

#include "stm32f10x.h"
#include "menu.h"
#include "flash.h"
#include "menu_key.h"
#include "oled.h"
#include "delay.h"
#include "can.h"

extern 	Menu roSpd,bltSpd, FIRE, Count, bltNum;

//Menu MenuHead= {NULL,0,"quit",0,NULL,NULL};				//退出项
Menu Read= {NULL,0,"**read",0,NULL,NULL};						//读flash项
Menu Write= {NULL,0,"**write",0,&Read,&Read};					//写入flash项


/**
 *@brief      	menu结构体获取参数名字
 *@param      	par_name:字符串地址
								name:字符数组
 *@SampleUsage 	GetName("718lab",menu->name);
 **/
void GetName(const char *par_name, char *name)
{
    int i = 0;
    for (i=0; i<MENUSIZE; i++) //将名字分解成字符，依此赋值
    {
        if (*(par_name + i) == '\0')
        {
            name[i] = '\0';
            break;
        }
        else {
            name[i] = *(par_name + i);
        }
    }
    name[i] = '\0';
}


/**
 *@brief      	menu结构体获取参数所有信息，即名字，地址，递增量
 *@param      	par_name：字符串地址
								par：参数地址
								menu：结构体地址
								add：每次的递增量
 *@SampleUsage  GetPar("718lab", &a, add, &menu);
 **/
void GetPar(const char *par_name, double *par,double add,Menu *menu )
{
    menu->par = (double*)par;				//结构体获取参数地址
    menu->add = add;
    GetName(par_name,menu->name);			//获取名字
}


/**
 *@brief      	创作一个菜单项
 *@param      	par_name：字符串地址
								par：参数地址
								add：每次的递增量
								menu：结构体地址
 *@SampleUsage  CreateMenu(“718lab”,&a,add,par_menu);
 **/
void CreateMenu(const char *par_name, double *par, double add,Menu *menu)
{
    //Menu *menu = (Menu *)malloc(sizeof(Menu));
    static int i=0;														//记录第几个菜单
    i++;
    Menu *last=Write.up;//上一个添加的菜单项
    GetPar(par_name, par,add,menu);

    //连接菜单项
    menu->up = last;
    menu->down = &Write;
    Write.up=menu;
    last->down=menu;
    menu->num=i;

}

/**
 *@brief      	 连接一个菜单项
 *@param      	 par_name：字符串地址
								 par：参数地址
								 add：每次的递增量
								 par_menu：结构体地址
 *@SampleUsage   CreateMenu(“718lab”,&a,add,&menupar);
 **/
void AddMenu(const char *par_name, double *par,double add,Menu *par_menu)
{
    CreateMenu(par_name, par,add,par_menu);
//    Menu *p=&Write;
//    Menu *menu = par_menu;
//    CreateMenu(par_name,par,add,par_menu);
//    while (p->down!=NULL)
//    {
//        p = p->down;
//    }
//    p->down = menu;					//上下指针连接
//    menu->up = p;
}
Menu* creatList(int num,char *names[],double *values,double *add)
{
    int i;
    Menu* result;
    result=(Menu*)malloc(sizeof(Menu));
    for(i=0; i<num; i++)
    {
        CreateMenu(names[i],&values[i],add[i],&result[i]);
    }
    return result;
}
/**
 *@brief      	 显示数字
 *@param      	 p：菜单项地址
								 y：显示位置
 *@SampleUsage   printnum(&menu,2)
 **/
void printnum(double *p,int y)
{
    int i=0,n=0;
    double a=*(p)>0?*(p):-*(p);
    uint8_t temp=((int)a)%10;
    if(a-(int)a==0)
    {
        if(a==0)
        {
            OLED_ShowChar(127-8,y,'0',16);
        }
        else
        {
            while(i++<9&&a!=0)
            {
                temp=((int)a)%10;
                OLED_ShowChar(127-i*8,y,temp+'0',16);
                a=(a-temp)/10;
            }
        }
    }
    else {
        a=(int)(a/0.001);
        for(i=1; i<4; i++)
        {
            temp=((int)a)%10;
            if(temp!=0||n>0)
            {
                n++;
                OLED_ShowChar(127-n*8,y,temp+'0',16);
            }
            a=(a-temp)/10;
        }
        i=++n;

        if (n>1)
        {
            OLED_ShowChar(127-i*8,y,'.',16);
        }

        do {
            i++;
            temp=((int)a)%10;
            OLED_ShowChar(127-i*8,y,temp+'0',16);
            a=(a-(int)temp)/10;
        } while(i<9&&a!=0);
    }
    if(*(p)<0)
    {
        OLED_ShowChar(127-(i+1)*8,y,'-',16);
    }
}

/**
 *@brief      	  显示数字
 *@param      	  p：菜单项地址
									p_now：选中的菜单项地址
									y：显示位置
 *@SampleUsage    PrintMenu(&menu,&menu_now,2)
 **/
void PrintMenu(Menu *p,Menu *p_now,int y)
{
    int i = 0;
    int x=0;
    if(p==p_now)
    {
        OLED_ShowChar(x,y,'-',16);
        x+=8;
        OLED_ShowChar(x,y,'>',16);
        x+=8;
    }
    for (; i<SIZE; i++)
    {
        if (p->name[i] != '\0')
        {
            OLED_ShowChar(x,y,p->name[i],16);
            x+=8;
        }
        else
        {
            break;
        }
    }
    if(p->par!=NULL)
    {
        printnum(p->par,y);
    }
}


/**
 *@brief      	   显示菜单项
 *@param      	   p_now：选中的菜单项地址
 *@SampleUsage     PrintPar(&menu_now)
 **/
void PrintPar(Menu *p_now)
{
    int x=0,i=0;
    OLED_Clear();
    for (; i<SIZE; i++)
    {
        if (p_now->name[i] != '\0')
        {
            OLED_ShowChar(x,2,p_now->name[i],16);
            x+=8;
        }
        else {
            break;
        }
    }
    printnum(p_now->par,4);
}

/**
 *@brief      	打印一页菜单
 *@param      	p_now：选中的菜单项地址
								serial：选中的菜单为第几个
 *@SampleUsage  PrintAllMenu(&menu_now,5)
 **/
void PrintAllMenu(Menu *p_now,int serial)
{
    int i=0;
    Menu *p = p_now;
    serial--;
    serial=serial%4+1;
//    for(; i<serial-1; i++)			//寻找一页的第一项菜单
//    {
    p=p->up;
//    }
    OLED_Clear();
    for(i=0; i<4; i++)					//依此打印
    {
        PrintMenu(p,p_now,i*2);
        if(p->down==NULL)
        {
            break;
        }
        else {
            p=p->down;
        }
    }
}
/**
 *@brief      	删除菜单项
 *@param      	head：结点地址
 *@SampleUsage  DeleteMenu(&menu_now);
 **/
/*void DeleteMenu(Menu *head)
{
	if (head->down!=NULL)
	{
		DeleteMenu(head->down);
	}
	else {
		free(head);
	}
}*/

/**
 *@brief      	改变参数值
 *@param      	menu：菜单项地址
 *@SampleUsage  Menu_AdjustPar(&menu)
 **/
void Menu_AdjustPar(Menu *menu)
{
    int state=1;
    PrintPar(menu);
    while(1)
    {
        state=1;
        switch(Key_detect())					//判断按键值
        {
        case KEY_UP:
            *(menu->par)+=menu->add;					//参数值增加1次
            while(state==Key_detect()&&state);
            break;
        case KEY_DOWN:
            *(menu->par)-=menu->add;					//参数值减少1次

            while(state==Key_detect()&&state);
            break;
        case KEY_RIGHT:												//参数值连续增加
            *(menu->par)+=menu->add/10;
            break;
        case KEY_LEFT:												//参数值连续减少
            *(menu->par)-=menu->add/10;
            break;
        //存入flash并回到菜单选择界面
        case KEY_ENDER:
            STMFLASH_Write(FLASH_SAVE_ADDR+menu->num*sizeof(double),(uint16_t*)menu->par,sizeof(double)/2);
            return;
        default:
            state=0;
        }
        if(state==1)
        {
            PrintPar(menu);   //有按键则刷新显示

        }
    }
}


/**
 *@brief      		进入调参模式
 *@param      		None
 *@SampleUsage
									while(1)
									{
									if(key==1){Menu_ShowPar();}		//如果按键按下，进入调参
									///////其他程序内容/////////
									}
 **/
void Menu_ShowPar(void)
{
    int i=0;
    int key=0;
    int serialnumber=1;
    Menu *NowMenu=&Read;
    PrintAllMenu(NowMenu,serialnumber);
	ReadFlash();
  NowMenu=&roSpd;
  for(i=0; i<4; i++)
  {
     Can_send_par(NowMenu);
     NowMenu=NowMenu->down;
  }
  NowMenu=NowMenu->down;
  free(bltSpd.par);
  bltSpd.par=(double*)malloc((int)((*bltNum.par)*sizeof(double)));
    while(1)
    {
        key=Key_detect();
        switch(key)
        {
        //到菜单上一位
        case KEY_UP:
            if(NowMenu->up!=NULL)
            {
                serialnumber--;
                NowMenu=NowMenu->up;
            }
            break;
        //到菜单下一位
        case KEY_DOWN:
            if(NowMenu->down!=NULL)
            {
                serialnumber++;
                NowMenu=NowMenu->down;
            }
            break;
        //到菜单下四位
        case KEY_RIGHT:
            for (i=0; i<4&&NowMenu->down!=NULL; i++)
            {
                serialnumber++;
                NowMenu=NowMenu->down;
            }
            break;
        //到菜单上四位
        case KEY_LEFT:
            for (i=0; i<4&&NowMenu->up!=NULL; i++)
            {
                serialnumber--;
                NowMenu=NowMenu->up;
            }
            break;
        //进入参数改变或对应的函数功能
        case KEY_ENDER:
        {
						while(key==Key_detect())
							delay_ms(50);
            if(NowMenu==&Read)
            {
                ReadFlash();
                NowMenu=&roSpd;
                for(i=0; i<4; i++)
                {
                    Can_send_par(NowMenu);
                    NowMenu=NowMenu->down;
                }
                NowMenu=NowMenu->down;
                free(bltSpd.par);
                bltSpd.par=(double*)malloc((int)((*bltNum.par)*sizeof(double)));
            }
            else if(NowMenu==&Write)
            {
                WriteFlash();
            }
            else if(NowMenu==&bltSpd)
            {
                showSpeed(bltSpd.par);
            }
            else if(NowMenu==&Count)
            {
                *Count.par=0;
            }
            else if(NowMenu==&FIRE)
            {
							NowMenu->par=bltNum.par;
                Can_send_par(NowMenu);
            }
            else
            {
                Menu_AdjustPar(NowMenu);
                if(NowMenu==&bltNum)
                {
                    free(bltSpd.par);
                    bltSpd.par=(double*)malloc((int)((*bltNum.par)*sizeof(double)));
                }
                Can_send_par(NowMenu);
            }
            break;
        }
        default:
            key=0;
        }
        if (key) {
            PrintAllMenu(NowMenu,serialnumber);
            while(key==Key_detect());
        }
				
//				 u8 can_buf[8];
//        int i=0;
//        i=Can_Receive_Msg(can_buf);
//				if(i)
//        Can_Receive_Handle(can_buf);
    }
}
/**
 *@brief      	读flash
 *@param      	None
 *@SampleUsage  ReadFlash();
 **/
void ReadFlash(void)
{
    u8 i;
    Menu *menu=&Read;
    for(i=0; i<5; i++)
    {
        menu=menu->down;
        STMFLASH_Read(FLASH_SAVE_ADDR+menu->num*sizeof(double),(uint16_t*)menu->par,sizeof(double)/2);//读取flash
    }
}
/**
 *@brief      		 写flash
 *@param      		 None
 *@SampleUsage     WriteFlash();
 **/
void WriteFlash(void)
{
    u8 i;
    Menu *menu=&Read;
    for(i=0; i<5; i++)
    {
        menu=menu->down;
        STMFLASH_Write(FLASH_SAVE_ADDR+menu->num*sizeof(double),(uint16_t*)menu->par,sizeof(double)/2);//写flash
    }
}

/**
 *@brief      		初始化OLED
 *@param      		None
 *@SampleUsage    Menu_Init();
 **/
void Menu_Init(void)
{
    //连接各节点

    Read.up=&Write;
    Read.down=&Write;
    Write.up=&Read;
    Write.down=&Read;

    Adc_Init();			//初始化ADC
    delay_init();		//初始化延时函数
    OLED_Init();		//初始化oled
}
/**
 *@brief      		初始化OLED
 *@param      		None
 *@SampleUsage    Menu_Init();
 **/
void showSpeed(double *speed)
{
    OLED_Clear();
    int key,					//按键值
        length=(int)(*bltNum.par-0.5),						//每次发射弹丸数量,四舍五入
        num=0,							//当前显示条目
        i;
    while(1)
    {
        key=Key_detect();
        switch(key)
        {
        case KEY_UP:
            num--;
            break;
        case KEY_DOWN:
            num++;
            break;
        case KEY_LEFT:
            num-=4;
            break;
        case KEY_RIGHT:
            num+=4;
            break;
        case KEY_ENDER:
            return;
        }

        if(num>length)
            num=length;
        if(num<0)
            num=0;
        if(key)
            OLED_Clear();
        for(i=0; ((i<4)&&((i+num)<=length)); i++)
        {
            OLED_ShowNum(0,i*2,(num+i)%(length+1)+1,2,16);
            printnum(&speed[(num+i)%(length+1)],i*2);
        }
        while(key==Key_detect()&&key);
    }
}









