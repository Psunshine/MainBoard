#pragma once
#include "includes.h"


// ϵͳʱ��
typedef struct
{
	int hour;
	int min;
	int sec;
}Time_TypeDef;


void App_Init(void);
void Sys_Start(void);



// ϵͳ����ʱ��
extern	Time_TypeDef Time;
