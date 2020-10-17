#ifndef __USERAPP__H
#define __USERAPP__H

#include "main.h"

/* debounce cnt */
#define TIME_SW_PD         3000  //软关机时间为3000ms
#define TIME_HW_PD         1200  //硬件开机时间为1200ms
#define TIMER_HW_PD_FORCE_TRIGGER  1000  //硬件强制关机预通知时间为1000ms
#define TIMER_HW_PD_FORCE 5000  //硬件强制关机时间为3000ms


typedef enum
{
  POWER_DISABLE = 0U,
  POWER_ENABLE
} POWER_State;









void HAL_Delay(uint32_t Delay);//延时函数，单位：ms
uint32_t HAL_GetTick(void);//获取系统时间，单位：ms

void PowerControl(POWER_State status);//控制电源通断
void PowerManager(void);//电源管理函数
void PowerManagerInit(void);//电源管理初始化函数
#endif

