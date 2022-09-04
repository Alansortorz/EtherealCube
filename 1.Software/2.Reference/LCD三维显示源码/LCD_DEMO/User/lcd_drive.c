/*******************************************************************************
* 文档: lcd_drive.c
* 作者: Wu LianWei
* 描述: 1.LCD硬件配置及数据写入
*       2.驱动芯片:ST7565R
*       3.I/O口模拟“串行数据写入”
*       4.有1K字节的显示buffer，只负责把buffer数据全部写入LCD，刷新屏幕
*******************************************************************************/
#include "STC15F2K60S2.h"
#include "public.h"
#include "delay.h"





/******************************************************************************/
/* 变量定义                                                                   */
/******************************************************************************/

/**********接口定义**********/
sbit SCL = P0^0;
sbit SDA = P0^1;
sbit RST = P0^3;
sbit CS  = P0^2; //Chip Select,(0:choose)
sbit RS  = P0^4; //Register Select,(0:CMD,1:DAT)

/**********缓存定义**********/
uchar xdata DisBuf[8][128];  //1024字节的显示缓存







/******************************************************************************/
/* 数据写入                                                                   */
/******************************************************************************/

/***************************************
函数: LcdWriteByte
功能: 向lcd驱动芯片写入一个"字节"
说明: "字节"可以是CMD或DAT
***************************************/
void LcdWriteByte(bit cd,uchar byte)
{
    uchar data i;
    RS = cd;
    for(i=0;i<8;i++)
    {
        SCL=0;
        if(byte&0x80)SDA=1;
        else         SDA=0;
        SCL=1;
        byte<<=1;
    }
}

/***************************************
宏名: LcdWriteCmd, 写入"指令"
宏名: LcdWriteDat, 写入"数据"
说明: 带参数的宏定义
***************************************/
#define DAT 1
#define CMD 0
#define LcdWriteCmd(byte)  LcdWriteByte(CMD,byte)
#define LcdWriteDat(byte)  LcdWriteByte(DAT,byte)









/******************************************************************************/
/* 模块操作                                                                   */
/******************************************************************************/

/***************************************
函数: LcdInit
功能: LCD上电初始化
***************************************/
void LcdInit(void)
{
    CS=0;
    RST=0;
    DelayMs(4);
    RST=1;
    DelayMs(6);
    //----------------
    LcdWriteCmd(0x2c);
    DelayMs(1);
    LcdWriteCmd(0x2e);
    DelayMs(1);
    LcdWriteCmd(0x2f);
    DelayMs(1);
    LcdWriteCmd(0x23);  /*粗调对比度，可设置范围 0x20～0x27*/ 
    LcdWriteCmd(0x81);  /*微调对比度*/ 
    LcdWriteCmd(0x28);  /*0x1a,微调对比度的值，可设置范围 0x00～0x3f*/ 
    LcdWriteCmd(0xa2);  /*1/9 偏压比（bias）*/ 
    LcdWriteCmd(0xc8);  /*行扫描顺序：从上到下*/ 
    LcdWriteCmd(0xa0);  /*列扫描顺序：从左到右*/ 
    LcdWriteCmd(0x60);  /*起始行：第一行开始*/ 
    LcdWriteCmd(0xaf);  /*开显示*/ 
    CS=1;
}

/***************************************
函数: LcdWriteAddr
功能: 写入地址
说明: page代表Y方向,column代表X方向
***************************************/
void LcdWriteAddr(uchar page,uchar column)
{
    column+=1;
    LcdWriteCmd(0xb0+page);        //页地址
    LcdWriteCmd(column&0x0f);      //列地址低4位
    LcdWriteCmd((column>>4)+0x10); //列地址高4位
}

/***************************************
函数: ClearScreen
功能: 全屏清屏
说明: 向LCD的所有RAM地址写入0x00
***************************************/
void ClearScreen(void) 
{ 
    uchar i,j;
    CS=0;
    for(i=0;i<9;i++)
    { 
        LcdWriteAddr(i,0); 
        for(j=0;j<132;j++) LcdWriteDat(0x00);
    }
    CS=1;
}

/***************************************
函数: RefreshScreen
功能: 更新屏幕"显示数据"
说明: 把缓存DisBuf数据写入LCD
***************************************/
void RefreshScreen(void)
{
    uchar i,j;
    //------------
    P0M1 &= 0xc0;
    P0M0 &= 0xc0; //准双向口
    //------------
    CS=0;
    for(i=0;i<8;i++)
    { 
        LcdWriteAddr(i,0); 
        for(j=0;j<128;j++) LcdWriteDat(DisBuf[i][j]);
    }
    CS=1;
}

