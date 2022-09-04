/**
 * @brief 控制串口与HC05进行通讯实验
 * @note HC05与STM32进行数据通讯，使用串口将数据返回到电脑的串口助手显示
 * 32将从HC05接收到的数据存放在一个静态的缓冲数组中保存
 **/
#include "usart_blt.h"
#include <stdarg.h>
#include "bsp_usart.h"

/**
 * @brief 串口终端静态缓冲数组定义
 **/
#define	UART_BUFF_SIZE		1024
uint8_t	uart_buff[UART_BUFF_SIZE];
volatile uint16_t	uart_p = 0;

/**
 * @brief 串口中断服务函数NVIC配置
 **/
static void NVIC_InitConfig(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			 //优先级组
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 //串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 串口初始化函数
 **/
void Usart_InitConfig(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启外设端口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);		//开启外设串口
	
	//配置串口引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	//配置中断优先级
	NVIC_InitConfig();
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}


/**
 * @brief 清理缓冲区uart_buff，set buff zero
 **/
void buff_clear(void){
	uint16_t i = UART_BUFF_SIZE + 1;
	uart_p = 0;
	while(i)
		uart_buff[--i] = 0;
}

/**
 * @brief 中断接收缓存
 * @note 利用串口中断服务函数将数据存储在一个静态的缓冲区中
 **/
void Uart_Process(void){
	if(uart_p < UART_BUFF_SIZE){
		//需要先检查串口中断是否发生，发生串口中断时将接收到的数据写到缓冲数组中
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
			uart_buff[uart_p] = USART_ReceiveData(USART2);
			uart_p++;
		}
	}//缓冲区存满之后清除缓冲区
	else{
		//clear usart interupt pending bit
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//缓冲区清除
		buff_clear();
	}
}

/**
 * @brief 串口发送一个字符
 **/
static void Usart_SendByte(USART_TypeDef *pUsart, uint8_t ch){
	USART_SendData(pUsart, ch);
	while(USART_GetFlagStatus(pUsart, USART_FLAG_TXE) == RESET);
}

/**
 * @brief 串口发送字符串
 * @note 发送字符串就是发送多个字符
 **/
void Usart_SendString(USART_TypeDef *pUsart, uint8_t *str){
	unsigned int k = 0;
	do{
		Usart_SendByte(pUsart, *(str+k));
		k++;
	}while(*(str+k) != '\0');
}

/**
 * @brief 获取缓冲区数据和长度
 **/
char *get_rebuff(uint16_t *len){
	*len = uart_p;
	return (char *)&uart_buff;
}

