
#ifndef  INCLUDES_H     //��ֹ�ظ����壨INCLUDES_H ��ͷ��
#define  INCLUDES_H
#include "user.h"
#ifdef GLOBLE_VAR              //GLOBLE_VAR��main.c�ļ����к궨��
  #define G_VAR_PREFIX         //ǰ׺G_VAR_PREFIX����Ϊ��
#else                          //GLOBLE_VAR�ڷ�main.c�ļ����޶���
  #define G_VAR_PREFIX extern  //ǰ׺G_VAR_PREFIX����Ϊ"extern"
#endif
uint8_t ch;

G_VAR_PREFIX vuint16_t gcRecvLen;
G_VAR_PREFIX vuint16_t gcRecvDCLen;
G_VAR_PREFIX vuint8_t  gcReccrc32[4];
G_VAR_PREFIX vuint8_t  gcRecvBuf[MCU_SECTORSIZE];

#endif                  //��ֹ�ظ����壨INCLUDES_H ��β��


