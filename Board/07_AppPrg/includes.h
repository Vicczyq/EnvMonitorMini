
#ifndef  INCLUDES_H     //防止重复定义（INCLUDES_H 开头）
#define  INCLUDES_H
#include "user.h"
#ifdef GLOBLE_VAR              //GLOBLE_VAR在main.c文件中有宏定义
  #define G_VAR_PREFIX         //前缀G_VAR_PREFIX定义为空
#else                          //GLOBLE_VAR在非main.c文件中无定义
  #define G_VAR_PREFIX extern  //前缀G_VAR_PREFIX定义为"extern"
#endif
uint8_t ch;

G_VAR_PREFIX vuint16_t gcRecvLen;
G_VAR_PREFIX vuint16_t gcRecvDCLen;
G_VAR_PREFIX vuint8_t  gcReccrc32[4];
G_VAR_PREFIX vuint8_t  gcRecvBuf[MCU_SECTORSIZE];

#endif                  //防止重复定义（INCLUDES_H 结尾）


