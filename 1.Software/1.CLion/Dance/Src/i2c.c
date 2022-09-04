/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
static void i2c_Delay(void)
{
  uint8_t i;

  /*�?
     下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的�?
    CPU主频72MHz时，在内部Flash运行, MDK工程不优�?
    循环次数�?10时，SCL频率 = 205KHz
    循环次数�?7时，SCL频率 = 347KHz�? SCL高电平时�?1.5us，SCL低电平时�?2.87us
     循环次数�?5时，SCL频率 = 421KHz�? SCL高电平时�?1.25us，SCL低电平时�?2.375us

    IAR工程编译效率高，不能设置�?7
  */
  for (i = 0; i < 10; i++);
}

void i2c_Start(void)
{
  /* 当SCL高电平时，SDA出现�?个下跳沿表示I2C总线启动信号 */
  I2C_SDA_1();
  I2C_SCL_1();
  i2c_Delay();
  I2C_SDA_0();
  i2c_Delay();
  I2C_SCL_0();
  i2c_Delay();
}

void i2c_Stop(void)
{
  /* 当SCL高电平时，SDA出现�?个上跳沿表示I2C总线停止信号 */
  I2C_SDA_0();
  I2C_SCL_1();
  i2c_Delay();
  I2C_SDA_1();
}

void i2c_SendByte(uint8_t _ucByte)
{
  uint8_t i;

  /* 先发送字节的高位bit7 */
  for (i = 0; i < 8; i++)
  {
    if (_ucByte & 0x80)
    {
      I2C_SDA_1();
    }
    else
    {
      I2C_SDA_0();
    }
    i2c_Delay();
    I2C_SCL_1();
    i2c_Delay();
    I2C_SCL_0();
    if (i == 7)
    {
      I2C_SDA_1(); // 释放总线
    }
    _ucByte <<= 1;	/* 左移�?个bit */
    i2c_Delay();
  }
}

uint8_t i2c_ReadByte(uint8_t ack)
{
  uint8_t i;
  uint8_t value;

  /* 读到�?1个bit为数据的bit7 */
  value = 0;
  for (i = 0; i < 8; i++)
  {
    value <<= 1;
    I2C_SCL_1();
    i2c_Delay();
    if (I2C_SDA_READ())
    {
      value++;
    }
    I2C_SCL_0();
    i2c_Delay();
  }
  if(ack==0)
    i2c_NAck();
  else
    i2c_Ack();
  return value;
}

uint8_t i2c_WaitAck(void)
{
  uint8_t re;

  I2C_SDA_1();	/* CPU释放SDA总线 */
  i2c_Delay();
  I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
  i2c_Delay();
  if (I2C_SDA_READ())	/* CPU读取SDA口线状�?? */
  {
    re = 1;
  }
  else
  {
    re = 0;
  }
  I2C_SCL_0();
  i2c_Delay();
  return re;
}

void i2c_Ack(void)
{
  I2C_SDA_0();	/* CPU驱动SDA = 0 */
  i2c_Delay();
  I2C_SCL_1();	/* CPU产生1个时�? */
  i2c_Delay();
  I2C_SCL_0();
  i2c_Delay();
  I2C_SDA_1();	/* CPU释放SDA总线 */
}

void i2c_NAck(void)
{
  I2C_SDA_1();	/* CPU驱动SDA = 1 */
  i2c_Delay();
  I2C_SCL_1();	/* CPU产生1个时�? */
  i2c_Delay();
  I2C_SCL_0();
  i2c_Delay();
}

void i2c_GPIO_Config(void){
  GPIO_InitTypeDef GPIO_InitStructure;

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStructure.Pin = I2C_SCL_Pin | I2C_SDA_Pin;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  	/* �?漏输�? */
  HAL_GPIO_Init(GPIO_I2C_Port, &GPIO_InitStructure);
  /* 给一个停止信�?, 复位I2C总线上的�?有设备到待机模式 */
  i2c_Stop();
}

uint8_t i2c_CheckDevice(uint8_t _Address)
{
  uint8_t ucAck;

  i2c_GPIO_Config();		/* 配置GPIO */

  i2c_Start();		/* 发�?�启动信�? */

  /* 发�?�设备地�?+读写控制bit�?0 = w�? 1 = r) bit7 先传 */
  i2c_SendByte(_Address|I2C_WR);
  ucAck = i2c_WaitAck();	/* �?测设备的ACK应答 */

  i2c_Stop();			/* 发�?�停止信�? */

  return ucAck;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
