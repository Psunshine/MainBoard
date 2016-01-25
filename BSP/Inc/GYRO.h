/*******************************Copyright (c)***********************************
  *                      ������ͨ��ѧRobocon2015������С��
  ******************************************************************************
  * @�ļ�	GYRO.h
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

#pragma once
#include "includes.h"

/********************************������غ궨��********************************/
// ����
#define Gyro_USART				USART3
#define Gyro_USART_PORT      	GPIOB
#define Gyro_USART_TX        	GPIO_Pin_10
#define Gyro_USART_RX        	GPIO_Pin_11
#define Gyro_USART_Baud      	19200
#define	Gyro_Reset_PORT			GPIOA
#define	Gyro_Reset_PIN			GPIO_Pin_8
// ����
#define Gyro_USART_AF			GPIO_AF_USART3
#define Gyro_TX_PinSource      	GPIO_PinSource10
#define Gyro_RX_PinSource      	GPIO_PinSource11
// ʱ��
#define Gyro_USART_Enable()			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE)
#define Gyro_USART_Port_Enable()	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE)
#define Gyro_Reset_Port_Enable()	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE)
// �ж�
#define Gyro_USART_IRQn			USART3_IRQn
#define Gyro_USART_IRQHandler	USART3_IRQHandler
/********************************������غ궨��********************************/

void Gyro_Init(void);
void Gyro_RST(void);
extern float Theta;
extern float Angle;