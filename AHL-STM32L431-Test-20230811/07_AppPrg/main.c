#define GLOBLE_VAR
#include "includes.h"      //包含总头文件

uint8_t GPIO_Tsi(uint16_t port_pin);
void Delay_ms(uint16_t u16ms);
float TempRegression(uint16_t tmpAD);  //环境温度AD值转为实际温度
float TempTrans(uint16_t mcu_temp_AD); //mcu温度AD值转为实际温度

void set_Color(int *mCount,int *currentColor)
{
 		if (*mCount >= 40) {
	        *mCount = 0;
	        *currentColor = 0;
	    }
		
	    if (*mCount % 5 == 0) {
	        gpio_reverse(LIGHT_RED);
	        if (*mCount / 5 == 1) {
	            *currentColor = 	1;
	        } else if (*mCount / 5 == 3) {
	            *currentColor = 2;
	        } else if (*mCount / 5 == 5) {
	            *currentColor = 3;
	        } else if (*mCount / 5 == 7) {
	            *currentColor = 4;
	        }
	    }
	    
	    if (*mCount % 10 == 0) {
	        gpio_reverse(LIGHT_GREEN);
	        if (*mCount / 10 == 1) {
	            *currentColor = 5;
	        } else if (*mCount / 10 == 3) {
	            *currentColor = 6;
	        }
	    }
	    
	    if (*mCount % 20 == 0) {
	        gpio_reverse(LIGHT_BLUE);				
	        if (*mCount / 20 == 1) {
	            *currentColor = 7;
	        }
	    }

}
int main(void)
{

    uint32_t mCount;			//延时的次数
    uint16_t mcu_temp_AD;	    //芯片温度AD值
    float mcu_temp;				//芯片温度回归值
    uint16_t temp_AD;		    //外接热敏电阻(温度)AD值
	float temp;
    DISABLE_INTERRUPTS;
    
    temp = 0;
    mCount=0;//记次数
    
    gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_OFF);	//初始化蓝灯
    gpio_init(LIGHT_GREEN,GPIO_OUTPUT,LIGHT_OFF);	//初始化绿灯
    gpio_init(LIGHT_RED,GPIO_OUTPUT,LIGHT_OFF);	//初始化红灯
    adc_init(AD_MCU_TEMP,0);					   //初始化AD芯片温度模块
    mcu_temp_AD = adc_read(AD_MCU_TEMP);       //读取mcu温度通道AD值
    temp_AD=adc_read(AD_TEMP);
    
    uart_init(UART_User,115200);     //初始化串口模块   

    uart_enable_re_int(UART_User);

    ENABLE_INTERRUPTS;
    
	int currentColor = 0;
	int error = 0;
	for (;;) {
	    Delay_ms(500);
	    
	    mcu_temp_AD = adc_read(AD_MCU_TEMP);
	    mcu_temp = adc_mcu_temp(mcu_temp_AD); // 将芯片温度AD值转换成实际温度
	    printf("{%.2lf,", mcu_temp);
	    
	    temp_AD = adc_ave(AD_TEMP, 8); // 获取外部热敏电阻（温度）AD值    
	    temp = tempRegression(temp_AD); // 将外部热敏电阻（温度）AD值转换成实际的温度
	    printf("%.2lf,", temp);
	   	if(temp >= 35 || mcu_temp >=35)error = 1;
	   	else error = 0;
	    
	    if(ch == '0'){
			gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_OFF);	//初始化蓝灯
		    gpio_init(LIGHT_GREEN,GPIO_OUTPUT,LIGHT_OFF);	//初始化绿灯
		    gpio_init(LIGHT_RED,GPIO_OUTPUT,LIGHT_OFF);	//初始化红灯
		    currentColor = -1;
		    mCount = 0;
		}else{
	    	mCount++;
	    	set_Color(&mCount,&currentColor);
	    }
	    
	    printf("%d,%d}", currentColor,error);
	}

}   



void Delay_ms(uint16_t u16ms)
{
    for(volatile uint32_t i = 0; i < 8000*u16ms; i++) __asm("NOP");
}






