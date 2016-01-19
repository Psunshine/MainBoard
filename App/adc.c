#include "adc.h"

	


#define ADC1_DR_Address  ((uint32_t)0X4001204C)

#define  N   20      //ÿͨ����20�� 
#define  M  3      //Ϊ2��ͨ��   
vu16  AD_Value[N][M];   //�������ADCת�������Ҳ��DMA��Ŀ���ַ

void ADC_Configuration(void)

{    
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
    DMA_InitTypeDef       DMA_InitStructure;
    
	ADC_InitTypeDef       ADC_InitStructure;
	
	
	
	//�ٿ���ADC��GPIO���ʱ�Ӻͳ�ʼ��GPIO
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_DMA2, ENABLE);//ʹ��GPIOAʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG |RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	
	
	
	//�ȳ�ʼ��ADC1ͨ��10-13 IO��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PC0-3 ͨ��11-14
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ�� 
	
	
	
	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);        //ADC1��λ
	
    // RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);     //��λ����    
	
	
	
	//�ڳ�ʼ��ͨ������
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	
	ADC_CommonInitStructure.ADC_TwoSamplingDelay =
		
		ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
		
		//ADC_DMAAccessMode_Disabled; //DMAʧ��
	
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��
	
	//ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz
	
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
	
	DMA_InitStructure.DMA_BufferSize = N*M;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&AD_Value;   //Ŀ������λ
    
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_MemoryInc_Enable
    
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    
    DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)ADC1_DR_Address;  //ADC->DR��ַ
    
    DMA_InitStructure.DMA_PeripheralBurst =DMA_PeripheralBurst_Single;
    
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    
    DMA_Init(DMA2_Stream0,&DMA_InitStructure);
    
    DMA_Cmd(DMA2_Stream0,ENABLE);
    
	//�۳�ʼ��ADC1��ز���
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	
	ADC_InitStructure.ADC_ScanConvMode =  ENABLE;   //DISABLE;//��ɨ��ģʽ  //��ͨ��ģʽʹ��
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//�ر�����ת��
	
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	
	//��ֹ������⣬ʹ���������
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���      
	
	ADC_InitStructure.ADC_NbrOfConversion = 3;//2��ת���ڹ���������
	
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
    
    
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_480Cycles);
    
    ADC_RegularChannelConfig(ADC1,ADC_Channel_12,2,ADC_SampleTime_480Cycles);
	
    ADC_RegularChannelConfig(ADC1,ADC_Channel_13,3,ADC_SampleTime_480Cycles);
	
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);  //if not have this function , can't get new value
     
    ADC_DMACmd(ADC1, ENABLE);  //make enable ADC1 WITH DMA2 
    //�ܿ���ADCת��
    
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����  
    
    
    ADC_SoftwareStartConv(ADC1);         //ʹ��ָ����ADC1�����ת����������
  
    
    
	
	
	
}                         
//���ADCֵ

//ch:ͨ��ֵ 0~16: ch: @ref ADC_channels

//����ֵ:ת�����
//u16 Get_Adc(u8 ch)  
//
//{
//	
//	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//
//	
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles);              
//	
//	ADC_SoftwareStartConv(ADC1);         //ʹ��ָ����ADC1�����ת���������� 
//	
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
//	
//	return ADC_GetConversionValue(ADC1);     //�������һ��ADC1�������ת�����
//	
//}
u32 temp_val[20];

u16 Get_Adc_Average(u8 ch)
{
	
    u32 temp;
    u16 minAverage;
	u8 t;
	for(t=0;t<20;t++)
	{
		temp_val[t]=AD_Value[t][ch]*3300/4096;
      //  delay_us(5000);
        
	}
    for(t=0;t<20;t++)
    {
      temp=temp_val[t];
      for(int j=t+1;j<20;j++)
      {
        if(temp>temp_val[j])
      {
        temp_val[t] = temp_val[j]; 
        temp_val[j] = temp;
       // temp = temp_val[j];
         
        
      }
      else ;
      }
      
    }
    
    for (t=5;t<10;t++)
    {
      minAverage+=temp_val[t];
    }
	return (minAverage/5);
} 	 


//void delay_us(u32 us) 
//{
//	u32 temp;
//      
//	SysTick_Config(us*21);
//	do
//    
//	{
//	   
//		temp = SysTick->CTRL;  //ʱ�䵽��֮��,��λ����Ӳ����1,������ѯ���Զ���0
//    
//	}
//    
//	while(temp & 0x01 && !(temp &(1<<16))); //��ѯ
//    
//	SysTick->CTRL = 0x00;  //�رռ�����
//    
//	SysTick->VAL = 0x00;   //���val
//}










