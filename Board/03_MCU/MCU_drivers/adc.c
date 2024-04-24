//======================================================================
//文件名称：adc.c
//框架提供：苏州大学恩智浦嵌入式中心(sumcu.suda.edu.cn)
//版本更新：2019-9-20 V1.1
//功能描述：见本工程的<01_Doc>文件夹下Readme.txt文件
//======================================================================
#include "includes.h"

//======================================================================
//函数名称：adc_init
//功能概要：初始化一个AD通道号与采集模式
//参数说明：Channel：通道号；可选范围：ADC_CHANNEL_VREFINT、
//         ADC_CHANNEL_TEMPSENSOR、ADC_CHANNEL_x(1=<x<=16)、ADC_CHANNEL_VBAT
//         diff：差分选择。=1(AD_DIFF 1)，差分；=0(AD_SINGLE);
//         单端；ADC_CHANNEL_VREFINT、ADC_CHANNEL_TEMPSENSOR、ADC_CHANNEL_VBAT
//         强制为单端；ADC_CHANNEL_x(1=<x<=16)可选单端或者差分模式
//======================================================================

void adc_init(uint16_t Channel,uint8_t Diff)
{
    //（1）开启ADC时钟，频率=总线时钟/4,48MHz，ADC时钟不超过14MHz
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
    ADC1_COMMON->CCR |= ADC_CCR_CKMODE; //4分频

    //（2）退出掉电状态并使能稳压器,ADC默认处于掉电状态以降低功耗
    //【20220614修改】
    if(Diff == AD_DIFF)
    {
    	ADC1->CR |= (1<<30);       //在差分输入模式下启动校准
    }else
    {
    	ADC1->CR &= ~(1<<30);      //在单端输入模式下启动校准
    }
    ADC1->CR &= ~ADC_CR_DEEPPWD;
    ADC1->CR |= 1<<31;             //开始校准
    ADC1->CR |= ADC_CR_ADVREGEN;   
    for(volatile uint32_t i = 0;i<48000;i++) //延时1毫秒以稳定
    //（3）使能ADC相关内部采集功能
    if(Channel == 0)
    {
        ADC1_COMMON->CCR |= ADC_CCR_VREFEN;  //使能参考电压采集功能
    }
    else if(Channel == 17)
    {
        ADC1_COMMON->CCR |= ADC_CCR_TSEN;    //使能温度采集功能
    }
    else if(Channel == 18)
    {
        ADC1_COMMON->CCR |= ADC_CCR_VBATEN;  //使能基准电压采集功能
    }
    
    //（4）初始化ADC控制寄存器,清零各个控制位
    ADC1->CR &= 0x3fffffc0;
    //（5）单端差分选择
    if (Diff)
    {
        BSET(Channel,ADC1->DIFSEL);  
    }
    else
    {
        BCLR(Channel,ADC1->DIFSEL);	
    }
    
    //（6）开启ADC
    ADC1->CR |= ADC_CR_ADEN;
    
    //（7）设置采样时间为12.5个时钟周期
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
    //（8）配置寄存器CFGR：精度12位、右对齐、单次单通道转换
    ADC1->CFGR &= 0xffffffe7;           //精度设置为12位
    ADC1->CFGR &= 0xffffffdf;           //数据对齐方式右对齐
    ADC1->CFGR |= ADC_CFGR_DISCEN;      //不连续转换模式
    ADC1->CFGR &= ~ADC_CFGR_CONT;       //单次转换模式
    ADC1->CFGR &= ~ADC_CFGR_DISCNUM;    //一个通道
    ADC1->CFGR &= ~ADC_CFGR_EXTEN;      //禁止硬件触发检测
    //（9）常规通道序列长度为1
    ADC1->SQR1 &= ~ADC_SQR1_L;	   
}

//============================================================================
//函数名称：adc_read
//功能概要：初始化一个AD通道与引脚采集模式
//参数说明：diff：差分选择。=1(AD_DIFF 1)，差分；=0(AD_SINGLE);
//         单端；DC_CHANNEL_VREFINT、ADC_CHANNEL_TEMPSENSOR、ADC_CHANNEL_VBAT
//         强制为单端；ADC_CHANNEL_x(1=<x<=16)可选单端或者差分模式
//============================================================================
uint16_t adc_read(uint8_t Channel)
{
    uint16_t ADCResult;   //用于存放AD值
    volatile int i,t;
    ADCResult=0;
    //（1）开启ADC,使能稳压器
    ADC1->CR |= ADC_CR_ADEN;
    ADC1->CR |= ADC_CR_ADVREGEN;
    for (i=0;i<=150;i++) __asm("nop");
    //（2）清空第一次转换序列
    ADC1->SQR1 &= 0xFFFFF83F; 
    //（3）所选通道加入第一次转换序列中
    ADC1->SQR1 |=((uint32_t)Channel<<6);
    //（4）开始转换
    ADC1->CR |= ADC_CR_ADSTART;
    //（5）等待转换完成，获取转换结果
    for (t = 0; t < 0xFBBB; t++)//查询指定次数
    {
        for (i=0;i<=20;i++) __asm("nop");
        //判断转换是否完成
        if (BGET(2,ADC1->ISR)==1)  //转换完成
        {
            ADCResult = ADC1->DR;   //读取数据,清转换完成标志位
            break;
        }
    } 
    return ADCResult;
}


//======================================================================
//函 数 名:adc_mid                                                  
//功    能:获取通道channel中值滤波后的A/D转换结果                    
//参    数:channel = 通道号                                           
//返    回:该通道中值滤波后的A/D转换结果                         
//内部调用:adc_read                                               
//======================================================================
uint16_t adc_mid(uint16_t Channel)
{
    uint16_t a,b,c;
    volatile uint32_t i;
    //1.取三次A/D转换结果
    a = adc_read(Channel);
    for (i=0;i<1500;i++) __asm("nop");
    b = adc_read(Channel);
    for (i=0;i<1500;i++) __asm("nop");
    c = adc_read(Channel);
   // 2.从三次A/D转换结果中取中值
    return  a > b ? (b > c ? b : ( a > c ? c : a)) : ( a > c ? a: (b > c ? c : a));
}

//======================================================================
//函 数 名:adc_ave                                                    
//功    能:1路A/D转换函数(均值滤波),通道channel进行n次中值滤波,求和再作  
//          均值,得出均值滤波结果                                        
//参    数:channel = 通道号,n = 中值滤波次数                               
//返    回:该通道均值滤波后的A/D转换结果                                   
//内部调用:adc_mid                                                          
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
//函数名称：adc_mcu_temp
//功能概要：将读到的芯片内部mcu温度AD值转换为实际温度
//参数说明：mcu_temp_AD：通过adc_read函数得到的AD值
//函数返回：实际温度值
//============================================================================
float adc_mcu_temp(uint16_t mcu_temp_AD)
{
	float mcu_temp_result;
	mcu_temp_result=(float)(6+55+(100*((float)(mcu_temp_AD) - AD_CAL1))/(AD_CAL2 - AD_CAL1));
	return mcu_temp_result;
 
}





