//======================================================================
//�ļ����ƣ�adc.c
//����ṩ�����ݴ�ѧ������Ƕ��ʽ����(sumcu.suda.edu.cn)
//�汾���£�2019-9-20 V1.1
//�����������������̵�<01_Doc>�ļ�����Readme.txt�ļ�
//======================================================================
#include "includes.h"

//======================================================================
//�������ƣ�adc_init
//���ܸ�Ҫ����ʼ��һ��ADͨ������ɼ�ģʽ
//����˵����Channel��ͨ���ţ���ѡ��Χ��ADC_CHANNEL_VREFINT��
//         ADC_CHANNEL_TEMPSENSOR��ADC_CHANNEL_x(1=<x<=16)��ADC_CHANNEL_VBAT
//         diff�����ѡ��=1(AD_DIFF 1)����֣�=0(AD_SINGLE);
//         ���ˣ�ADC_CHANNEL_VREFINT��ADC_CHANNEL_TEMPSENSOR��ADC_CHANNEL_VBAT
//         ǿ��Ϊ���ˣ�ADC_CHANNEL_x(1=<x<=16)��ѡ���˻��߲��ģʽ
//======================================================================

void adc_init(uint16_t Channel,uint8_t Diff)
{
    //��1������ADCʱ�ӣ�Ƶ��=����ʱ��/4,48MHz��ADCʱ�Ӳ�����14MHz
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
    ADC1_COMMON->CCR |= ADC_CCR_CKMODE; //4��Ƶ

    //��2���˳�����״̬��ʹ����ѹ��,ADCĬ�ϴ��ڵ���״̬�Խ��͹���
    //��20220614�޸ġ�
    if(Diff == AD_DIFF)
    {
    	ADC1->CR |= (1<<30);       //�ڲ������ģʽ������У׼
    }else
    {
    	ADC1->CR &= ~(1<<30);      //�ڵ�������ģʽ������У׼
    }
    ADC1->CR &= ~ADC_CR_DEEPPWD;
    ADC1->CR |= 1<<31;             //��ʼУ׼
    ADC1->CR |= ADC_CR_ADVREGEN;   
    for(volatile uint32_t i = 0;i<48000;i++) //��ʱ1�������ȶ�
    //��3��ʹ��ADC����ڲ��ɼ�����
    if(Channel == 0)
    {
        ADC1_COMMON->CCR |= ADC_CCR_VREFEN;  //ʹ�ܲο���ѹ�ɼ�����
    }
    else if(Channel == 17)
    {
        ADC1_COMMON->CCR |= ADC_CCR_TSEN;    //ʹ���¶Ȳɼ�����
    }
    else if(Channel == 18)
    {
        ADC1_COMMON->CCR |= ADC_CCR_VBATEN;  //ʹ�ܻ�׼��ѹ�ɼ�����
    }
    
    //��4����ʼ��ADC���ƼĴ���,�����������λ
    ADC1->CR &= 0x3fffffc0;
    //��5�����˲��ѡ��
    if (Diff)
    {
        BSET(Channel,ADC1->DIFSEL);  
    }
    else
    {
        BCLR(Channel,ADC1->DIFSEL);	
    }
    
    //��6������ADC
    ADC1->CR |= ADC_CR_ADEN;
    
    //��7�����ò���ʱ��Ϊ12.5��ʱ������
    if((int)Channel >= 0 && (int)Channel <= 9 )
    {
        BCLR(Channel*3,ADC1->SMPR1);
        BSET(Channel*3+1,ADC1->SMPR1);
        BCLR(Channel*3+2,ADC1->SMPR1);
    }
    if((int)Channel >= 10 && (int)Channel <= 18 )
    {
        BCLR((Channel%10)*3,ADC1->SMPR2);
        BSET((Channel%10)*3+1,ADC1->SMPR2);
        BCLR((Channel%10)*3+2,ADC1->SMPR2);
    }
    //��8�����üĴ���CFGR������12λ���Ҷ��롢���ε�ͨ��ת��
    ADC1->CFGR &= 0xffffffe7;           //��������Ϊ12λ
    ADC1->CFGR &= 0xffffffdf;           //���ݶ��뷽ʽ�Ҷ���
    ADC1->CFGR |= ADC_CFGR_DISCEN;      //������ת��ģʽ
    ADC1->CFGR &= ~ADC_CFGR_CONT;       //����ת��ģʽ
    ADC1->CFGR &= ~ADC_CFGR_DISCNUM;    //һ��ͨ��
    ADC1->CFGR &= ~ADC_CFGR_EXTEN;      //��ֹӲ���������
    //��9������ͨ�����г���Ϊ1
    ADC1->SQR1 &= ~ADC_SQR1_L;	   
}

//============================================================================
//�������ƣ�adc_read
//���ܸ�Ҫ����ʼ��һ��ADͨ�������Ųɼ�ģʽ
//����˵����diff�����ѡ��=1(AD_DIFF 1)����֣�=0(AD_SINGLE);
//         ���ˣ�DC_CHANNEL_VREFINT��ADC_CHANNEL_TEMPSENSOR��ADC_CHANNEL_VBAT
//         ǿ��Ϊ���ˣ�ADC_CHANNEL_x(1=<x<=16)��ѡ���˻��߲��ģʽ
//============================================================================
uint16_t adc_read(uint8_t Channel)
{
    uint16_t ADCResult;   //���ڴ��ADֵ
    volatile int i,t;
    ADCResult=0;
    //��1������ADC,ʹ����ѹ��
    ADC1->CR |= ADC_CR_ADEN;
    ADC1->CR |= ADC_CR_ADVREGEN;
    for (i=0;i<=150;i++) __asm("nop");
    //��2����յ�һ��ת������
    ADC1->SQR1 &= 0xFFFFF83F; 
    //��3����ѡͨ�������һ��ת��������
    ADC1->SQR1 |=((uint32_t)Channel<<6);
    //��4����ʼת��
    ADC1->CR |= ADC_CR_ADSTART;
    //��5���ȴ�ת����ɣ���ȡת�����
    for (t = 0; t < 0xFBBB; t++)//��ѯָ������
    {
        for (i=0;i<=20;i++) __asm("nop");
        //�ж�ת���Ƿ����
        if (BGET(2,ADC1->ISR)==1)  //ת�����
        {
            ADCResult = ADC1->DR;   //��ȡ����,��ת����ɱ�־λ
            break;
        }
    } 
    return ADCResult;
}


//======================================================================
//�� �� ��:adc_mid                                                  
//��    ��:��ȡͨ��channel��ֵ�˲����A/Dת�����                    
//��    ��:channel = ͨ����                                           
//��    ��:��ͨ����ֵ�˲����A/Dת�����                         
//�ڲ�����:adc_read                                               
//======================================================================
uint16_t adc_mid(uint16_t Channel)
{
    uint16_t a,b,c;
    volatile uint32_t i;
    //1.ȡ����A/Dת�����
    a = adc_read(Channel);
    for (i=0;i<1500;i++) __asm("nop");
    b = adc_read(Channel);
    for (i=0;i<1500;i++) __asm("nop");
    c = adc_read(Channel);
   // 2.������A/Dת�������ȡ��ֵ
    return  a > b ? (b > c ? b : ( a > c ? c : a)) : ( a > c ? a: (b > c ? c : a));
}

//======================================================================
//�� �� ��:adc_ave                                                    
//��    ��:1·A/Dת������(��ֵ�˲�),ͨ��channel����n����ֵ�˲�,�������  
//          ��ֵ,�ó���ֵ�˲����                                        
//��    ��:channel = ͨ����,n = ��ֵ�˲�����                               
//��    ��:��ͨ����ֵ�˲����A/Dת�����                                   
//�ڲ�����:adc_mid                                                          
//======================================================================
uint16_t adc_ave(uint16_t Channel,uint8_t n) 
{
    uint16_t i;
    uint32_t j;
    j = 0;
    for (i = 0; i < n; i++) j += adc_mid(Channel);
    j = j/n;
    return (uint16_t)j;
}


//============================================================================
//�������ƣ�adc_mcu_temp
//���ܸ�Ҫ����������оƬ�ڲ�mcu�¶�ADֵת��Ϊʵ���¶�
//����˵����mcu_temp_AD��ͨ��adc_read�����õ���ADֵ
//�������أ�ʵ���¶�ֵ
//============================================================================
float adc_mcu_temp(uint16_t mcu_temp_AD)
{
	float mcu_temp_result;
	mcu_temp_result=(float)(6+55+(100*((float)(mcu_temp_AD) - AD_CAL1))/(AD_CAL2 - AD_CAL1));
	return mcu_temp_result;
 
}





