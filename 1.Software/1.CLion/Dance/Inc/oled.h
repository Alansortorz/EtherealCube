#ifndef __OLED_H
#define __OLED_H

#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "cmsis_os.h"

#define OLED_MODE         0
#define SIZE 							16
#define XLevelL						0x00
#define XLevelH						0x10
#define Max_Column				128
#define Max_Row						64
#define	Brightness				0xFF
#define X_WIDTH 					128
#define Y_WIDTH 					64

#define OLED_CMD    0
#define OLED_DATA   1

#define OLED_SCL_GPIO_Port  GPIOB
#define OLED_SCL_Pin        GPIO_PIN_13
#define OLED_SDA_GPIO_Port  GPIOB
#define OLED_SDA_Pin        GPIO_PIN_15
#define OLED_RST_GPIO_Port  GPIOB
#define OLED_RST_Pin        GPIO_PIN_14
#define OLED_DC_GPIO_Port   GPIOB
#define OLED_DC_Pin         GPIO_PIN_12

#define OLED_SCL_Clr()      HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET);
#define OLED_SCL_Set()      HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET);
#define OLED_SDA_Clr()      HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET);
#define OLED_SDA_Set()      HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET);
#define OLED_RST_Clr()      HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
#define OLED_RST_Set()      HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
#define OLED_DC_Clr()      HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
#define OLED_DC_Set()      HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);

void OLED_Clear(void);																																											 /* OLED清屏 */
void OLED_Display_On(void);                                                                                  /* OLED开 */
void OLED_Display_Off(void);                                                                                 /* OLED关 */
void OLED_InitConfig(void);                                                                                  /* OLED初始化 */
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	                                                                 /* 写字节 */
void OLED_Set_Pos(unsigned char x, unsigned char y);                                                         /* 设置坐标 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);                                                         /* 显示字符 */
void OLED_ShowString(uint8_t x,uint8_t y, char *p);	                                                     		 /* 显示字符串 */
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);                                                       /* 显示中文 */
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);                                /* 显示数字 */
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);/* 显示图片 */
void OLED_Refresh(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawWave(int8_t x, int8_t y);
void Before_State_Update(uint8_t y);
void Current_State_Update(uint8_t y);

#endif