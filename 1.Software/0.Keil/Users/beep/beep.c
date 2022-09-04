#include "beep.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void BEEP_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE);
	
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
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
