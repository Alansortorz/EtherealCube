#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"
#include "bsp_systick.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define BEEP_GPIO_PORT    	GPIOC		             		/* GPIO�˿� */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN				GPIO_Pin_1			        
        



/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define BEEP(a)	if (a)	\
					GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);\
					else		\
					GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)



/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define BEEP_TOGGLE		 	digitalToggle(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_ON		   		digitalHi(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_OFF			  digitalLo(BEEP_GPIO_PORT, BEEP_GPIO_PIN)


void BEEP_GPIO_Config(void);
void BEEP_Key(void);
void BEEP_On(void);


#endif /* __BEEP_H */
