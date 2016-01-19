/**
  ******************************************************************************
  * @�ļ�	TASK.c
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

#include "includes.h"



/* ����Ϊ��������Ķ�ջ�ռ� */
// ��������(����������main.c�е�����)
OS_TCB TASK_START_TCB;							// ����TCB���ƿ�
CPU_STK TASK_START_STK[STARTUP_TASK_STK_SIZE];	// �����ջ�ռ�
// LED����
OS_TCB TASK_LED_BEEP_TCB;							// ����TCB���ƿ�
CPU_STK TASK_LED_BEEP_STK[TASK_LED_BEEP_STK_SIZE];		// �����ջ�ռ�
// CAN��������
OS_TCB TASK_CAN_TCB;							// ����TCB���ƿ�
CPU_STK TASK_CAN_STK[TASK_CAN_STK_SIZE];		// �����ջ�ռ�
// ��ʱ����
OS_TCB TASK_TIME_TCB;							// ����TCB���ƿ�
CPU_STK TASK_TIME_STK[TASK_TIME_STK_SIZE];		// �����ջ�ռ�
// ��ʾ����
OS_TCB TASK_SHOW_TCB;							// ����TCB���ƿ�
CPU_STK TASK_SHOW_STK[TASK_SHOW_STK_SIZE];		// �����ջ�ռ�
// ���������ȡ����
OS_TCB TASK_KEY_TCB;							// ����TCB���ƿ�
CPU_STK TASK_KEY_STK[TASK_KEY_STK_SIZE];		// �����ջ�ռ�
// �ֱ���ȡ����
OS_TCB TASK_PS_TCB;							// ����TCB���ƿ�
CPU_STK TASK_PS_STK[TASK_KEY_STK_SIZE];		// �����ջ�ռ�
// �����ȡ����
OS_TCB TASK_ADC_TCB;							// ����TCB���ƿ�
CPU_STK TASK_ADC_STK[TASK_KEY_STK_SIZE];		// �����ջ�ռ�

float infrared_data1;
float infrared_data2;
float infrared_data3;
double distance1 ;
double distance2 ;
double fanheight ;
double ave_distance ;
double xielv ;

u16 can1id_diceng = 0x90;                                                //�ײ���ʼ��
u16 can1id_pagan = 0x91;                                                 //���˳�ʼ��
u8 can1data_disable[8] = { 0,0,0,0,0,0,0,0};                     //ʧ��ģʽ
u8 can1data_speed[8] = { 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};//�ٶȻ�ģʽ
u8 can1data_PWM[8] = { 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};//PWM����ģʽ

/*******************************************************************************
�� �� ����TASK_START(void)
��    ������������
*******************************************************************************/
void TASK_START(void)
{
    OS_ERR err;
    
	// ��ʼ��CPUʱ������жϺ�CPU name
    CPU_Init();
	// SysTick��ʼ��
    SysTickInit();
	// �ж�����������
	NVIC_Config();	
    PS_Init();	
    KEY_Init();	
	STATU_Init();
	LED_Init();
	BEEP_Init();
    CAN1_Init();
	CAN2_Init();
    OLED_Init();
    ADC_Configuration();
	FanCMD.height = 250;
	FanCMD.FanSpeed = 0;
	FanCMD.FanDirection = 55;
    
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running */
#endif
    
    OSTaskCreate((OS_TCB     *)&TASK_LED_BEEP_TCB,
                 (CPU_CHAR   *)"TASK_LED_BEEP",
                 (OS_TASK_PTR)TASK_LED_BEEP,
                 (void       *)0,
                 (OS_PRIO    )TASK_LED_BEEP_PRIO,
                 (CPU_STK    *)&TASK_LED_BEEP_STK[0],
                 (CPU_STK_SIZE)TASK_LED_BEEP_STK_SIZE / 10,
                 (CPU_STK_SIZE)TASK_LED_BEEP_STK_SIZE,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void       *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP), 
                 (OS_ERR     *)&err);	
	OSTaskCreate((OS_TCB     *)&TASK_CAN_TCB,
			 	(CPU_CHAR   *)"TASK_CAN",
			 	(OS_TASK_PTR)TASK_CAN,
			 	(void       *)0,
			 	(OS_PRIO    )TASK_CAN_PRIO,
			 	(CPU_STK    *)&TASK_CAN_STK[0],
			 	(CPU_STK_SIZE)TASK_CAN_STK_SIZE / 10,
			 	(CPU_STK_SIZE)TASK_CAN_STK_SIZE,
			 	(OS_MSG_QTY )0,
			 	(OS_TICK    )0,
			 	(void       *)0,
			 	(OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/), 
			 	(OS_ERR     *)&err);
	OSTaskCreate((OS_TCB     *)&TASK_TIME_TCB,
				 (CPU_CHAR   *)"TASK_TIME",
				 (OS_TASK_PTR)TASK_TIME,
				 (void       *)0,
				 (OS_PRIO    )TASK_TIME_PRIO,
				 (CPU_STK    *)&TASK_TIME_STK[0],
				 (CPU_STK_SIZE)TASK_TIME_STK_SIZE / 10,
				 (CPU_STK_SIZE)TASK_TIME_STK_SIZE,
				 (OS_MSG_QTY )0,
				 (OS_TICK    )0,
				 (void       *)0,
				 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
				 (OS_ERR     *)&err);
	OSTaskCreate((OS_TCB     *)&TASK_SHOW_TCB,
				 (CPU_CHAR   *)"TASK_SHOW",
				 (OS_TASK_PTR)TASK_SHOW,
				 (void       *)0,
				 (OS_PRIO    )TASK_SHOW_PRIO,
				 (CPU_STK    *)&TASK_SHOW_STK[0],
				 (CPU_STK_SIZE)TASK_SHOW_STK_SIZE / 10,
				 (CPU_STK_SIZE)TASK_SHOW_STK_SIZE,
				 (OS_MSG_QTY )0,
				 (OS_TICK    )0,
				 (void       *)0,
				 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
				 (OS_ERR     *)&err);
	OSTaskCreate((OS_TCB     *)&TASK_KEY_TCB,
				 (CPU_CHAR   *)"TASK_KEY",
				 (OS_TASK_PTR)TASK_KEY,
				 (void       *)0,
				 (OS_PRIO    )TASK_KEY_PRIO,
				 (CPU_STK    *)&TASK_KEY_STK[0],
				 (CPU_STK_SIZE)TASK_KEY_STK_SIZE / 10,
				 (CPU_STK_SIZE)TASK_KEY_STK_SIZE,
				 (OS_MSG_QTY )0,
				 (OS_TICK    )0,
				 (void       *)0,
				 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
				 (OS_ERR     *)&err);
    OSTaskCreate((OS_TCB     *)&TASK_PS_TCB,
				 (CPU_CHAR   *)"TASK_PS",
				 (OS_TASK_PTR)TASK_PS,
				 (void       *)0,
				 (OS_PRIO    )TASK_PS_PRIO,
				 (CPU_STK    *)&TASK_PS_STK[0],
				 (CPU_STK_SIZE)TASK_PS_STK_SIZE / 10,
				 (CPU_STK_SIZE)TASK_PS_STK_SIZE,
				 (OS_MSG_QTY )0,
				 (OS_TICK    )0,
				 (void       *)0,
				 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
				 (OS_ERR     *)&err);
        OSTaskCreate((OS_TCB     *)&TASK_ADC_TCB,
				 (CPU_CHAR   *)"TASK_ADC",
				 (OS_TASK_PTR)TASK_ADC,
				 (void       *)0,
				 (OS_PRIO    )TASK_ADC_PRIO,
				 (CPU_STK    *)&TASK_ADC_STK[0],
				 (CPU_STK_SIZE)TASK_ADC_STK_SIZE / 10,
				 (CPU_STK_SIZE)TASK_ADC_STK_SIZE,
				 (OS_MSG_QTY )0,
				 (OS_TICK    )0,
				 (void       *)0,
				 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
				 (OS_ERR     *)&err);
	// ɾ����������
    OSTaskDel((OS_TCB*)&TASK_START_TCB,(OS_ERR*)&err);
}

/*******************************************************************************
�� �� ����TASK_LED_BEEP(void *p_arg)
��    ����LED��������
*******************************************************************************/
void TASK_LED_BEEP(void *p_arg)
{
    OS_ERR err;
	

	
	Sys_Start();
    
    while (1)
    {
		CORE_Toggle();
		
		OSTimeDlyHMSM(0, 0, 0, 400,
                      OS_OPT_TIME_HMSM_NON_STRICT,
                      &err);
		
		CORE_Toggle();
		
		OSTimeDlyHMSM(0, 0, 1, 500,
                      OS_OPT_TIME_HMSM_NON_STRICT,
                      &err);
	}
}

/*******************************************************************************
�� �� ����TASK_CAN(void *p_arg)
��    ����CAN��������
*******************************************************************************/
void TASK_CAN(void *p_arg)
{
    OS_ERR err;
	
	
	
        // ����Ϊ�ٶ�ģʽ
    InsertQueue(&CAN2_TX_Queue,can1id_diceng,can1data_speed);
    CAN2_Send_Msg();
    while (1)
    {
		//��ȡ�ֱ�����
        Get_PS_Data();
      	PS_Digital_Mode();
		
		infrared_data1 = Get_Adc_Average(0);  
		infrared_data2 = Get_Adc_Average(1);
	
		distance1 = -17.59*log(infrared_data1)+ 146.48;
        distance2 = -15.95*log(infrared_data2)+ 134.42;
		
		
		infrared_data3 = Get_Adc_Average(2);   //���Ȼ�е�ۺ�������
		
		fanheight = -15.95*log(infrared_data3)+ 134.42;                             //������ܵ��߶�15
			
		ave_distance = (distance1+distance2)/2.0 ;
		
		xielv = (distance1-distance2)/50.0;    //���̻����˺��ⰲװ���50mm
		
		if(ave_distance<25)
		{
			if(ave_distance<10)
			{
				Speed.vy = -20;

			}
			else if(ave_distance>15)
			{
					
				Speed.vy = 20;
			}
			else 
			{
				Speed.vy = 0;
			}
			
			if(xielv<0)
			{
				Speed.wz = -5;

			}
			else if(xielv>0)
			{
					
				Speed.wz = 5;
			}
			else 
			{
				Speed.wz = 0;
			}	
		}
		else
		{
			Speed.vy = 0;
			Speed.wz = 0;
			
		}
        
		
		//���õ����ٶ�
        Set_Speed();
		//���÷��Ȼ�е���ٶȺ�λ��
		Set_Fan(FanCMD.height-(fanheight-15.0)*10.0,FanCMD.FanSpeed,FanCMD.FanDirection);
		
		OSTimeDlyHMSM(0, 0, 0, TASK_CAN_DELAY,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
	}
}

/*******************************************************************************
�� �� ����TASK_TIME(void *p_arg)
��    ������ʱ�������ȼ�12
*******************************************************************************/
void TASK_TIME(void *p_arg)
{
	OS_ERR err;
	CPU_TS OS_ticks;

	while(1)
	{
		// ��ȡϵͳTicks����ϵͳʱ��
		OS_ticks = OSTimeGet(&err);
		OS_ticks /= 1000;
		Time.sec = OS_ticks%60;
		Time.min = OS_ticks/60%60;
		Time.hour = OS_ticks/3600%60;			

		OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_NON_STRICT,
                      &err);
	}
}

/*******************************************************************************
�� �� ����TASK_SHOW(void *p_arg)
��    ������ʾ�������ȼ�13
*******************************************************************************/
void TASK_SHOW(void *p_arg)
{
	OS_ERR err;
	//char time_s[] 		= "  :  :  ";
	//char cpu_usage_s[]	= "00.00%";
	
	
	
	OLED_Init_Show();
	
	while(1)
	{
//		CPU_SR_ALLOC();// ����cpu_sr
//		
//		sprintf( time_s,"%2d:%2d:%2d",\
//						 Time.hour,Time.min,Time.sec);
//		sprintf( cpu_usage_s,"%2.2f%%",((float)OSStatTaskCPUUsage/100));
//				
//		
//		CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
//		// ʱ��
//		OLED_P6x8Str(0,2,(u8*)time_s);
//		// CPUʹ����
//		OLED_P6x8Str(66,2,"CPU:");
//		OLED_P6x8Str(90,2,(u8*)cpu_usage_s);
//		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

		OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_NON_STRICT,
                      &err);
	}
}

/*******************************************************************************
�� �� ����TASK_KEY(void *p_arg)
��    �������������ȡ����
*******************************************************************************/
void TASK_KEY(void *p_arg)
{
	OS_ERR err;
	
		
	while(1)
	{


		OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_NON_STRICT,
                      &err);
	}
}
/*******************************************************************************
�� �� ����TASK_PS(void *p_arg)
��    �����ֱ���ȡ����
*******************************************************************************/
void TASK_PS(void *p_arg)
{
	OS_ERR err;
	
	

	
	while(1)
	{
      

		OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_NON_STRICT,
                      &err);
	}
}
/*******************************************************************************
�� �� ����TASK_ADC(void *p_arg)
��    ���������ȡ����
*******************************************************************************/
void TASK_ADC(void *p_arg)
{
	OS_ERR err;
	
	

	
	while(1)
	{
      
	  //Set_Speed();
		OSTimeDlyHMSM(0, 0, 0, 20,
                      OS_OPT_TIME_HMSM_NON_STRICT,
                      &err);
        
	}
}