#ifndef __HCSR04_H
#define __HCSR04_H

#include "tim.h"
//����ʹ�õĶ�ʱ���Ͳ���ͨ��
#define HCSR04_TIM 		 		  TIM3
#define HCSR04_TIM_HAL 			htim3
#define HCSR04_TIM_CHANNEL  TIM_CHANNEL_1

//����һ��״̬ö�ٳ���
typedef enum Run_State
{
	TRIG_WAIT = 0,
	RISING,
	FALLING,
	OVER
}Run_State;
//�����ṹ�巽�㴫��ʹ������
typedef struct 
{
	Run_State STATE;
	int buf[2];
	uint8_t len;
}HCSR04;
//����һ�»ص�����
extern void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void car_distance(void);
void key_open_light(void);
#endif
