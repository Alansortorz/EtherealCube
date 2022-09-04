/**
 * @brief ���ƴ�����HC05����ͨѶʵ��
 * @note HC05��STM32��������ͨѶ��ʹ�ô��ڽ����ݷ��ص����ԵĴ���������ʾ
 * 32����HC05���յ������ݴ����һ����̬�Ļ��������б���
 **/
#include "usart_blt.h"
#include <stdarg.h>
#include "bsp_usart.h"

/**
 * @brief �����ն˾�̬�������鶨��
 **/
#define	UART_BUFF_SIZE		1024
uint8_t	uart_buff[UART_BUFF_SIZE];
volatile uint16_t	uart_p = 0;

/**
 * @brief �����жϷ�����NVIC����
 **/
static void NVIC_InitConfig(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			 //���ȼ���
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 //����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�����Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief ���ڳ�ʼ������
 **/
void Usart_InitConfig(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//��������˿�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);		//�������贮��
	
	//���ô�������
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
	
	//�����ж����ȼ�
	NVIC_InitConfig();
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}


/**
 * @brief ��������uart_buff��set buff zero
 **/
void buff_clear(void){
	uint16_t i = UART_BUFF_SIZE + 1;
	uart_p = 0;
	while(i)
		uart_buff[--i] = 0;
}

/**
 * @brief �жϽ��ջ���
 * @note ���ô����жϷ����������ݴ洢��һ����̬�Ļ�������
 **/
void Uart_Process(void){
	if(uart_p < UART_BUFF_SIZE){
		//��Ҫ�ȼ�鴮���ж��Ƿ��������������ж�ʱ�����յ�������д������������
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
			uart_buff[uart_p] = USART_ReceiveData(USART2);
			uart_p++;
		}
	}//����������֮�����������
	else{
		//clear usart interupt pending bit
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//���������
		buff_clear();
	}
}

/**
 * @brief ���ڷ���һ���ַ�
 **/
static void Usart_SendByte(USART_TypeDef *pUsart, uint8_t ch){
	USART_SendData(pUsart, ch);
	while(USART_GetFlagStatus(pUsart, USART_FLAG_TXE) == RESET);
}

/**
 * @brief ���ڷ����ַ���
 * @note �����ַ������Ƿ��Ͷ���ַ�
 **/
void Usart_SendString(USART_TypeDef *pUsart, uint8_t *str){
	unsigned int k = 0;
	do{
		Usart_SendByte(pUsart, *(str+k));
		k++;
	}while(*(str+k) != '\0');
}

/**
 * @brief ��ȡ���������ݺͳ���
 **/
char *get_rebuff(uint16_t *len){
	*len = uart_p;
	return (char *)&uart_buff;
}

