#include "hcsr04.h"
#include "gpio.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>

uint8_t key_flag = 0;
uint8_t light_flag = 0;
uint8_t distance_flag = 0;
// ����һ��ģ������ݽṹ��
HCSR04 hcsr04;
// ���±�д���벶��ص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (HCSR04_TIM == htim->Instance)
    {
        // ��������ؼ��
        if (hcsr04.STATE == RISING)
        {
            // ���ö�ʱ��CNTΪ0
            __HAL_TIM_SET_COUNTER(&HCSR04_TIM_HAL, 0);
            // ��ȡ������ʱ��CNTֵ��buf
            hcsr04.buf[0] = __HAL_TIM_GetCounter(&HCSR04_TIM_HAL);
            // ������һ������Ϊ�½���
            __HAL_TIM_SET_CAPTUREPOLARITY(&HCSR04_TIM_HAL, HCSR04_TIM_CHANNEL, TIM_ICPOLARITY_FALLING);
            // �ı�����ģʽ
            hcsr04.STATE = FALLING;
        }
        else if (hcsr04.STATE == FALLING) // ����½��ؼ��
        {
            // ��ȡ��ǰ��CNT��buf2�������ߵ�ƽά�ֵ�ʱ�䳤�Ⱦͼ�¼��
            hcsr04.buf[1] = __HAL_TIM_GetCounter(&HCSR04_TIM_HAL);
            // �����б�־����Ϊ���
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
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 0); // �ص�
        light_flag = 0;
    }
    break;
    case 1:
    {
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 10); // �򿪽����
        light_flag = 1;
    }
    break;
    case 2:
    {
        if(!distance_flag)
        {
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 100); // ��Զ���
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
        // ��HCSR04һ��1ms������ƽ
        HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
        osDelay(1);
        HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
        // ���������ز���
        __HAL_TIM_SET_CAPTUREPOLARITY(&HCSR04_TIM_HAL, HCSR04_TIM_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
        // �������벶��
        HAL_TIM_IC_Start_IT(&HCSR04_TIM_HAL, HCSR04_TIM_CHANNEL);
        // ���������ز���
        hcsr04.STATE = RISING;
    }
    if (hcsr04.STATE == OVER) // �жϵ�ƽ�������
    {
        // ����ߵ�ƽʱ���ֵ����ʱ��ά�ֵĳ��ȣ���Ϊ��ʱ������1us�Լ�һ�Σ�����ʱ����λΪus
        // ����0.017������Ϊ�����ٶ�Ϊ340m/s����1s=1000000us���Ҽ���һ�ξ�����һ������
        // Ҫ����2���˴���λΪcm
        hcsr04.len = (float)(hcsr04.buf[1] - hcsr04.buf[0]) * 0.017;
        // ˢ��״̬��ʹ���Խ�����һ�δ�����ƽ
        hcsr04.STATE = TRIG_WAIT;
        if (hcsr04.len > 20) // �������20cm
        {
            distance_flag = 1;
        }
        else if (hcsr04.len > 15 && hcsr04.len < 20) // ������15-20����
        {
            distance_flag = 2;
        }
        else if (hcsr04.len > 10 && hcsr04.len < 15) // ������10-15����
        {
            distance_flag = 3;
        }
        else if (hcsr04.len > 5 && hcsr04.len < 10) // ������5-10����
        {
            distance_flag = 4;
        }
        else if (hcsr04.len < 5) // ����С��5
        {
            distance_flag = 5;
        }
    }
    if (light_flag == 2) // ������Զ���
    {
        switch (distance_flag)
        {
        case 1:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 100); // ��Զ���
        }
        break;
        case 2:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 80); // ��Զ���
        }
        break;
        case 3:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 60); // ��Զ���
        }
        break;
        case 4:
        {
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 40); // ��Զ���
        }
        break;
        case 5:
        {
            HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_SET);
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 20); // ��Զ���

        }
        break;
        default:
            break;
        }
    }
}
