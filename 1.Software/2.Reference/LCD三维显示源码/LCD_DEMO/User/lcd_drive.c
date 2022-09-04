/*******************************************************************************
* �ĵ�: lcd_drive.c
* ����: Wu LianWei
* ����: 1.LCDӲ�����ü�����д��
*       2.����оƬ:ST7565R
*       3.I/O��ģ�⡰��������д�롱
*       4.��1K�ֽڵ���ʾbuffer��ֻ�����buffer����ȫ��д��LCD��ˢ����Ļ
*******************************************************************************/
#include "STC15F2K60S2.h"
#include "public.h"
#include "delay.h"





/******************************************************************************/
/* ��������                                                                   */
/******************************************************************************/

/**********�ӿڶ���**********/
sbit SCL = P0^0;
sbit SDA = P0^1;
sbit RST = P0^3;
sbit CS  = P0^2; //Chip Select,(0:choose)
sbit RS  = P0^4; //Register Select,(0:CMD,1:DAT)

/**********���涨��**********/
uchar xdata DisBuf[8][128];  //1024�ֽڵ���ʾ����







/******************************************************************************/
/* ����д��                                                                   */
/******************************************************************************/

/***************************************
����: LcdWriteByte
����: ��lcd����оƬд��һ��"�ֽ�"
˵��: "�ֽ�"������CMD��DAT
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
����: LcdWriteCmd, д��"ָ��"
����: LcdWriteDat, д��"����"
˵��: �������ĺ궨��
***************************************/
#define DAT 1
#define CMD 0
#define LcdWriteCmd(byte)  LcdWriteByte(CMD,byte)
#define LcdWriteDat(byte)  LcdWriteByte(DAT,byte)









/******************************************************************************/
/* ģ�����                                                                   */
/******************************************************************************/

/***************************************
����: LcdInit
����: LCD�ϵ��ʼ��
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
    LcdWriteCmd(0x23);  /*�ֵ��Աȶȣ������÷�Χ 0x20��0x27*/ 
    LcdWriteCmd(0x81);  /*΢���Աȶ�*/ 
    LcdWriteCmd(0x28);  /*0x1a,΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f*/ 
    LcdWriteCmd(0xa2);  /*1/9 ƫѹ�ȣ�bias��*/ 
    LcdWriteCmd(0xc8);  /*��ɨ��˳�򣺴��ϵ���*/ 
    LcdWriteCmd(0xa0);  /*��ɨ��˳�򣺴�����*/ 
    LcdWriteCmd(0x60);  /*��ʼ�У���һ�п�ʼ*/ 
    LcdWriteCmd(0xaf);  /*����ʾ*/ 
    CS=1;
}

/***************************************
����: LcdWriteAddr
����: д���ַ
˵��: page����Y����,column����X����
***************************************/
void LcdWriteAddr(uchar page,uchar column)
{
    column+=1;
    LcdWriteCmd(0xb0+page);        //ҳ��ַ
    LcdWriteCmd(column&0x0f);      //�е�ַ��4λ
    LcdWriteCmd((column>>4)+0x10); //�е�ַ��4λ
}

/***************************************
����: ClearScreen
����: ȫ������
˵��: ��LCD������RAM��ַд��0x00
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
����: RefreshScreen
����: ������Ļ"��ʾ����"
˵��: �ѻ���DisBuf����д��LCD
***************************************/
void RefreshScreen(void)
{
    uchar i,j;
    //------------
    P0M1 &= 0xc0;
    P0M0 &= 0xc0; //׼˫���
    //------------
    CS=0;
    for(i=0;i<8;i++)
    { 
        LcdWriteAddr(i,0); 
        for(j=0;j<128;j++) LcdWriteDat(DisBuf[i][j]);
    }
    CS=1;
}

