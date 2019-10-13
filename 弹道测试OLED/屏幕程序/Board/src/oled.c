#include "stdlib.h"
#include "oled_font.h"
#include "delay.h"
#include "oled.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**
  * @brief  ����IIC
  * @param  None
  * @retval None
  */
void IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**
  * @brief  ֹͣ
  * @param  None
  * @retval None
  */
void IIC_Stop()
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}
/**
  * @brief  �ȴ�Ӧ��
  * @param  None
  * @retval None
  */
void IIC_Wait_Ack()
{

	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**
  * @brief  дһ���ֽ�
  * @param  IIC_Byte��Ҫд���ֽ�
  * @retval None
  */

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}
/**
  * @brief  �����������������
  * @param  IIC_Command�����͵�����
  * @retval None
  */
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**
  * @brief  �������ݵ���������
  * @param  IIC_Data�����͵�����
  * @retval None
  */
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**
  * @brief  ��������
  * @param  Dat�����͵����ݻ�����
						Cmd�����Ʒ������ݻ�������
  * @retval None
  */
void OLED_WR_Byte(unsigned Dat,unsigned Cmd)
{
	if(Cmd)
			{

   Write_IIC_Data(Dat);
   
		}
	else {
   Write_IIC_Command(Dat);
		
	}


}

/**
  * @brief  ��ʾ����
  * @param  Fill_Data��������ʾ������
  * @retval None
  */
void Fill_picture(unsigned char Fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(Fill_Data,1);
			}
	}
}

/**
  * @brief  50ms��ʱ����
  * @param  Del_50ms��50ms�ĸ���
  * @retval None
  */
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

/**
  * @brief  1ms��ʱ����
  * @param  Del_1ms��1ms�ĸ���
  * @retval None
  */
void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

/**
  * @brief  ��������
  * @param  x��x����
						y��y����
  * @retval None
  */
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
/**
  * @brief  ����OLED
  * @param  None
  * @retval None
  */   
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
  * @brief  �ر�OLED��ʾ 
  * @param  None
  * @retval None
  */   
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

/**
  * @brief  	��������
  * @param  	None
  * @retval 	None
	* @caution 	������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
  */   
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}
/**
  * @brief  ��
  * @param  None
  * @retval None
  */
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}

/**
  * @brief  ��ָ��λ����ʾһ���ַ�,���������ַ�
  * @param  x:0~127
            y:0~63
            mode:0,������ʾ;1,������ʾ	
            Char_Size:ѡ������ 16/12 
  * @retval None
  */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t Chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=Chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}

/**
  * @brief  m^n����
  * @param  m������
						n����
  * @retval None
  */
uint32_t Oled_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
	 	
/**
  * @brief  ��ʾ2������
  * @param  x,y :�������	 
            Len :���ֵ�λ��
            size:�����С
            mode:ģʽ	0,���ģʽ;1,����ģʽ
            Num:��ֵ(0~4294967295);	 	
  * @retval None
  */
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t Num,uint8_t Len,uint8_t Size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<Len;t++)
	{
		temp=(Num/Oled_Pow(10,Len-t-1))%10;
		if(enshow==0&&t<(Len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(Size2/2)*t,y,' ',Size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(Size2/2)*t,y,temp+'0',Size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
/**
  * @brief  ��ָ��λ����ʾһ���ַ��Ŵ�
  * @param  x,y����ʼ����
						chr����ʾ�����ݵĵ�ַ
				    Char_Size��ѡ������ 16/12 
  * @retval None
  */
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
/**
  * @brief  ��ָ��λ����ʾһ���ַ��Ŵ�
  * @param  x,y����ʼ����
						No�����ֱ��
  * @retval None
  */
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t No)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*No][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*No+1][t],OLED_DATA);
				adder+=1;
      }					
}

/**
  * @brief  ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
  * @param  x0��x�������ʾ���
						y0��y�������ʾ���
						x1��x�������ʾ�յ�
						x2��x�������ʾ�յ�
						BMP��BMP����
  * @retval None
  */
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
	
/**
  * @brief  ��ʼ��SSD1306
  * @param  None
  * @retval None
  */
void OLED_Init(void)
{ 	
 
 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��B�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;	 //PB6,PB7�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOA,&GPIO_InitStructure);	  //��ʼ��GPIOB
	
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��B�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //PB6,PB7�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB,&GPIO_InitStructure);	  //��ʼ��GPIOB
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
 	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2);	//PB6,PB7 �����

 /*#if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,0xFF); //PC0~7�����

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT�������
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  �����

 #else
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //PC0,1 OUT�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);						 //PC0,1 OUT  �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PG15 OUT�������	  RST
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG15 OUT  �����


 #endif*/
delay_ms(200);

OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  

