#ifndef __USERAPP__H
#define __USERAPP__H

#include "main.h"

/* debounce cnt */
#define TIME_SW_PD         3000  //��ػ�ʱ��Ϊ3000ms
#define TIME_HW_PD         1200  //Ӳ������ʱ��Ϊ1200ms
#define TIMER_HW_PD_FORCE_TRIGGER  1000  //Ӳ��ǿ�ƹػ�Ԥ֪ͨʱ��Ϊ1000ms
#define TIMER_HW_PD_FORCE 5000  //Ӳ��ǿ�ƹػ�ʱ��Ϊ3000ms


typedef enum
{
  POWER_DISABLE = 0U,
  POWER_ENABLE
} POWER_State;









void HAL_Delay(uint32_t Delay);//��ʱ��������λ��ms
uint32_t HAL_GetTick(void);//��ȡϵͳʱ�䣬��λ��ms

void PowerControl(POWER_State status);//���Ƶ�Դͨ��
void PowerManager(void);//��Դ������
void PowerManagerInit(void);//��Դ�����ʼ������
#endif

