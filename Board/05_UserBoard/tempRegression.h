///=====================================================================
//�ļ����ƣ�emuart.h�ļ�
//������λ��SD-Arm(sumcu.suda.edu.cn)
//���¼�¼��20200831-20200904
//��ֲ���򣺡��̶���
///=====================================================================

#ifndef __ADC_TEMP_REGRESSION_H
#define __ADC_TEMP_REGRESSION_H

#include "string.h"
#include "uart.h"
#include "flash.h"
#include "mcu.h"
#include "adc.h"
#include "user.h"

//=======================����ʹ�ò��迪ʼ================================
//��1�����������е�emuart_recv����������Ӧ�Ĵ����ж��У������淵��ֵ��ȫ�ֱ����С�
//��2�����������е���emuart_init������ɳ�ʼ����
//��3�����з���������Ҫʱ��������Ҫ����emuart_send1��emuart_sendN
//          ��emuart_send_string�����������ݡ�
//��4�����������У�ͨ���ж�emuart_recv�ķ���ֵ�Ƿ�Ϊ���ж��Ƿ���յ����ݲ�������
//=======================����ʹ�ò������================================


//============================================================================
//�������ƣ�tempRegression
//���ܸ�Ҫ�����������ⲿ��������ADֵת��Ϊʵ���¶�
//����˵����tempAD��ͨ��adc_ave�����õ���ADֵ
//�������أ�ʵ���¶�ֵ
//============================================================================
float tempRegression(uint16_t tempAD);


#endif