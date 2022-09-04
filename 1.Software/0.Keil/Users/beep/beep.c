#include "beep.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void BEEP_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE);
	
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);	

		//GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
}

void BEEP_Key(void){
	BEEP_ON;
	Delay_ms(250);
	BEEP_OFF;
}

void BEEP_On(void){
	BEEP_ON;
	Delay_ms(150);
	BEEP_OFF;
}
/*********************************************END OF FILE**********************/
