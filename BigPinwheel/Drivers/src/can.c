#include "can.h"


/**********************************************
*CAN��ʼ��
*ʾ����CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_8tq,4,CAN_Mode_LoopBack);
*������=Fpclk1/((bs1+bs2+1)*brp);
**********************************************/
uint8_t CAN_Mode_Init(uint8_t sjw, uint8_t bs1, uint8_t bs2, uint8_t brp, uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	#if CAN_RX0_INT_ENABLE
	NVIC_InitTypeDef NVIC_InitStructure;
	#endif
	/*GPIO��ʼ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//ʹ�� PORTA ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			//ʹ�� CAN1 ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 					//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 										//��ʼ�� IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 						//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 										//��ʼ�� IO
	
	/*CAN����*/
	CAN_InitStructure.CAN_TTCM=DISABLE; 											//��ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM=DISABLE; 											//����Զ����߹���
	CAN_InitStructure.CAN_AWUM=DISABLE; 											//˯��ģʽͨ���������
	CAN_InitStructure.CAN_NART=DISABLE; 												//��ֹ�����Զ�����
	CAN_InitStructure.CAN_RFLM=DISABLE; 											//���Ĳ�����,�µĸ��Ǿɵ�
	CAN_InitStructure.CAN_TXFP=ENABLE; 											//���ȼ��ɱ��ı�ʶ������
	CAN_InitStructure.CAN_Mode= mode; 												//ģʽ���ã� 0,��ͨģʽ;1,�ػ�ģʽ;

	CAN_InitStructure.CAN_SJW=sjw; 														//����ͬ����Ծ���(Tsjw)
	CAN_InitStructure.CAN_BS1=bs1; 														//ʱ��� 1 ռ��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2=bs2; 														// ʱ��� 2 ռ��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler=brp; 											//��Ƶϵ��(Fdiv)Ϊ brp+1
	CAN_Init(CAN1, &CAN_InitStructure); 											// ��ʼ�� CAN1
	/*�˲�������*/
	CAN_FilterInitStructure.CAN_FilterNumber=0; 							//������ 0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32 λ
	#ifdef SLAVE
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000; 					//32 λ ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000; 			//32 λ MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	#else
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000; 					//32 λ ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000; 			//32 λ MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	#endif
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;// FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 			//��������� 0
	CAN_FilterInit(&CAN_FilterInitStructure); 								//�˲�����ʼ��
	
	#if CAN_RX0_INT_ENABLE
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE); //FIFO0 ��Ϣ�Һ��ж�����.
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // �����ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // �����ȼ�Ϊ 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	#endif
	return 0;
}


/**********************************************
*CAN���ͺ���
*return ��1�ɹ� 0ʧ��
**********************************************/
uint8_t CAN_SendMsg(uint8_t *msg, uint8_t length)
{
	uint8_t mbox;
	uint16_t i=0;
	CanTxMsg TxMessage;
	
	assert_param((length<=8));
	
	TxMessage.StdId=0x1FF; 										// ��׼��ʶ��Ϊ 0
	TxMessage.ExtId=0; 										// 11 λ��
	TxMessage.IDE=CAN_Id_Standard; 						// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;								// ����֡
	TxMessage.DLC=length; 										// Ҫ���͵����ݳ���
	for(i=0;i<length;i++)
	{
		TxMessage.Data[i]=msg[i];
	}
	mbox= CAN_Transmit(CAN1, &TxMessage);
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //�ȴ�����
	if(i>=0xfff)return 0;
	return 1;
}
/**********************************************
*��ȡCANFIFO����
*return ��0�������� 	���������յ����ݳ���
**********************************************/
uint8_t Can_Receive_Msg(uint8_t *buf)
{
	uint32_t i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;//û�н��յ�����,ֱ���˳�
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); //��ȡ����
	if(RxMessage.IDE == CAN_Id_Standard)
	{
		for(i=0;i<RxMessage.DLC;i++)
		buf[i]=RxMessage.Data[i];
	}
	else if(RxMessage.IDE == CAN_Id_Extended)
	{
		
	}
	
	return RxMessage.DLC;
}
