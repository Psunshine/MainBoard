/**
  ******************************************************************************
  * @�ļ�	TASK.h
  * @����	2015-2-15
  * @˵��	ϵͳ����
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
  */

#pragma once
#include "includes.h"

// ��������
#define STARTUP_TASK_PRIO 3								// �������ȼ�
#define STARTUP_TASK_STK_SIZE 256						// �����ջ��С
extern OS_TCB TASK_START_TCB;							// ����TCB���ƿ�
extern CPU_STK TASK_START_STK[STARTUP_TASK_STK_SIZE];	// �����ջ�ռ�

// LED����
#define TASK_LED_BEEP_PRIO 20									// �������ȼ�
#define TASK_LED_BEEP_STK_SIZE 256							// �����ջ��С
#define TASK_LED_BEEP_DELAY 50								// ������ʱ							// ������ʱ

// CAN1��������
#define TASK_CAN_PRIO 4									// �������ȼ�
#define TASK_CAN_STK_SIZE 256							// �����ջ��С
#define TASK_CAN_DELAY 10								// ������ʱ

// ��ʱ����
#define TASK_TIME_PRIO 12								// �������ȼ�
#define TASK_TIME_STK_SIZE 128							// �����ջ��С
#define TASK_TIME_DELAY 500

// ��ʾ����
#define TASK_SHOW_PRIO 13								// �������ȼ�
#define TASK_SHOW_STK_SIZE 128							// �����ջ��С
#define TASK_SHOW_DELAY 20

// ���������ȡ����
#define TASK_KEY_PRIO 14								// �������ȼ�
#define TASK_KEY_STK_SIZE 128							// �����ջ��С
#define TASK_KEY_DELAY 10				// ������ʱ

// �ֱ���ȡ����
#define TASK_PS_PRIO 8								// �������ȼ�
#define TASK_PS_STK_SIZE 128							// �����ջ��С
#define TASK_PS_DELAY 20				// ������ʱ

// ADC�ɼ�����
#define TASK_ADC_PRIO 9								// �������ȼ�
#define TASK_ADC_STK_SIZE 128							// �����ջ��С
#define TASK_ADC_DELAY 20				// ������ʱ


// ��������
void TASK_START(void);
void TASK_LED_BEEP(void *p_arg);
void TASK_CAN(void *p_arg);
void TASK_TIME(void *p_arg);
void TASK_SHOW(void *p_arg);
void TASK_KEY(void *p_arg);
void TASK_PS(void *p_arg);
void TASK_ADC(void *p_arg);
