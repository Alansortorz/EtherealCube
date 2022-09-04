#ifndef __HC05_H
#define __HC05_H

#include "stm32f10x.h"
#include "bsp_systick.h"
#include "usart_blt.h"
#include "bsp_usart.h"
/*
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平

#define BLT_KEY_HIGHT1		   digitalHi(GPIOA,LED1_GPIO_PIN)
#define BLT_KEY_LOW1		   digitalLo(GPIOA,LED1_GPIO_PIN)
*/
#define BLT_INT_GPIO_PORT    	GPIOA
#define BLT_INT_GPIO_PIN		GPIO_Pin_7
#define BLT_KEY_GPIO_PORT    	GPIOA
#define BLT_KEY_GPIO_PIN		GPIO_Pin_5

#define BLT_KEY_HIGHT  		GPIO_SetBits(BLT_KEY_GPIO_PORT,   BLT_KEY_GPIO_PIN);
#define BLT_KEY_LOW  		GPIO_ResetBits(BLT_KEY_GPIO_PORT, BLT_KEY_GPIO_PIN);
#define IS_HC05_CONNECTED() GPIO_ReadInputDataBit(BLT_INT_GPIO_PORT,BLT_INT_GPIO_PIN)

#define HC05_DEBUG_ON			0


#define HC05_INFO(fmt,arg...)           printf("<<-HC05-INFO->> "fmt"\n",##arg)
#define HC05_ERROR(fmt,arg...)          printf("<<-HC05-ERROR->> "fmt"\n",##arg)
#define HC05_DEBUG(fmt,arg...)          do{\
                                          if(HC05_DEBUG_ON)\
                                          printf("<<-HC05-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define HC05_DEBUG_FUNC()               do{\
                                         if(HC05_DEBUG_FUNC_ON)\
                                         printf("<<-HC05-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)

/**
 * @brief 定义一个结构体保存扫描到远程蓝牙设备的数量、地址和名称
 **/
typedef struct{
	uint16_t NAP;
	uint8_t  UAP;
	uint32_t LAP;
}BLTAddr;

#define BLTDEV_MAX_NUM 	5

typedef struct{
	uint8_t num;							//设备数量
	BLTAddr addr[BLTDEV_MAX_NUM];			//设备物理地址，数字形式
	char unpraseAddr[BLTDEV_MAX_NUM][50];	//设备物理地址，字符串形式
	char name[BLTDEV_MAX_NUM][50];			//设备名称
}BLTDev;

void HC05_GPIO_Config(void);
uint8_t HC05_Send_CMD(char *, uint8_t);
void HC05_SendString(char *);
uint8_t HC05_Init(void);
int get_line(char* line, char* stream ,int max_size);

#endif 
