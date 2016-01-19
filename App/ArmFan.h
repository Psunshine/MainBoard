#pragma once
#include "includes.h"

#define HEIGHT_MIN      0
#define HEIGHT_MAX      500

#define DIRECTION_MIN   0
#define DIRECTION_MAX   180

#define SPEED_MIN		0
#define SPEED_MAX		500

// ���Ȼ�е�ۿ�����
typedef struct
{
	s16 height;
	s16 FanSpeed;
	s16 FanDirection;
}Fan_TypeDef;

void Set_Fan(s16 height,s16 FanSpeed,s16 FanDirection);

extern Fan_TypeDef FanCMD;