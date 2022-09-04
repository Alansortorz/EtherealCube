#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"
#include "bsp_systick.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define BEEP_GPIO_PORT    	GPIOC		             		/* GPIO端口 */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define BEEP_GPIO_PIN				GPIO_Pin_1			        
        



/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 使用标准的固件库控制IO*/
#define BEEP(a)	if (a)	\
					GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);\
					else		\
					GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)



/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define BEEP_TOGGLE		 	digitalToggle(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_ON		   		digitalHi(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_OFF			  digitalLo(BEEP_GPIO_PORT, BEEP_GPIO_PIN)


void BEEP_GPIO_Config(void);
void BEEP_Key(void);
void BEEP_On(void);


#endif /* __BEEP_H */
