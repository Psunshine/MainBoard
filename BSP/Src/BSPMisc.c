#include "includes.h"

// ��1ms��ʱΪ���Խ����ʮ��׼ȷ
void delay_ms(int tmp)
{
	int volatile tmp1;
	int i;
	for(i = 0;i < 8400;i++)
	{
		tmp1 = tmp;
		while(tmp1--);
	}
}

// ������ʼ������
void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	KEY_PORT_ENABLE();

	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = PIN_KEY1; 
	GPIO_Init(PORT_KEY1,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_KEY2; 
	GPIO_Init(PORT_KEY2,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_KEY3; 
	GPIO_Init(PORT_KEY3,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_KEY4; 
	GPIO_Init(PORT_KEY4,&GPIO_InitStructure);
}

// �����ʼ������
void STATU_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	STATU_PORT_ENABLE();

	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = PIN_STATU1; 
	GPIO_Init(PORT_STATU1,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_STATU2; 
	GPIO_Init(PORT_STATU2,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_STATU3; 
	GPIO_Init(PORT_STATU3,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_STATU4; 
	GPIO_Init(PORT_STATU4,&GPIO_InitStructure);
}

// LED��ʼ������
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	LED_PORT_ENABLE();
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = PIN_LED0; 
	GPIO_Init(PORT_LED0,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_LED1; 
	GPIO_Init(PORT_LED1,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_LED2; 
	GPIO_Init(PORT_LED2,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_LED3; 
	GPIO_Init(PORT_LED3,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_LED4; 
	GPIO_Init(PORT_LED4,&GPIO_InitStructure);
}

// ��������ʼ������
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	BEEP_PORT_ENABLE();
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = PIN_BEEP; 
	GPIO_Init(PORT_BEEP,&GPIO_InitStructure);
}

//��������ĳ�ʼ��
void Peripheral_Init(void)
{
	// �ж�����������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	PS_Init();	
    KEY_Init();	
	STATU_Init();
	LED_Init();
	BEEP_Init();
    CAN1_Init();
	CAN2_Init();
    OLED_Init();
    ADC_Configuration();
}