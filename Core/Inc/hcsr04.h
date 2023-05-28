#ifndef __HCSR04_H
#define __HCSR04_H

#include "tim.h"
//配置使用的定时器和捕获通道
#define HCSR04_TIM 		 		  TIM3
#define HCSR04_TIM_HAL 			htim3
#define HCSR04_TIM_CHANNEL  TIM_CHANNEL_1

//声明一个状态枚举常量
typedef enum Run_State
{
	TRIG_WAIT = 0,
	RISING,
	FALLING,
	OVER
}Run_State;
//声明结构体方便传递使用数据
typedef struct 
{
	Run_State STATE;
	int buf[2];
	uint8_t len;
}HCSR04;
//声明一下回调函数
extern void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void car_distance(void);
void key_open_light(void);
#endif
