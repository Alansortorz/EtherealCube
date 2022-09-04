/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <gpio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
#include "kalman.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
osThreadId OLEDTaskHandle;
osThreadId IMUTaskHandle;
osThreadId BTTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartOLEDTask(void const * argument);
void StartIMUTask(void const * argument);
void StartBTTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
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
void MX_FREERTOS_Init(void) {
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
  /* definition and creation of OLEDTask */
  osThreadDef(OLEDTask, StartOLEDTask, osPriorityNormal, 0, 128);
  OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);

  /* definition and creation of IMUTask */
  osThreadDef(IMUTask, StartIMUTask, osPriorityIdle, 0, 128);
  IMUTaskHandle = osThreadCreate(osThread(IMUTask), NULL);

  /* definition and creation of BTTask */
  osThreadDef(BTTask, StartBTTask, osPriorityIdle, 0, 128);
  BTTaskHandle = osThreadCreate(osThread(BTTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartOLEDTask */
/**
  * @brief  Function implementing the OLEDTask thread.
  * @param  argument: Not used
  * @retval None
  */
#define PI  3.141

float pitch, roll, yaw;
float pitch_buf, roll_buf, yaw_buf;
float cube[8][3]={{-20,-20,-20},{-20,20,-20},{20,20,-20},{20,-20,-20},{-20,-20,20},{-20,20,20},{20,20,20},{20,-20,20}};//各点坐标
int lineid[]={1,2,2,3,3,4,4,1,5,6,6,7,7,8,8,5,8,4,7,3,6,2,5,1,1,3,2,4};//连接顺序

float *matconv(float *a, float b[3][3]){
    float res[3];
    int i;
    for(i=0; i<3; ++i)
        res[i] = b[i][0] * a[0] + b[i][1] * a[1] + b[i][2] * a[2];
    for(i=0; i<3; ++i)
        a[i] = res[i];
    return a;
}

void rotate(float* obj, float x, float y, float z){
    x /= PI; y /= PI; z /= PI;
    float rz[3][3] = {{cos(z),-sin(z),0}, {sin(z),cos(z),0}, {0,0,1}};
    float ry[3][3] = {{1,0,0}, {0,cos(y),-sin(y)}, {0,sin(y),cos(y)}};
    float rx[3][3] = {{cos(x),0,sin(x)}, {0,1,0}, {-sin(x),0,cos(x)}};
    matconv(matconv(matconv(obj,rz), ry), rx);
}

static uint8_t flag = 0;

/* USER CODE END Header_StartOLEDTask */
void StartOLEDTask(void const * argument)
{
  /* USER CODE BEGIN StartOLEDTask */
    uint8_t x = 0, i;
    Before_State_Update(pitch);
    Before_State_Update(roll);
    Before_State_Update(yaw);
    OLED_Clear();
    /* Infinite loop */
    for(;;){//简单的按键切换
        if(KEY_ON == Key1_Scan()){
            OLED_Clear();
            for(x=0; x<128; x=(x+1)%128){
                //OLED_ShowString(0, 0, (char*)"roll");
                OLED_DrawWave(x, (int8_t)roll/3);
                osDelay(10);
                if(KEY_ON == Key1_Scan()){
                    OLED_Clear();
                    for(x=0; x<128; x=(x+1)%128){
                        //OLED_ShowString(0, 0, (char*)"pitch");
                        OLED_DrawWave(x, (int8_t)pitch/3);
                        osDelay(10);
                        if(KEY_ON == Key1_Scan()){
                            OLED_Clear();
                            for(x=0; x<128; x=(x+1)%128){
                                //OLED_ShowString(0, 0, (char*)"yaw");
                                OLED_DrawWave(x, (int8_t)yaw/3);
                                if(KEY_ON == Key2_Scan())
                                    break;
                            }
                        }
                        if(KEY_ON == Key2_Scan())
                            break;
                    }
                }
                if(KEY_ON == Key2_Scan()){
                    break;
                }
                osDelay(10);
            }
        }
        OLED_Clear();
        for(i=0; i<8; i++)
            rotate(cube[i], pitch/360, roll/360, yaw/360);
        for(i=0; i<28; i+=2)
            OLED_DrawLine(64+cube[lineid[i]-1][0], 32+cube[lineid[i]-1][1], 64+cube[lineid[i+1]-1][0], 32+cube[lineid[i+1]-1][1], 1);
        OLED_Refresh();

        osDelay(10);
  }
  /* USER CODE END StartOLEDTask */
}

/* USER CODE BEGIN Header_StartIMUTask */
/**
* @brief Function implementing the IMUTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIMUTask */
void StartIMUTask(void const * argument)
{
  /* USER CODE BEGIN StartIMUTask */
  /* Infinite loop */
  for(;;)
  {
      mpu_dmp_get_data(&pitch, &roll, &yaw);
      //Angle_Calcu();
      //pitch = Angle_X_Final;
      //roll = Angle_Y_Final;
      //yaw = 0;
      osDelay(2);
  }
  /* USER CODE END StartIMUTask */
}

/* USER CODE BEGIN Header_StartBTTask */
/**
* @brief Function implementing the BTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBTTask */
void StartBTTask(void const * argument)
{
  /* USER CODE BEGIN StartBTTask */

  /* Infinite loop */
  for(;;)
  {//蓝牙暂时没加
      if(KEY_ON == Key1_Scan())
          HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      osDelay(10);
      if(KEY_ON == Key2_Scan())
          HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
      osDelay(10);
  }
  /* USER CODE END StartBTTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
