//======================================================================
//文件名称：user.h（user头文件）
//制作单位：SD-Arm(sumcu.suda.edu.cn)
//更新记录：20181201-20200830
//概要说明：（1）包含用到的头文件，为达到应用程序的可移植性，具体硬件要接
//              芯片哪个引脚，需要在这里宏定义，目的是实现对具体硬件对象
//              编程，而不是对芯片的引脚编程
//          （2）中断处理程序名字在此宏定义，以便isr.c可移植
//======================================================================
#ifndef USER_H   //防止重复定义（USER_H 开头）
#define USER_H

//（1）【固定】文件包含
#include "printf.h"
#include "gpio.h"
#include "flash.h"
#include "emuart.h"
#include "adc.h"
#include "gec.h"
#include "tempRegression.h"
//（2）【变动】指示灯端口及引脚定义—根据实际使用的引脚改动
//指示灯端口及引脚定义
#define  LIGHT_RED    (PTB_NUM|7)  //红灯，（GEC_56）
#define  LIGHT_GREEN  (PTB_NUM|8)  //绿灯，（GEC_55）
#define  LIGHT_BLUE   (PTB_NUM|9)  //蓝灯，（GEC_54）
#define  GPIO_TSI      (PTD_NUM|2)  	 //定义GPIO_TSI引脚

//灯状态宏定义（灯亮、灯暗对应的物理电平由硬件接法决定）
#define  LIGHT_ON       0    //灯亮
#define  LIGHT_OFF      1    //灯暗

//（3）【变动】UART可用模块定义
#define UART_Debug   UART_3   //用于程序更新，无法被使用
#define UART_User    UART_2   //用户串口（黑-GND;白-TX;绿-RX）

//（4）【变动】中断服务函数宏定义
#define UART_User_Handler   USART2_IRQHandler  //用户串口中断函数

//（5）ADC模块定义
#define AD_MCU_TEMP   17	//内部温度采集通道17
#define AD_TEMP       15	//热敏温度采集通道15(PTB_NUM|0)

#endif    //防止重复定义（USER_H 结尾）
