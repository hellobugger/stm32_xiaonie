/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
#include "gpio.h"
#include "oled.h"
#include "SGP30.h"
#include "MPU6050.h"
#include "hcsr04.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern HCSR04 hcsr04;
int16_t AX, AY, AZ, GX, GY, GZ;
uint16_t TVOC = 0, CO2 = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId myTask01Handle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osThreadId myTask05Handle;
osThreadId myTask06Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);
void StartTask02(void const *argument);
void StartTask03(void const *argument);
void StartTask04(void const *argument);
void StartTask05(void const *argument);
void StartTask06(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of myTask01 */
    osThreadDef(myTask01, StartDefaultTask, osPriorityNormal, 0, 128);
    myTask01Handle = osThreadCreate(osThread(myTask01), NULL);

    /* definition and creation of myTask02 */
    osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 32);
    myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

    /* definition and creation of myTask03 */
    osThreadDef(myTask03, StartTask03, osPriorityIdle, 0, 32);
    myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

    /* definition and creation of myTask04 */
    osThreadDef(myTask04, StartTask04, osPriorityIdle, 0, 64);
    myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

    /* definition and creation of myTask05 */
    osThreadDef(myTask05, StartTask05, osPriorityIdle, 0, 128);
    myTask05Handle = osThreadCreate(osThread(myTask05), NULL);

    /* definition and creation of myTask06 */
    osThreadDef(myTask06, StartTask06, osPriorityIdle, 0, 128);
    myTask06Handle = osThreadCreate(osThread(myTask06), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    MX_GPIO_Init();
    MPU6050_Init();
    /* Infinite loop */
    while (1)
    {
        osDelay(100);
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        printf(" AX:%5d AY:%5d AZ:%5d GX:%5d GY:%5d GZ:%5d\r\n", AX, AY, AZ, GX, GY, GZ);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the myTask02 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTask02(void const *argument)
{
    /* USER CODE BEGIN StartTask02 */
    /* Infinite loop */
    // uint16_t pwmVal = 0; // PWM占空比
    
    while (1)
    {
        //	  while (pwmVal< 10)
        //	  {
        //		  pwmVal++;
        //   __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 100);    //修改比较值，修改占空比
        //		  osDelay(10);
        //	  }
        //	  while (pwmVal)
        //	  {
        //		  pwmVal--;
        //		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, pwmVal);    //修改比较值，修改占空比
        //		  osDelay(10);
        //	  }
    }
    /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the myTask03 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void StartTask03(void const *argument)
{
    /* USER CODE BEGIN StartTask03 */
    /* Infinite loop */
    while (1)
    {
        HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_SET);
        osDelay(2000);
        HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_RESET);
        osDelay(500);
    }
    /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
 * @brief Function implementing the myTask04 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask04 */
void StartTask04(void const *argument)
{
    /* USER CODE BEGIN StartTask04 */
    /* Infinite loop */
    while (1)
    {
        
    }
    /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
 * @brief Function implementing the myTask05 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask05 */
void StartTask05(void const *argument)
{
    /* USER CODE BEGIN StartTask05 */

    /* Infinite loop */
    MX_GPIO_Init();
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    while (1)
    {
        key_open_light();
        car_distance();
    }
    /* USER CODE END StartTask05 */
}

/* USER CODE BEGIN Header_StartTask06 */
/**
 * @brief Function implementing the myTask06 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask06 */
void StartTask06(void const *argument)
{
    /* USER CODE BEGIN StartTask06 */
    while (sgp30_init() < 0)
    {
        osDelay(1000);
    }
    do
    {
        if (sgp30_read(&CO2, &TVOC) < 0)
        {
        }
        else
        {
        }
        osDelay(500);
    } while (TVOC == 0 && CO2 == 400);
    OLED_ShowString(0, 0, "CO2:", 16);
    OLED_ShowString(0, 4, "TVOC:", 16);
    /* Infinite loop */
    while (1)
    {
        sgp30_read(&CO2, &TVOC);
        if(CO2>350&&CO2<450)
        {
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        }
        else if(CO2>450)
        {
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        }
        else if(TVOC>350&&TVOC<1000)
        {
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        }
        else if(TVOC>1000)
        {
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        }
        OLED_ShowNum(32, 0, CO2, 4, 16);
        OLED_ShowNum(48, 4, TVOC, 3, 16);
        osDelay(500);
    }
    /* USER CODE END StartTask06 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
