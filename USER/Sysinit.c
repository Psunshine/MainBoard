#include "includes.h"

Time_TypeDef Time;

// ��ʼ����Ӧ
void Sys_Start(void)
{
	OS_ERR err;
	
	RED_Toggle();
	BEEP_Toggle();
	OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);//���ʱ����Ȼ��ֹ80ms,��Լ200ms
	RED_Toggle();
	BEEP_Toggle();
	OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
	YELLOW_Toggle();
	BEEP_Toggle();
	OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
	CORE_Toggle();
	YELLOW_Toggle();
	BEEP_Toggle();
	OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
	GREEN_Toggle();
	BEEP_Toggle();
	OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
	GREEN_Toggle();
	BEEP_Toggle();
	OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
	BLUE_Toggle();
	BEEP_Toggle();
	OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
	BLUE_Toggle();
	BEEP_Toggle();
	CORE_Toggle();
}

/*******************************************************************************
�� �� ����Struct_Init(void)
��    ����ϵͳ�ṹ���ʼ��ָ��
*******************************************************************************/
void Struct_Init(void)
{

}

