#ifndef __USART_BLT_H
#define __USART_BLT_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

static void NVIC_InitConfig(void);
void Usart_InitConfig(void);
void buff_clear(void);
void Uart_Process(void);
static void Usart_SendByte(USART_TypeDef *, uint8_t);
void Usart_SendString(USART_TypeDef *, uint8_t *);
char *get_rebuff(uint16_t *);

#endif
