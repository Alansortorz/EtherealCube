#include "init.h"



void Init_Config(void){
	Systick_InitConfig();//定时器初始化
	USART1_Config();
	//printf("Usart init successful");
	LED_GPIO_Config();
	Key_GPIO_Config();
	TIM3_Init(4999, 7199);//10Khz的计数频率，计数到5000为500ms  
	//Key_GPIO_Config();
	BEEP_GPIO_Config();
	i2c_GPIO_Config();
	GPIO_OLED_InitConfig();
	OLED_Clear();
	//OLED_DrawBMP(0,0,126,8,BMP);
	MPU6050_Init();
	BEEP_On();
	Delay_ms(5000);
}



