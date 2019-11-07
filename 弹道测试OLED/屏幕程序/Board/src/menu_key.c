/**********************718����ʵ���ҿ���������*********************
*  ��д��718����ʵ����
*  ƽ̨��STM32F103ZET6
*  ˵�����������ߵ�ˮƽ���ޣ����в���֮�����������½⡣
*		 �����Ҷ࿴�������ֲᡣ     
******************************************************************/
#include "stm32f10x.h"
#include "menu_key.h"
#include "delay.h"

/**
  * @brief  ��ʼ�� ADC,����ͨ�� 0~3
  * @param  None
  * @retval None
  */
void Adc_Init(void)
{ 
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1 , ENABLE ); //ʹ�� ADC1 ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���� ADC ��Ƶ���� 6
	//72M/6=12,ADC ���ʱ�䲻�ܳ��� 14M
	//PA4 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA.3
	
	ADC_DeInit(ADC1); //��λ ADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת����
	//����������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC �����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1; //˳����й���ת���� ADC ͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure); //����ָ���Ĳ�����ʼ������ ADCx
	ADC_Cmd(ADC1, ENABLE); //ʹ��ָ���� ADC1
	ADC_ResetCalibration(ADC1); //������λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����
	ADC_StartCalibration(ADC1); //���� AD У׼
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����
}


/**
  * @brief  ��� ADC ֵ
  * @param  ch:ͨ��ֵ 0~3
  * @retval None
  */
uint16_t Get_Adc(uint8_t Ch)
{
	//����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, Ch, 1, ADC_SampleTime_55Cycles5 );
	//ͨ�� 3,�������˳��ֵΪ 1,����ʱ��Ϊ 239.5 ����
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ�����ת������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1); //�������һ�� ADC1 �������ת�����
}

/**
  * @brief  ����ƽ��ֵ
	* @param  Ch:ͨ��ֵ 0~3
						Times:ȡƽ���Ĵ���
  * @retval None
  */
uint16_t Get_Adc_Average(uint8_t Ch,uint8_t Times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<Times;t++)
	{ 
		temp_val+=Get_Adc(Ch);
	//	delay_ms(5);
	}
	return temp_val/Times;		//��ƽ��
}

/**
  * @brief  �ж��ĸ�����������
  * @param  None
  * @retval None
  */
int Key_detect(void)
{
	uint16_t adcx;
	float temp;
	int key=0;
	adcx=Get_Adc(ADC_Channel_3);//��ȡadcֵ
	temp=(float)adcx*(3.3/4096);					//�����ѹ
	//�ж��ĸ�����
	if(temp<3.1)
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	if (temp<=KEY_UP_ADC+0.2&&temp>=KEY_UP_ADC-0.2){
		delay_ms(6);//��ʱ����
		adcx=Get_Adc_Average(ADC_Channel_3,20);//��ȡadcֵ
		temp=(float)adcx*(3.3/4096);					//�����ѹ
		if (temp<=KEY_UP_ADC+0.2&&temp>=KEY_UP_ADC-0.2){
			key= KEY_UP;
		}
	}
	else if(temp<=KEY_DOWN_ADC+0.2&&temp>=KEY_DOWN_ADC-0.2){
		delay_ms(6);
		adcx=Get_Adc_Average(ADC_Channel_3,20);//��ȡadcֵ
		temp=(float)adcx*(3.3/4096);					//�����ѹ
		if (temp<=KEY_DOWN_ADC+0.2&&temp>=KEY_DOWN_ADC-0.2){
			key= KEY_DOWN;
		}
	}
	else if(temp<=KEY_RIGHT_ADC+0.2&&temp>=KEY_RIGHT_ADC-0.2){
		delay_ms(6);
		adcx=Get_Adc_Average(ADC_Channel_3,20);//��ȡadcֵ
		temp=(float)adcx*(3.3/4096);					//�����ѹ
		if (temp<=KEY_RIGHT_ADC+0.2&&temp>=KEY_RIGHT_ADC-0.2){
			key=KEY_RIGHT;
		}
	}
	else if(temp<=KEY_LEFT_ADC+0.2&&temp>=KEY_LEFT_ADC-0.2){
		delay_ms(6);
		adcx=Get_Adc_Average(ADC_Channel_3,20);//��ȡadcֵ
		temp=(float)adcx*(3.3/4096);					//�����ѹ
		if (temp<=KEY_LEFT_ADC+0.2&&temp>=KEY_LEFT_ADC-0.2){
			key= KEY_LEFT;
		}
	}
	else if(temp<=KEY_ENDER_ADC+0.2&&temp>=KEY_ENDER_ADC-0.2){
		delay_ms(6);
		adcx=Get_Adc_Average(ADC_Channel_3,20);//��ȡadcֵ
		temp=(float)adcx*(3.3/4096);					//�����ѹ
		if (temp<=KEY_ENDER_ADC+0.2&&temp>=KEY_ENDER_ADC-0.2){
			key= KEY_ENDER;
		}
	}
	else{
		key= 0;
	}
	if(key)
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	return key;
}
