/**
  ******************************************************************************
  * @�ļ�	main.c
  * @����	2015-2-15
  * @˵��	�������
  ******************************************************************************
  * ������ͨ��ѧ-ͨ��1208-��С��
  *
  * ˵��:	IAR�汾:7.20.2.7314
  *      	���ļ��汾:��׼�� V1.3.0
  *         UCOS�汾:3.04.04
  * �ļ�����:	Tab Size:4
  * �Զ�����:	Body:4
  * ��ϸ˵����Documents��Readme.txt
  ******************************************************************************
  */
#include "includes.h"

/*******************************************************************************
�� �� ����main(void)
��    �����������
*******************************************************************************/
void main(void)
{
    OS_ERR err;
    
    OSInit(&err); 
    
    OSTaskCreate((OS_TCB     *)&TASK_START_TCB,
                 (CPU_CHAR   *)"TASK_START",
                 (OS_TASK_PTR)TASK_START,
                 (void       *)0,
                 (OS_PRIO    ) STARTUP_TASK_PRIO,
                 (CPU_STK    *)&TASK_START_STK[0],
                 (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void       *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                 (OS_ERR     *)&err);    
    
    OSStart(&err);
}

