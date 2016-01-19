
#include "includes.h"

//CAN1��CAN2���ĸ�����
_CANQUEUE CAN1_TX_Queue;
_CANQUEUE CAN1_RX_Queue;
_CANQUEUE CAN2_TX_Queue;
_CANQUEUE CAN2_RX_Queue;

/*******************************************************************
��������: ���ͨ�Ŷ���
********************************************************************/
void ClearQueue(_CANQUEUE* canqueue)
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����	
    int i;

    for(i = 0; i < MAX_CAN_QUEUE_SIZE; i++)
    {
        memset(&canqueue->Elem[i],0,sizeof(_CANMSG));
    }

    canqueue->front = 0;
    canqueue->rear = 0;
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
}
/*******************************************************************
��������: �ж�CAN�����Ƿ�Ϊ��
�� �� ֵ: 1: ��; 0:�ǿ�
********************************************************************/
u8 IsEmptyQueue(_CANQUEUE* canqueue)
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
    if(canqueue->front == canqueue->rear)
    {
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 1;
    }
    else
    {
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 0;
    }
}
/*******************************************************************
��������: �ж����Ƿ���
�� �� ֵ: 1: ��; 0:����
********************************************************************/
u8 IsFullQueue(_CANQUEUE* canqueue)
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
    if(((MAX_CAN_QUEUE_SIZE - canqueue->front) + canqueue->rear) == (MAX_CAN_QUEUE_SIZE - 1))
    {
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 1;
    }
    else
    {
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 0;
    }
}
/*******************************************************************
��������: �����ݲ������
��ڲ���: element:����Ԫ��
�� �� ֵ: 1: �ɹ�; 0:ʧ��
********************************************************************/
u8 InsertQueue(_CANQUEUE* canqueue,u16 CAN_ID,u8 Msg[8])
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
    if(!IsFullQueue(canqueue))//�Ƿ�Ϊ��
    {
		_CANMSG CANMsg;
		CANMsg.CAN_ID  = CAN_ID;
		memcpy(CANMsg.Msg,Msg,2*sizeof(Msg));
		memcpy(&canqueue->Elem[canqueue->rear],&CANMsg,sizeof(CANMsg));
		
        canqueue->rear = (canqueue->rear + 1) % MAX_CAN_QUEUE_SIZE;
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 1;
    }
    else                       //������
    {
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 0;
    }
}

/*******************************************************************
��������: �������ö���ͷָ��
��ڲ���: head: ��ͷ
********************************************************************/
void SetHeadQueue(_CANQUEUE* canqueue,u16 head)
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
    if(canqueue->front != canqueue->rear)
    {
        canqueue->front = head;
    }
	CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����

}
/*******************************************************************
��������: ȡ������һ��Ԫ��,fifo
��ڲ���: *element;����
�� �� ֵ: 1: �ɹ� 0: ʧ��
********************************************************************/
u8 GetQueue(_CANQUEUE* canqueue,u16* CAN_ID,u8 Msg[])
{
	CPU_SR_ALLOC();// ����cpu_sr
	////
	CPU_CRITICAL_ENTER(); // �ؼ��Σ��ر�ȫ���жϣ��Ӿ��������Ӵ����
    if(!IsEmptyQueue(canqueue))//���зǿ�
    {		
		_CANMSG CANMsg;
		memcpy(&CANMsg,&canqueue->Elem[canqueue->front],sizeof(_CANMSG));//�õ�����
		
		*CAN_ID  = CANMsg.CAN_ID;
        memcpy(Msg,CANMsg.Msg,sizeof(CANMsg.Msg));//�õ�����
		// ��ͷ��һ
		canqueue->front = (canqueue->front + 1) % MAX_CAN_QUEUE_SIZE ;
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 1;
    }
    else
    {
		CPU_CRITICAL_EXIT(); // �˳��ؼ��Σ�����ȫ���жϣ��Ӿ��������Ӵ����
        return 0;                               //��ָ����ͷ����
    }
}
/*******************************************************************
��������: can���ݴ���
********************************************************************/
void Can_data_process(_CANQUEUE* canqueue)
{
//    u16 head;
// //   u32 i;
//    CanRxMsg RxMessage;
//    CanTxMsg TxMessage;
//
//
//    head = CANQueue.front;
//
//    if(1 == GetCanQueue(&RxMessage))
//    {
//        head = (head + 1) % MAX_CAN_SIZE;     //��ѯͷǰ��
//        SetHeadCanQueue(head);
//      //  printf("RxMessage.StdId is 0x%x\r\n",RxMessage.StdId);
//      //  printf("RxMessage.DLC is 0x%x\r\n",RxMessage.DLC);
//      //  for(i = 0; i < RxMessage.DLC; i++)
//      //  {
//      //      printf("data[%d] is 0x%x\r\n",i,RxMessage.Data[i]);
//      //  }
//      //  printf("\r\n");
//        // �ѽ��յ������ݷ���ȥ
//        /* transmit */
//      //  TxMessage.StdId=RxMessage.StdId;
//        TxMessage.StdId=0x5f1;
//        TxMessage.RTR=RxMessage.RTR;
//        TxMessage.IDE=RxMessage.IDE;
//        TxMessage.DLC=RxMessage.DLC;
//        memcpy(TxMessage.Data,RxMessage.Data,TxMessage.DLC);
//        //Can_Send_Msg(TxMessage);
//
//    }
//    else
//    {
//       // printf("CAN queue is empty\r\n");
//    }



}