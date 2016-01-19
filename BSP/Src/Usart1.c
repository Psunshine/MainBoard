/*
ע��
	USART1�ں��İ��ϣ���·�Ѿ��̶������ں���λ������ͨ��
*/
#include "includes.h"

u8 Usart1buf[USART1_BUF_SIZE];
u8 pointer = 0;

// ����1��ʼ������
void USART1_Init(void)
{	
	USART_InitTypeDef USART1_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// A��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// A9 -> TX  ,  A10 -> RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	// PC11 -> BT_Status
	
	// ���ù�������
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	// USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	// USART1�ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// USART1��ʼ��
	USART_DeInit(USART1);
	USART1_InitStructure.USART_BaudRate = 115200;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_Parity = USART_Parity_No;
	USART1_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART1_InitStructure);
	USART_Cmd(USART1,ENABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

void Send_Char(u8 Char)
{
	USART_SendData(USART1,Char);
	// �ȴ��������һ���ֽ�
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) != SET);
}

void Send_String(uint8_t *pData,char len)
{
	while(len--)
	{
		Send_Char(*pData);
		// ָ���һ
		pData++;
	}
}

void USART1_IRQHandler(void)
{
	CPU_SR_ALLOC();// ����cpu_sr

	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
	OSIntEnter(); // �����жϣ��жϲ�����һ
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		Usart1buf[pointer++%USART1_BUF_SIZE] = USART_ReceiveData(USART1);
		
		// �ڴ�д������ճ���
		if(1)
		{
		}
		
		
	}
	
	OSIntExit(); // �жϲ�����һ���������жϼ������л�
}
