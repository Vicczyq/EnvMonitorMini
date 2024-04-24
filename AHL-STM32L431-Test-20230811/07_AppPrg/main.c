#define GLOBLE_VAR
#include "includes.h"      //������ͷ�ļ�

uint8_t GPIO_Tsi(uint16_t port_pin);
void Delay_ms(uint16_t u16ms);
float TempRegression(uint16_t tmpAD);  //�����¶�ADֵתΪʵ���¶�
float TempTrans(uint16_t mcu_temp_AD); //mcu�¶�ADֵתΪʵ���¶�

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

    uint32_t mCount;			//��ʱ�Ĵ���
    uint16_t mcu_temp_AD;	    //оƬ�¶�ADֵ
    float mcu_temp;				//оƬ�¶Ȼع�ֵ
    uint16_t temp_AD;		    //�����������(�¶�)ADֵ
	float temp;
    DISABLE_INTERRUPTS;
    
    temp = 0;
    mCount=0;//�Ǵ���
    
    gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_OFF);	//��ʼ������
    gpio_init(LIGHT_GREEN,GPIO_OUTPUT,LIGHT_OFF);	//��ʼ���̵�
    gpio_init(LIGHT_RED,GPIO_OUTPUT,LIGHT_OFF);	//��ʼ�����
    adc_init(AD_MCU_TEMP,0);					   //��ʼ��ADоƬ�¶�ģ��
    mcu_temp_AD = adc_read(AD_MCU_TEMP);       //��ȡmcu�¶�ͨ��ADֵ
    temp_AD=adc_read(AD_TEMP);
    
    uart_init(UART_User,115200);     //��ʼ������ģ��   

    uart_enable_re_int(UART_User);

    ENABLE_INTERRUPTS;
    
	int currentColor = 0;
	int error = 0;
	for (;;) {
	    Delay_ms(500);
	    
	    mcu_temp_AD = adc_read(AD_MCU_TEMP);
	    mcu_temp = adc_mcu_temp(mcu_temp_AD); // ��оƬ�¶�ADֵת����ʵ���¶�
	    printf("{%.2lf,", mcu_temp);
	    
	    temp_AD = adc_ave(AD_TEMP, 8); // ��ȡ�ⲿ�������裨�¶ȣ�ADֵ    
	    temp = tempRegression(temp_AD); // ���ⲿ�������裨�¶ȣ�ADֵת����ʵ�ʵ��¶�
	    printf("%.2lf,", temp);
	   	if(temp >= 35 || mcu_temp >=35)error = 1;
	   	else error = 0;
	    
	    if(ch == '0'){
			gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_OFF);	//��ʼ������
		    gpio_init(LIGHT_GREEN,GPIO_OUTPUT,LIGHT_OFF);	//��ʼ���̵�
		    gpio_init(LIGHT_RED,GPIO_OUTPUT,LIGHT_OFF);	//��ʼ�����
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






