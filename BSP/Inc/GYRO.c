/*******************************Copyright (c)***********************************
  *                      ������ͨ��ѧRobocon2015������С��
  ******************************************************************************
  * @�ļ�	Gyro.c
  * @����	2014-10-18
  * @˵��	�����ǲɼ�
  ******************************************************************************
  * ������ͨ��ѧ-ͨ��1208-��С��
  *
  * ˵��:	IAR�汾:7.20.2.7314
  *      	���ļ��汾:��׼�� V1.3.0
  *         UCOS�汾:3.04.04(�������޸����FPU��֧��)
  * �ļ�����:	Tab Size:4
  * �Զ�����:	Body:4
  * ��ϸ˵����Documents��Readme.txt
  ******************************************************************************
  * ����˵����
  * USART2��
  *		TX	 	A2
  *		RX	 	A3
  *		NRST	A5
  * ��	 ����	V1.0(ԭ����)
  *				V1.1:��С��(2014-10-18),��д���շ�ʽ����Ϊ�ӻ�������ֱ�Ӷ�ȡ����֡
  *				V1.2:��С��(2014-12-03),�޸�Ӳ���ж�bug,��������Ǹ�λ����Gyro_RST
  *				V1.3:��С��(2015-02-17),��ֲ��STM32F4��UCOSIII,�淶���궨���
  ******************************************************************************
  */
#include "includes.h"

u16 Gyro_Buffer[40];		// ���ݻ�������,�ڲ�ʹ��
char Gyro_RecPointer = 0;	// ���ݻ���ָ��,�ڲ�ʹ��
s16 iAngle;					// ���͵ĽǶ����ݣ�δ����10,�ڲ�ʹ��
float Angle;				// �����͵ĽǶȣ�δ���й���,�ڲ�ʹ��
float Theta;				// �����͵ĽǶ�(������)��-pi~pi��Χ,�ڲ�ʹ��

/*******************************************************************************
* Function Name  : void Gyro_Usart_Init()
* Description    : ������ͨ�Ŵ��ڣ�����2���ó�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Gyro_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	Gyro_USART_Port_Enable();
	Gyro_USART_Enable();

    // TX��RX�ܽ�ģʽ����
	GPIO_InitStructure.GPIO_Pin = Gyro_USART_TX | Gyro_USART_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;				//��ת�ٶ�Ϊ50M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(Gyro_USART_PORT,&GPIO_InitStructure);

	// ���ù�������
	GPIO_PinAFConfig(Gyro_USART_PORT,Gyro_TX_PinSource,Gyro_USART_AF);
	GPIO_PinAFConfig(Gyro_USART_PORT,Gyro_RX_PinSource,Gyro_USART_AF);

	// Reset��
	Gyro_Reset_Port_Enable();
	GPIO_InitStructure.GPIO_Pin = Gyro_Reset_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(Gyro_Reset_PORT,&GPIO_InitStructure);

	GPIO_ResetBits(Gyro_Reset_PORT,Gyro_Reset_PIN);

    //���ڹ���ģʽ����
	USART_InitStructure.USART_BaudRate = Gyro_USART_Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(Gyro_USART,&USART_InitStructure);
	USART_Cmd(Gyro_USART,ENABLE);

    //�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = Gyro_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    //���ж�
    USART_ITConfig(Gyro_USART,USART_IT_RXNE,ENABLE);
}

/*******************************************************************************
* Function Name  : void Gyro_RST(void)
* Description    : �����Ǹ�λ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Gyro_RST(void)
{
	volatile unsigned char i=0;
	GPIO_ResetBits(Gyro_Reset_PORT,Gyro_Reset_PIN);
	//��ʱ2S
	OS_ERR err;
	OSTimeDlyHMSM(0, 0, 2, 0,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
	GPIO_SetBits(Gyro_Reset_PORT,Gyro_Reset_PIN);
}


/*******************************************************************************
* Function Name  : TransTheta(float fAngle)
* Description    : �ڲ����ã����ڽǶ�ת��
* Input          : Angle�������͵ĽǶ�(�Ƕ���)��ȡֵ����
* Return         : theta:�����͵ĽǶ�(������)����Χ-pi~pi
* BUG            : NoBug
*******************************************************************************/
float TransTheta(float angle)
{
    float theta;
    while(angle > 180.0)
    {
        angle -= 360.0;
    }
    while(angle < -180.0)
    {
        angle += 360.0;
    }
	theta = angle*PI/180.0;
    return theta;
}

/*******************************************************************************
* Function Name  : void Gyro_USART_IRQHandler(void)
* Description    : ����4�жϷ�����
* Input          : None
* Output         : None
* Return         : Angle
* BUG            : NoBug
*******************************************************************************/
void Gyro_USART_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr

	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

	if(USART_GetITStatus(Gyro_USART, USART_IT_RXNE) != RESET )
    {
		USART_ClearFlag(Gyro_USART, USART_FLAG_RXNE);

		// ��ȡ��ǰ�ֽ�,�����ݴ��뻺����
		Gyro_Buffer[Gyro_RecPointer++] = USART_ReceiveData(Gyro_USART);

		// ����յ�������4~40��
		if((Gyro_RecPointer >= 4)&&(Gyro_RecPointer < 40))
		{
			// ���3���ֽ�ǰ�յ�֡ͷ���һ�����δ��
			if((Gyro_Buffer[Gyro_RecPointer-4] == 0xff\
				||Gyro_Buffer[Gyro_RecPointer-4] == 0xfe))
			{
				// ���У���
				if((Gyro_Buffer[Gyro_RecPointer-1]&0xff) == \
					((Gyro_Buffer[Gyro_RecPointer-4]\
						+Gyro_Buffer[Gyro_RecPointer-3]\
							+Gyro_Buffer[Gyro_RecPointer-2])\
								&0xff))
				{
					if(Gyro_Buffer[Gyro_RecPointer-4] == 0xff)
						iAngle = (Gyro_Buffer[Gyro_RecPointer-3]<<8)\
									|Gyro_Buffer[Gyro_RecPointer-2];
					if(Gyro_Buffer[Gyro_RecPointer-4] == 0xfe)
						iAngle = -((Gyro_Buffer[Gyro_RecPointer-3]<<8)\
									|Gyro_Buffer[Gyro_RecPointer-2]);
					Angle = ((float)iAngle)/10;
					Theta = TransTheta(Angle);
					
				}
				else
				{}
			}
		}
		// ָ���������
		if(Gyro_RecPointer > 39)Gyro_RecPointer = 0;
    }

	OSIntExit(); // �жϲ�����һ���������жϼ������л�
}
