#include "includes.h"

#define MAX_MAIL_NUM  3
u8 CAN_msg_num2[MAX_MAIL_NUM];   // ����������

//can���սṹ��
CanRxMsg RxMessage2;

void CAN2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	// ��GPIOʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	// ��CANʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);

	// TX��RX������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2);

	// CAN��������
	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;// ʧ��ʱ�����ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM = ENABLE; // ʹ���Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;// ʧ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART = DISABLE;// ʧ���Զ��ش�ģʽ
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
	CAN_Init(CAN2,&CAN_InitStructure);

	// �����˲����ã������������б��ľ��ɽ���
	CAN_FilterInitStructure.CAN_FilterNumber = 14;//��ʼ��������14
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;// ��ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;// ʹ��һ��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;// ��������ʾ��0x00000000
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;// ���������α�ʾ��0x0000000
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;// ������FIFO0ָ�������0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;// ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	 /* Enable CAN2 RX0 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	 /* Enable CAN2 RX1 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable CAN2 TX0 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = CAN2_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN2, CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE);  // fifo0�ж�
    CAN_ITConfig(CAN2, CAN_IT_FMP1 | CAN_IT_FF1 | CAN_IT_FOV1, ENABLE);  // fifo1�ж�
    CAN_ITConfig(CAN2, CAN_IT_TME, DISABLE);                // �����ж�
    CAN_ITConfig(CAN2, CAN_IT_EWG | CAN_IT_EPV | CAN_IT_BOF | CAN_IT_LEC
                     | CAN_IT_ERR | CAN_IT_WKU | CAN_IT_SLK, ENABLE);  // ERR�ж�
}


char CAN2_Send_Msg(void)
{
	u16 CAN_ID;
	u8 msg[8];
	if(GetQueue(&CAN2_TX_Queue,&CAN_ID,msg))
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
		while(((CAN2->TSR&CAN_TSR_TME0) != CAN_TSR_TME0)\
			&&((CAN2->TSR&CAN_TSR_TME1) != CAN_TSR_TME1)\
			&&((CAN2->TSR&CAN_TSR_TME1) != CAN_TSR_TME1));

		mbox = CAN_Transmit(CAN2,&TxMessage);

		CPU_SR_ALLOC();// ����cpu_sr
		CPU_CRITICAL_ENTER();
		CAN_msg_num2[mbox] = 1;
		CPU_CRITICAL_EXIT();

		CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE);
	}

	return 0;
}


u8 CAN2_Receive_Msg(char *buf,u8* CAN_ID)
{
	u8 i;
	if(CAN_MessagePending(CAN2,CAN_FIFO0)==0xff)
		return 0;        //û�н��յ�����,ֱ���˳�
	CAN_Receive(CAN2,CAN_FIFO0, &RxMessage2);//��ȡ����
	for(i = 0;i < 8;i++)
		buf[i]=RxMessage2.Data[i];
	*CAN_ID = RxMessage2.StdId;
	return RxMessage2.DLC;
}

void CAN2_TX_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

    if(CAN_msg_num2[0])
    {
        if(SET == CAN_GetITStatus(CAN2,CAN_IT_RQCP0))
        {
            CAN_ClearITPendingBit(CAN2,CAN_IT_RQCP0);
            CAN_ITConfig(CAN2, CAN_IT_TME, DISABLE);
            CAN_msg_num2[0] = 0;
        }
    }

    if(CAN_msg_num2[1])
    {
        if(SET == CAN_GetITStatus(CAN2,CAN_IT_RQCP1))
        {
            CAN_ClearITPendingBit(CAN2,CAN_IT_RQCP1);
            CAN_ITConfig(CAN2, CAN_IT_TME, DISABLE);
            CAN_msg_num2[1] = 0;
        }
    }

    if(CAN_msg_num2[2])
    {
        if(SET == CAN_GetITStatus(CAN2,CAN_IT_RQCP2))
        {
            CAN_ClearITPendingBit(CAN2,CAN_IT_RQCP2);
            CAN_ITConfig(CAN2, CAN_IT_TME, DISABLE);
            CAN_msg_num2[2] = 0;
        }
    }
    OSIntExit(); // �жϲ�����һ���������жϼ������л�
}

/*******************************************************************************
�� �� ����CAN2_RX0_IRQHandler(void)
��    ����CAN���������ж�
*******************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr

	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

	if(SET == CAN_GetITStatus(CAN2,CAN_IT_FF0))
   	{
        CAN_ClearITPendingBit(CAN2,CAN_IT_FF0);
   	}
   	else if(SET == CAN_GetITStatus(CAN2,CAN_IT_FOV0))
   	{
        CAN_ClearITPendingBit(CAN2,CAN_IT_FOV0);
   	}
   	else
  	{
        CAN_Receive(CAN2, CAN_FIFO0, &RxMessage2);
		InsertQueue(&CAN2_RX_Queue,RxMessage2.StdId,RxMessage2.Data);
    }

	OSIntExit(); // �жϲ�����һ���������жϼ������л�
}

/*******************************************************************************
�� �� ����CAN2_RX1_IRQHandler(void)
��    ����CAN���������ж�
*******************************************************************************/
void CAN2_RX1_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr

	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

	if(SET == CAN_GetITStatus(CAN2,CAN_IT_FF1))
   	{
        CAN_ClearITPendingBit(CAN2,CAN_IT_FF1);
   	}
   	else if(SET == CAN_GetITStatus(CAN2,CAN_IT_FOV1))
   	{
        CAN_ClearITPendingBit(CAN2,CAN_IT_FOV1);
   	}
   	else
  	{
        CAN_Receive(CAN2, CAN_FIFO1, &RxMessage2);
		InsertQueue(&CAN2_RX_Queue,RxMessage2.StdId,RxMessage2.Data);
    }

	OSIntExit(); // �жϲ�����һ���������жϼ������л�
}


void CAN2_SCE_IRQHandler(void)
{

}

