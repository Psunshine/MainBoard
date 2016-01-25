/**
  ******************************************************************************
  * @�ļ�	PS_Dirver.c
  * @����	2015-1-26
  * @˵��	�ֱ�����
  ******************************************************************************
  * ������ͨ��ѧ-ͨ��1208-��С��
  *
  * ˵��:	IAR�汾:7.20.2.7314
  *      	���ļ��汾:��׼�� V1.3.0
  * �ļ�����:	Tab Size:4
  * �Զ�����:	Body:4
  ******************************************************************************
  * ʹ�õĽӿڣ�
  *             DATA -> GPIOE15
  *             CMD -> GPIOE14
  *             ATT -> GPIOE13
  *             CLK -> GPIOE12
  *             ACK -> GPIOE11
  * ע����PS�ֱ���ȡ�ٶȽ�����ֻ��ʹ��ģ��SPIʱ��
  ******************************************************************************
  */
#pragma once
#include "includes.h"

// PS���ӵ�GPIO��
#define		PS_GPIO_RCC1	RCC_AHB1Periph_GPIOE
#define		PS_GPIO1		GPIOE

// DATA
#define		PS_DATA_PIN		GPIO_Pin_15
#define		PS_DATA_RD()	GPIO_ReadInputDataBit(PS_GPIO1,PS_DATA_PIN)

// CMD
#define		PS_CMD_PIN		GPIO_Pin_14
#define		PS_CMD_HIGH()	GPIO_SetBits(PS_GPIO1,PS_CMD_PIN)
#define		PS_CMD_LOW()	GPIO_ResetBits(PS_GPIO1,PS_CMD_PIN)


#define		PS_GPIO_RCC2	RCC_AHB1Periph_GPIOE
#define		PS_GPIO2		GPIOE
// ATT
#define		PS_ATT_PIN		GPIO_Pin_13
#define		PS_ATT_HIGH()	GPIO_SetBits(PS_GPIO1,PS_ATT_PIN)
#define		PS_ATT_LOW()	GPIO_ResetBits(PS_GPIO1,PS_ATT_PIN)

// CLK
#define		PS_CLK_PIN		GPIO_Pin_12
#define		PS_CLK_HIGH()	GPIO_SetBits(PS_GPIO1,PS_CLK_PIN)
#define		PS_CLK_LOW()	GPIO_ResetBits(PS_GPIO1,PS_CLK_PIN)

// ACK
#define		PS_ACK_PIN		GPIO_Pin_11
#define		PS_ACK_RD()		GPIO_ReadInputDataBit(PS_GPIO1,PS_ACK_PIN)

// PS_Mode
#define		Digital_Mode	0x41
#define		Analog_Red		0x73

// Common Mode
#define		SLCT			0x01
#define		STRT			0x08
#define		UP				0x10
#define		RGHT			0x20
#define		DOWN			0x40
#define		LEFT			0x80
#define		L2				0x01
#define		R2				0x02
#define		L1				0x04
#define		R1				0x08
#define		Triangle	  	0x10
#define		Circular	  	0x20
#define		Cross		  	0x40
#define		Square		  	0x80

// Digital_Mode

// NegCon
#define		A				0x10
#define		B				0x20

// Analog_Red
#define		JOYR			0x02
#define		JOYL			0x04

/**
  * @˵�� �ֱ�״̬����
  */
typedef struct
{
	u8 PS_Data[9];		/*!<�ֱ�����*/
}PSTypeDef;

extern PSTypeDef PS;

// �����ӿڵ��ⲿ����
extern void PS_Init(void);
extern void Get_PS_Data(void);
