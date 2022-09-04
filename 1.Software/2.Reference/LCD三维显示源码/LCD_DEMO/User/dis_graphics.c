/*******************************************************************************
* 文档: dis_graphics.c
* 作者: Wu LianWei
* 描述: 1.建立绘图显示的基础:DisDot
*       2.基本的绘图显示操作
*       3.所有操作都基于显示缓存DisBuf
*******************************************************************************/
#include"public.h"
extern uchar xdata DisBuf[8][128];  //1024字节的显示缓存





/***************************************
函数: DisDot
功能: 在屏幕任意位置显示一个点
说明: 1.显示点位置坐标(Y:0~63,X:0~127)
      2.超出坐标范围,不显示
***************************************/
void DisDot(uchar Y,uchar X)
{
    uchar idata page;
    uchar idata dot;
    //-------------------
    //Y&=0x3f;
    //X&=0x7f; //限制输入坐标范围
    //-------------------
    if(Y>63||X>127)return; //坐标超出范围,则退出
    //-------------------
    page=Y/8;
    dot =Y%8;
    dot =_crol_(0x01,dot);
    //-------------------
    DisBuf[page][X] |= dot;
}





/***************************************
函数: DisDotMatrix
功能: 以X/Y为起点坐标,显示一个边长为NxM的点方阵
说明: 1.起点位置坐标(Y:0~63,X:0~127)
      2.超出坐标范围,不显示
      3.起点坐标位于点方阵的左上角
      4.M和N代表边长,当M或N=0时,则不显示
      5.建立在DisDot概念上
***************************************//*
void DisDotMatrix(uchar Y,uchar X,uchar M,uchar N)
{
    uchar idata i,j;
    uchar idata row;
    uchar idata column;
    uchar idata page;
    uchar idata dot;
    //-------------------
    if(M==0||N==0)return;
    //-------------------
    //Y&=0x3f;
    //X&=0x7f; //限制输入坐标范围
    //-------------------
    if(Y>63||X>127)return; //坐标超出范围,则退出
    //-------------------
    for(i=0;i<M;i++)
    {
        if( (Y+i)>63 )break; //坐标超出范围,则退出
        row=(Y+i);
        //row&=0x3f; //限制输入坐标范围
        //--------
        page=row/8;
        dot =row%8;
        dot =_crol_(0x01,dot);
        //-------------------
        for(j=0;j<N;j++)
        {
            if( (X+j)>127 )break; //坐标超出范围,则退出
            column=(X+j);
            //column&=0x7f; //限制输入坐标范围
            //-----------
            DisBuf[page][column] |= dot;
        }
    }
}

/***************************************
函数: DisRecArea
功能: 显示X1/Y1到X2/Y2为对角线的矩形面
说明: 1.获得矩形左上角的X/Y,和矩形的边长M/N
      2.坐标范围(Y:0~63,X:0~127)
      3.超出部分,不会显示
      4.建立在DisDotMatrix概念上
      5.允许边长为零
***************************************//*
void DisRecArea(uchar Y1,uchar X1,uchar Y2,uchar X2)
{
    uchar idata Y,X;
    uchar idata M,N;
    uchar idata i,j;
    uchar idata row;
    uchar idata column;
    uchar idata page;
    uchar idata dot;
    //-------------------
    //Y1&=0x3f; Y2&=0x3f;
    //X1&=0x7f; X2&=0x7f;
    if(Y1> 63)Y1= 63;
    if(X1>127)X1=127;
    if(Y2> 63)Y2= 63;
    if(X2>127)X2=127;
    //-------------------
    if(Y1<Y2){ Y=Y1; M=Y2-Y1; }
    else     { Y=Y2; M=Y1-Y2; }
    if(X1<X2){ X=X1; N=X2-X1; }
    else     { X=X2; N=X1-X2; }
    //-------------------
    for(i=0;i<=M;i++)
    {
        row=(Y+i);
        //row&=0x3f;
        //--------
        page=row/8;
        dot =row%8;
        dot =_crol_(0x01,dot);
        //-------------------
        for(j=0;j<=N;j++)
        {
            column=(X+j);
            //column&=0x7f;
            //-----------
            DisBuf[page][column] |= dot;
        }
    }
}

/***************************************
函数: DisRecBox
功能: 显示X1/Y1到X2/Y2为对角线的矩形框
说明: 1.获得矩形左上角的X/Y,和矩形的边长M/N
      2.建立在DisRecArea概念上
      3.用法同DisRecArea
***************************************//*
void DisRecBox(uchar Y1,uchar X1,uchar Y2,uchar X2)
{
    uchar idata Y,X;
    uchar idata M,N;
    uchar idata i,j;
    uchar idata row;
    uchar idata column;
    uchar idata page;
    uchar idata dot;
    //-------------------
    //Y1&=0x3f; Y2&=0x3f;
    //X1&=0x7f; X2&=0x7f;
    if(Y1> 63)Y1= 63;
    if(X1>127)X1=127;
    if(Y2> 63)Y2= 63;
    if(X2>127)X2=127;
    //-------------------
    if(Y1<Y2){ Y=Y1; M=Y2-Y1; }
    else     { Y=Y2; M=Y1-Y2; }
    if(X1<X2){ X=X1; N=X2-X1; }
    else     { X=X2; N=X1-X2; }
    //-------------------
    for(i=0;i<=M;i++)
    {
        row=(Y+i);
        //row&=0x3f;
        //--------
        page=row/8;
        dot =row%8;
        dot =_crol_(0x01,dot);
        //-------------------
        for(j=0;j<=N;j++)
        {
            //---------------------------- 掏空
            if(i==0||i==M)
            {
                column=(X+j);
                //column&=0x7f;
                //-----------
                DisBuf[page][column] |= dot;
            }
            else if(j==0||j==N)
            {
                column=(X+j);
                //column&=0x7f;
                //-----------
                DisBuf[page][column] |= dot;
            }
            //----------------------------
        }
    }
}
*/