#ifndef __INIT_H
#define __INIT_H

#include "bsp_usart.h"
#include "bsp_led.h"
#include "mpu6050.h"
#include "bsp_i2c.h"
#include "stm32f10x_it.h"
#include "bsp_key.h"
#include "bsp_spi_nrf.h"
#include "beep.h"
#include "time.h"
#include "stm32f10x.h"
#include "oled.h"
#include "bsp_systick.h"


void Init_Config(void);

#endif
