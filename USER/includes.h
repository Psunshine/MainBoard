/**
  ******************************************************************************
  * @�ļ�	includes.h
  * @����	2015-1-26
  * @˵��	����ͷ�ļ�
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "arm_math.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "arm_math.h"

/* BSP -----------------------------------------------------------------------*/
#include "BSP.h"
#include "BSPMisc.h"
#include "Usart1.h"
#include "Can1.h"
#include "Can2.h"
#include "oled.h"


/* APP -----------------------------------------------------------------------*/
#include "Sysinit.h"
#include "os.h"
#include "TASK.h"
#include "CANQueue.h"
#include "ArmFan.h"
#include "PS_Driver.h"
#include "speed.h"
#include "adc.h"



