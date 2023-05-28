#include "hcsr04.h"
#include "gpio.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>

uint8_t key_flag = 0;
uint8_t light_flag = 0;
uint8_t distance_flag = 0;
// 声明一个模块的数据结构体
HCSR04 hcsr04;
// 重新编写输入捕获回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (HCSR04_TIM == htim->Instance)
    {
        // 如果上升沿检测
        if (hcsr04.STATE == RISING)
        {
            // 设置定时器CNT为0
            __HAL_TIM_SET_COUNTER(&HCSR04_TIM_HAL, 0);
            // 读取上升沿时的CNT值到buf
            hcsr04.buf[0] = __HAL_TIM_GetCounter(&HCSR04_TIM_HAL);
            // 设置下一个捕获为下降沿
            __HAL_TIM_SET_CAPTUREPOLARITY(&HCSR04_TIM_HAL, HCSR04_TIM_CHANNEL, TIM_ICPOLARITY_FALLING);
            // 改变运行模式
            hcsr04.STATE = FALLING;
        }
        else if (hcsr04.STATE == FALLING) // 如果下降沿检测
        {
            // 获取当前的CNT到buf2，这样高电平维持的时间长度就记录了
            hcsr04.buf[1] = __HAL_TIM_GetCounter(&HCSR04_TIM_HAL);
            // 将运行标志设置为完成
            hcsr04.STATE = OVER;
        }
    }
}

void key_open_light(void)
{
    if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
    {
        osDelay(100);
        if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
        {
            key_flag++;
            if (key_flag == 3)
            {
                key_flag = 0;
            }
        }
    }
    switch (key_flag)
    {
    case 0:
    {
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 0); // 关灯
        light_flag = 0;
    }
    break;
    case 1:
    {
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 10); // 打开近光灯
        light_flag = 1;
    }
    break;
    case 2:
    {
        if(!distance_flag)
        {
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 100); // 打开远光灯
        }
        
        light_flag = 2;
    }
    break;
    }
}
void car_distance(void)
{
    if (hcsr04.STATE == TRIG_WAIT)
    {
        // 给HCSR04一个1ms触发电平
        HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
        osDelay(1);
        HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
        // 启动上升沿捕获
        __HAL_TIM_SET_CAPTUREPOLARITY(&HCSR04_TIM_HAL, HCSR04_TIM_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
        // 启动输入捕获
        HAL_TIM_IC_Start_IT(&HCSR04_TIM_HAL, HCSR04_TIM_CHANNEL);
        // 设置上升沿捕获
        hcsr04.STATE = RISING;
    }
    if (hcsr04.STATE == OVER) // 判断电平捕获结束
    {
        // 计算高电平时间差值代表时间维持的长度，因为定时器设置1us自加一次，所以时长单位为us
        // 乘以0.017的是因为声波速度为340m/s，而1s=1000000us，且技术一次距离是一个来回
        // 要除以2，此处单位为cm
        hcsr04.len = (float)(hcsr04.buf[1] - hcsr04.buf[0]) * 0.017;
        // 刷新状态，使可以进行下一次触发电平
        hcsr04.STATE = TRIG_WAIT;
        if (hcsr04.len > 20) // 距离大于20cm
        {
            distance_flag = 1;
        }
        else if (hcsr04.len > 15 && hcsr04.len < 20) // 距离在15-20以内
        {
            distance_flag = 2;
        }
        else if (hcsr04.len > 10 && hcsr04.len < 15) // 距离在10-15以内
        {
            distance_flag = 3;
        }
        else if (hcsr04.len > 5 && hcsr04.len < 10) // 距离在5-10以内
        {
            distance_flag = 4;
        }
        else if (hcsr04.len < 5) // 距离小于5
        {
            distance_flag = 5;
        }
    }
    if (light_flag == 2) // 开启了远光灯
    {
        switch (distance_flag)
        {
        case 1:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 100); // 打开远光灯
        }
        break;
        case 2:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 80); // 打开远光灯
        }
        break;
        case 3:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 60); // 打开远光灯
        }
        break;
        case 4:
        {
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 40); // 打开远光灯
        }
        break;
        case 5:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_SET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 20); // 打开远光灯

        }
        break;
        default:
            break;
        }
    }
}
