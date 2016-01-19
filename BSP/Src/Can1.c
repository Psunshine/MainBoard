#include "includes.h"

#define MAX_MAIL_NUM  3
u8 CAN_msg_num1[MAX_MAIL_NUM];   // ����������

//can���սṹ��
CanRxMsg RxMessage1;

void CAN1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	// ��GPIOʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	// ��CANʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

	// TX��RX������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;				//��ת�ٶ�Ϊ50M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1);

	// CAN��������
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;// ʧ��ʱ�����ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM = ENABLE; // ʹ���Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;// ʧ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART = ENABLE;// ʧ���Զ��ش�ģʽ
	CAN_InitStructure.CAN_RFLM = DISABLE;// ʧ�ܽ���FIFO����ģʽ
	CAN_InitStructure.CAN_TXFP = DISABLE;// ʧ�ܷ���FIFO���ȼ�
	#ifdef CAN_DEBUG
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;// CAN�����ڻػ�ģʽ
	#else
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;// CAN����������ģʽ
	#endif

	// ����������
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;// ����ͬ����Ծ���1��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;// ʱ���1Ϊ10��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;// ʱ���2Ϊ1��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler = 3;// ��Ƶ��3��������=42M/((1+10+3)*3)=1M
	CAN_Init(CAN1,&CAN_InitStructure);

	// �����˲����ã������������б��ľ��ɽ���
	CAN_FilterInitStructure.CAN_FilterNumber = 0;//��ʼ��������0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;// ��ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;// ʹ��һ��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;// ��������ʾ��0x00000000
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;// ���������α�ʾ��0x0000000
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;// ������FIFO0ָ�������0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;// ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	 /* Enable CAN1 RX0 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	 /* Enable CAN1 RX1 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable CAN1 TX0 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE);  // fifo0�ж�
    CAN_ITConfig(CAN1, CAN_IT_FMP1 | CAN_IT_FF1 | CAN_IT_FOV1, ENABLE);  // fifo1�ж�
    CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                // �����ж�
    CAN_ITConfig(CAN1, CAN_IT_EWG | CAN_IT_EPV | CAN_IT_BOF | CAN_IT_LEC
                     | CAN_IT_ERR | CAN_IT_WKU | CAN_IT_SLK, ENABLE);  // ERR�ж�
}


char CAN1_Send_Msg(void)
{
	u16 CAN_ID;
	u8 msg[8];
	if(GetQueue(&CAN1_TX_Queue,&CAN_ID,msg))
	{
		CanTxMsg TxMessage;
		u8 mbox;
		u32 i=0;

		TxMessage.StdId = CAN_ID;	// ��׼��ʶ��
		TxMessage.ExtId = 0x00000000;	// ������չ��ʾ����29λ��
		TxMessage.IDE = CAN_ID_STD;		// ʹ�ñ�׼��ʶ��
		TxMessage.RTR = CAN_RTR_DATA;	// ��Ϣ����Ϊ����֡��һ֡8λ
		TxMessage.DLC = 8;			// ������Ϣ�ĳ���
		for(i = 0;i < 8;i++)
			TxMessage.Data[i] = msg[i];


		// �ȴ�һ���յ�CAN��������
		//while(((CAN1->TSR&CAN_TSR_TME0) != CAN_TSR_TME0)\
			&&((CAN1->TSR&CAN_TSR_TME1) != CAN_TSR_TME1)\
			&&((CAN1->TSR&CAN_TSR_TME1) != CAN_TSR_TME1));

		mbox = CAN_Transmit(CAN1,&TxMessage);

		CPU_SR_ALLOC();// ����cpu_sr
		CPU_CRITICAL_ENTER();
		CAN_msg_num1[mbox] = 1;
		CPU_CRITICAL_EXIT();

		CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
	}

	return 0;
}


u8 CAN1_Receive_Msg(char *buf,u8* CAN_ID)
{
	u8 i;
	if(CAN_MessagePending(CAN1,CAN_FIFO0)==0xff)
		return 0;        //û�н��յ�����,ֱ���˳�
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage1);//��ȡ����
	for(i = 0;i < 8;i++)
		buf[i]=RxMessage1.Data[i];
	*CAN_ID = RxMessage1.StdId;
	return RxMessage1.DLC;
}

void CAN1_TX_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

    if(CAN_msg_num1[0])
    {
        if(SET == CAN_GetITStatus(CAN1,CAN_IT_RQCP0))
        {
            CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP0);
            CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);
            CAN_msg_num1[0] = 0;
        }
    }

    if(CAN_msg_num1[1])
    {
        if(SET == CAN_GetITStatus(CAN1,CAN_IT_RQCP1))
        {
            CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP1);
            CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);
            CAN_msg_num1[1] = 0;
        }
    }

    if(CAN_msg_num1[2])
    {
        if(SET == CAN_GetITStatus(CAN1,CAN_IT_RQCP2))
        {
            CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP2);
            CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);
            CAN_msg_num1[2] = 0;
        }
    }
    OSIntExit(); // �жϲ�����һ���������жϼ������л�
}

/*******************************************************************************
�� �� ����CAN1_RX0_IRQHandler(void)
��    ����CAN���������ж�
*******************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr

	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

	if(SET == CAN_GetITStatus(CAN1,CAN_IT_FF0))
   	{
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
   	}
   	else if(SET == CAN_GetITStatus(CAN1,CAN_IT_FOV0))
   	{
        CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
   	}
   	else
  	{
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage1);
		InsertQueue(&CAN1_RX_Queue,RxMessage1.StdId,RxMessage1.Data);
    }

	OSIntExit(); // �жϲ�����һ���������жϼ������л�
}

/*******************************************************************************
�� �� ����CAN1_RX1_IRQHandler(void)
��    ����CAN���������ж�
*******************************************************************************/
void CAN1_RX1_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr

	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

	if(SET == CAN_GetITStatus(CAN1,CAN_IT_FF1))
   	{
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF1);
   	}
   	else if(SET == CAN_GetITStatus(CAN1,CAN_IT_FOV1))
   	{
        CAN_ClearITPendingBit(CAN1,CAN_IT_FOV1);
   	}
   	else
  	{
        CAN_Receive(CAN1, CAN_FIFO1, &RxMessage1);
		InsertQueue(&CAN1_RX_Queue,RxMessage1.StdId,RxMessage1.Data);
    }

	OSIntExit(); // �жϲ�����һ���������жϼ������л�
}


void CAN1_SCE_IRQHandler(void)
{

}

