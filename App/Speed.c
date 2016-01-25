
#include "includes.h"

SPEEDTypeDef Speed;
RobotWspeed robotw;


u16 can1id = 0;
u8 can1data[8];
u16 can2id = 0x11;
u8 can2data[8];
u8 count_speed = 0;
u8 count_PWM = 0;

u16 can1id_diceng_return = 0x10;                                         //�ײ�巵��ID
u16 can1id_pagan_return = 0x11;                                          //���˳�����ID

u16 can1id_speed_diceng_1 = 0x80;        //�ٶ����� ��� 0,1
u16 can1id_speed_diceng_2 = 0xA0;        //�ٶ����� ��� 2,3

u16 can1id_speed_pagan_1 = 0x81;        //�ٶ����� ��� 0,1
u16 can1id_speed_pagan_2 = 0xA1;        //�ٶ����� ��� 2,3

u8 can1data_speed_wheel[8] ;//���0�����1
//u8 can1data_speed_wheel_2_3[8] ;//���2�����3

u8 lastspeed_1[8];
u8 lastspeed_2[8];
u8 count_1=0;
u8 count_2=0;

u16 can1id_PWM_diceng_1 = 0xC0;       //PWM���� ���0,1
u16 can1id_PWM_diceng_2 = 0xE0;       //PWM���� ���2,3

u16 can1id_PWM_pagan_1 = 0xC1;        //�ٶ����� ��� 0,1
u16 can1id_PWM_pagan_2 = 0xE1;        //�ٶ����� ��� 2,3

u8 can1data_PWM_0_1[8] = {1,1,1,1,1,1,1,1 };//���0�����1
u8 can1data_PWM_2_3[8] = {1,1,1,1,1,1,1,1 };//���2�����3

u8 FLAG ;

u16 can1id_diceng = 0x90;                                                //�ײ���ʼ��
u16 can1id_pagan = 0x91;                                                 //���˳�ʼ��
u8 can1data_disable[8] = { 0,0,0,0,0,0,0,0};                     //ʧ��ģʽ
u8 can1data_speed[8] = { 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};//�ٶȻ�ģʽ
u8 can1data_PWM[8] = { 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};//PWM����ģʽ


// �ĸ������ٶȷֽ�
void WSpeed(float vx, float vy,float wz)    //�����ķ���˶�����
{  
  robotw.speed0=(s32)(((vx-vy)+(LL1+LL2)*wz/550)/PI/R/2*1000);//0
  robotw.speed1=(s32)((-(vx+vy)+(LL1+LL2)*wz/550)/PI/R/2*1000);//1
  robotw.speed2=(s32)(((vx-vy)-(LL1+LL2)*wz/550)/PI/R/2*1000);//2
  robotw.speed3=(s32)((-(vx+vy)-(LL1+LL2)*wz/550)/PI/R/2*1000);//3
  
  //�����ٶ�����
  RANGE_LIMIT(robotw.speed0,-MAX_WHELL_SPEED,MAX_WHELL_SPEED);
  RANGE_LIMIT(robotw.speed1,-MAX_WHELL_SPEED,MAX_WHELL_SPEED);
  RANGE_LIMIT(robotw.speed2,-MAX_WHELL_SPEED,MAX_WHELL_SPEED);
  RANGE_LIMIT(robotw.speed3,-MAX_WHELL_SPEED,MAX_WHELL_SPEED);
}

void Speed_Mode_Init(void)
{
	// ����Ϊ�ٶ�ģʽ
    InsertQueue(&CAN2_TX_Queue,can1id_diceng,can1data_speed);
    CAN2_Send_Msg();
}

//ͨ��CAN���߷����ٶ������������
void Set_Speed(void)
{
    WSpeed(Speed.vx,Speed.vy,Speed.wz);
           
	//���ٶ�ֵ��ִ��
    can1data_speed_wheel[0] = robotw.speed0 >> 8;
    can1data_speed_wheel[1] = robotw.speed0 ;
    can1data_speed_wheel[2] = robotw.speed1 >> 8;
    can1data_speed_wheel[3] = robotw.speed1;
    can1data_speed_wheel[4] = robotw.speed2 >> 8;
    can1data_speed_wheel[5] = robotw.speed2;
    can1data_speed_wheel[6] = robotw.speed3 >> 8;
    can1data_speed_wheel[7] = robotw.speed3;
  
	//�����ٶ�ֵ����ͬ����ֻ����һ��
    if(!IsEmptyQueue(&CAN2_RX_Queue))
    {
        GetQueue(&CAN2_RX_Queue,&can1id,can1data);
        FLAG =1;
        if( (can1id == can1id_diceng_return )||(FLAG ==1 ))
        {    
            for ( int i=0;i<8;i++ )
            {
                if( can1data[i] == 0x55 ) 
                { 
                    count_speed++;
                }   
                else if( can1data[i] == 0xAA ) 
                { 
                    count_PWM++;
                } 
                else if( can1data[i] == 0 )
                {
                    break;
                }
                else ;
            }
        }
    }
	if(FLAG == 1)
	{
	   // OS_ERR err;
		
		 
		for(int i=0;i<8;i++)
		{
			if(lastspeed_1[i]!=can1data_speed_wheel[i])
			{count_1++;}
			else ;
			
		}
		if(count_1 )
		{
			InsertQueue(&CAN2_TX_Queue,can1id_speed_diceng_1,can1data_speed_wheel);
			CAN2_Send_Msg();
			count_1 =0;
			memcpy(lastspeed_1,can1data_speed_wheel,8);
		}
		else;
            
            
           // OSTimeDly(100,OS_OPT_TIME_MATCH,&err);
            
//            for(int i=0;i<8;i++)
//            {
//                if(lastspeed_2[i]!=can1data_speed_wheel_2_3[i])
//                {count_2++;}
//                else ;
//                
//            }
//            if(count_2)
//            {
//                InsertQueue(&CAN2_TX_Queue,can1id_speed_diceng_2,can1data_speed_wheel_2_3);
//                CAN2_Send_Msg();
//                count_2 =0;
//                memcpy(lastspeed_2,can1data_speed_wheel_2_3,8);
//            }
//            else;
		count_speed = 0;
	}
        
//        if(count_PWM == 8)
//        {
//            InsertQueue(&CAN2_TX_Queue,can1id_PWM_diceng_1,can1data_PWM_0_1); 
//            CAN2_Send_Msg();
//            InsertQueue(&CAN2_TX_Queue,can1id_PWM_diceng_2,can1data_PWM_2_3); 
//            CAN2_Send_Msg();
//            count_PWM =0 ;
//        }
//        else ;
        
    
}
