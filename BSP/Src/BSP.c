/**
  ******************************************************************************
  * @�ļ�	BSP.c
  * @����	2015-2-15
  * @˵��	UCOSIII�弶֧�ְ�
  ******************************************************************************
  * ������ͨ��ѧ-ͨ��1208-��С��
  *
  * ˵��:	IAR�汾:7.20.2.7314
  *      	���ļ��汾:��׼�� V1.3.0
  * �ļ�����:	Tab Size:4
  * �Զ�����:	Body:4
  * ��ϸ˵����Documents��Readme.txt
  */

#include "BSP.h"


/*******************************************************************************
�� �� ����CPU_INT32U  BSP_CPU_ClkFreq (void)
��    ������ȡϵͳHCLKʱ��Ƶ��
��    �أ�HCLKʱ��Ƶ��
*******************************************************************************/
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);
    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/*******************************************************************************
�� �� ����SysTickInit(void)
��    ����STM32��SysTick��ʼ����ΪUCOSIII�ṩʱ��
*******************************************************************************/
void SysTickInit(void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts); 
}

/*******************************************************************************
�� �� ����CPU_TS_TmrInit (void)
��    ����ϵͳʱ�����ʼ��
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
	CPU_INT32U fclk_freq; 
	
	fclk_freq = BSP_CPU_ClkFreq(); 
	
	CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*******************************************************************************
�� �� ����CPU_TS_TMR  CPU_TS_TmrRd (void)
��    ������ʱ�������ֵ
��    �أ������ļ���ֵ
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return (SysTick->VAL);
}
#endif

/*
*********************************************************************************************************
*                                         CPU_TSxx_to_uSec()
*
* Description : Convert a 32-/64-bit CPU timestamp from timer counts to microseconds.
*
* Argument(s) : ts_cnts   CPU timestamp (in timestamp timer counts [see Note #2aA]).
*
* Return(s)   : Converted CPU timestamp (in microseconds           [see Note #2aD]).
*
* Caller(s)   : Application.
*
*               This function is an (optional) CPU module application programming interface (API) 
*               function which MAY be implemented by application/BSP function(s) [see Note #1] & 
*               MAY be called by application function(s).
*
* Note(s)     : (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions that MAY be 
*                   optionally defined by the developer when either of the following CPU features is 
*                   enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) The amount of time measured by CPU timestamps is calculated by either of 
*                       the following equations :
*
*                                                                        10^6 microseconds
*                       (1) Time measured  =   Number timer counts   *  -------------------  *  Timer period
*                                                                            1 second
*
*                                              Number timer counts       10^6 microseconds
*                       (2) Time measured  =  ---------------------  *  -------------------
*                                                Timer frequency             1 second
*
*                               where
*
*                                   (A) Number timer counts     Number of timer counts measured
*                                   (B) Timer frequency         Timer's frequency in some units 
*                                                                   of counts per second
*                                   (C) Timer period            Timer's period in some units of 
*                                                                   (fractional)  seconds
*                                   (D) Time measured           Amount of time measured, 
*                                                                   in microseconds
*
*                   (b) Timer period SHOULD be less than the typical measured time but MUST be less 
*                       than the maximum measured time; otherwise, timer resolution inadequate to 
*                       measure desired times.
*
*                   (c) Specific implementations may convert any number of CPU_TS32 or CPU_TS64 bits 
*                       -- up to 32 or 64, respectively -- into microseconds.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;

    
    fclk_freq = BSP_CPU_ClkFreq();    
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;
    

    fclk_freq = BSP_CPU_ClkFreq();    
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


//1ms����ʽ��ʱ
void Delay_ms(unsigned int t)
{
	int volatile tmp1;
	int i;
	for(i = 0;i < 8400;i++)
	{
		tmp1 = t;
		while(tmp1--);
	}
}