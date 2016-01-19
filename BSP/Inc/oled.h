#pragma once
#include "includes.h"

#define USEOLED

/******************************OLED������غ궨��******************************/
// ����
#define OLED_GPIO_PORT      		GPIOD
#define OLED_GPIO_SDA        		GPIO_Pin_10
#define OLED_GPIO_SCL        		GPIO_Pin_8
#define OLED_GPIO_RST      			GPIO_Pin_9
#define OLED_GPIO_DC      			GPIO_Pin_11
// ʱ��
#define OLED_GPIO_Port_Enable()			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
/******************************OLED������غ궨��******************************/

/******************************OLED������غ궨��******************************/
#define OLED_RST_Clr() 	GPIO_ResetBits(OLED_GPIO_PORT,OLED_GPIO_RST)
#define OLED_RST_Set() 	GPIO_SetBits(OLED_GPIO_PORT,OLED_GPIO_RST)    //��OLEDģ��RST

#define OLED_DC_Clr() 	GPIO_ResetBits(OLED_GPIO_PORT,OLED_GPIO_DC)
#define OLED_DC_Set() 	GPIO_SetBits(OLED_GPIO_PORT,OLED_GPIO_DC)     //��OLEDģ��D/C

#define OLED_SDA_Clr() 	GPIO_ResetBits(OLED_GPIO_PORT,OLED_GPIO_SDA)
#define OLED_SDA_Set() 	GPIO_SetBits(OLED_GPIO_PORT,OLED_GPIO_SDA)    //��OLEDģ��MOSI

#define OLED_SCL_Clr() 	GPIO_ResetBits(OLED_GPIO_PORT,OLED_GPIO_SCL)
#define OLED_SCL_Set() 	GPIO_SetBits(OLED_GPIO_PORT,OLED_GPIO_SCL)    //��OLEDģ���CLK
/******************************OLED������غ궨��******************************/

// ����궨���������ʹ��UCOSIII����ϵͳ�е�ms��ʱ
#define	OLED_DLY_ms(ms)	OS_ERR err;\
	  					OSTimeDlyHMSM(0,0,0,ms,OS_OPT_TIME_HMSM_STRICT,&err);
//#define OLED_DLY_ms(ms) OLED_DLY_ms_i(ms);

#define X_WIDTH 		128
#define Y_WIDTH 		64


//OLED�����ú���
void OLED_WrDat(unsigned char data);//д����
void OLED_WrCmd(unsigned char cmd);//д����
void OLED_Set_Pos(unsigned char x, unsigned char y);//������ʼ������
void OLED_Fill(unsigned char bmp_data);//ȫ�����
void OLED_CLS(void);//����
void OLED_DLY_ms_i(int ms);
void OLED_Init(void);//��ʼ��
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);



void OLED_Init_Show(void);
//void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N);
//void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);

