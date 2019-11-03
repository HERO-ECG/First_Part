#include "can.h"


/**********************************************
*CAN初始化
*示例：CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_8tq,4,CAN_Mode_LoopBack);
*波特率=Fpclk1/((bs1+bs2+1)*brp);
**********************************************/
uint8_t CAN_Mode_Init(uint8_t sjw, uint8_t bs1, uint8_t bs2, uint8_t brp, uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	#if CAN_RX0_INT_ENABLE
	NVIC_InitTypeDef NVIC_InitStructure;
	#endif
	/*GPIO初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//使能 PORTA 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			//使能 CAN1 时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 					//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure); 										//初始化 IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 						//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 										//初始化 IO
	
	/*CAN配置*/
	CAN_InitStructure.CAN_TTCM=DISABLE; 											//非时间触发通信模式
	CAN_InitStructure.CAN_ABOM=DISABLE; 											//软件自动离线管理
	CAN_InitStructure.CAN_AWUM=DISABLE; 											//睡眠模式通过软件唤醒
	CAN_InitStructure.CAN_NART=DISABLE; 												//禁止报文自动传送
	CAN_InitStructure.CAN_RFLM=DISABLE; 											//报文不锁定,新的覆盖旧的
	CAN_InitStructure.CAN_TXFP=ENABLE; 											//优先级由报文标识符决定
	CAN_InitStructure.CAN_Mode= mode; 												//模式设置： 0,普通模式;1,回环模式;

	CAN_InitStructure.CAN_SJW=sjw; 														//重新同步跳跃宽度(Tsjw)
	CAN_InitStructure.CAN_BS1=bs1; 														//时间段 1 占用时间单位
	CAN_InitStructure.CAN_BS2=bs2; 														// 时间段 2 占用时间单位
	CAN_InitStructure.CAN_Prescaler=brp; 											//分频系数(Fdiv)为 brp+1
	CAN_Init(CAN1, &CAN_InitStructure); 											// 初始化 CAN1
	/*滤波器配置*/
	CAN_FilterInitStructure.CAN_FilterNumber=0; 							//过滤器 0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32 位
	#ifdef SLAVE
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000; 					//32 位 ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000; 			//32 位 MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	#else
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000; 					//32 位 ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000; 			//32 位 MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	#endif
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;// FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 			//激活过滤器 0
	CAN_FilterInit(&CAN_FilterInitStructure); 								//滤波器初始化
	
	#if CAN_RX0_INT_ENABLE
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE); //FIFO0 消息挂号中断允许.
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 主优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 次优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	#endif
	return 0;
}


/**********************************************
*CAN发送函数
*return ：1成功 0失败
**********************************************/
uint8_t CAN_SendMsg(uint8_t *msg, uint8_t length)
{
	uint8_t mbox;
	uint16_t i=0;
	CanTxMsg TxMessage;
	
	assert_param((length<=8));
	
	TxMessage.StdId=0x1FF; 										// 标准标识符为 0
	TxMessage.ExtId=0; 										// 11 位）
	TxMessage.IDE=CAN_Id_Standard; 						// 标准帧
	TxMessage.RTR=CAN_RTR_Data;								// 数据帧
	TxMessage.DLC=length; 										// 要发送的数据长度
	for(i=0;i<length;i++)
	{
		TxMessage.Data[i]=msg[i];
	}
	mbox= CAN_Transmit(CAN1, &TxMessage);
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok)&&(i<0xfff))i++; //等待结束
	if(i>=0xfff)return 0;
	return 1;
}
/**********************************************
*读取CANFIFO数据
*return ：0：无数据 	其他：接收的数据长度
**********************************************/
uint8_t Can_Receive_Msg(uint8_t *buf)
{
	uint32_t i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;//没有接收到数据,直接退出
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); //读取数据
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
