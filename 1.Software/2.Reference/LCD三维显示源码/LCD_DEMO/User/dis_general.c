/*******************************************************************************
* 文档: dis_general.c
* 作者: Wu LianWei
* 描述: 1.一般的字符与图形显示操作
*       2.所有操作都基于显示缓存DisBuf
*       3.配合font.c使用
*******************************************************************************/
#include"public.h"
//#include"font.h"
extern uchar xdata DisBuf[8][128];  //1024字节的显示缓存







/***************************************
函数: ClearDis
功能: 清显示缓存
说明: 把DisBuf全部清零
***************************************/
void ClearDis(void)
{
    uchar i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<128;j++)DisBuf[i][j]=0;
    }
}







/***************************************
函数: DisChar6x8
功能: 在屏幕上显示一个6x8的ASCII码字符
说明: 1.屏幕根据6x8字符,划分为X轴上21个单位,Y轴上8个单位
      2.全屏能显示168个6x8字符
      3.Y取值范围:0~7,X取值范围:0~20
      4.输入显示坐标超出范围，则退出
***************************************//*
void DisChar6x8(uchar Y,uchar X,uchar Character)
{
    uchar i,j;
    //------------------
    if(Y>7||X>20)return;
    //------------------
    if( (Character>=0x20)&&(Character<0x7f) )
    {
        i=Character-0x20;
        for(j=0;j<5;j++) DisBuf[Y][(6*X)+j] = ASCII_Table_6X8[i][j];//把显示数据写入缓存
    }
}

/***************************************
函数: DisString6x8
功能: 在屏幕上显示6x8的ASCII码字符串
说明: 1.屏幕容量同DisChar6x8
      2.Y取值范围:0~7,X取值范围:0~20
      3.输入显示坐标超出范围，则退出
      4.能够自动换行，超出最后一行则退出
***************************************//*
void DisString6x8(uchar Y,uchar X,uchar *str)
{
    uchar i,j;
    //------------------
    if(Y>7||X>20)return;
    //------------------
    while(*str>0x00)
    {
        if( (*str>=0x20)&&(*str<0x7f) )
        {
            i=*str-0x20;
            for(j=0;j<5;j++) DisBuf[Y][(6*X)+j] = ASCII_Table_6X8[i][j];//把显示数据写入缓存
            str++;
            X++;
            if(X>20)
            {
                Y++;
                X=0;
                //------------ 超出最后一行则退出
                if(Y>7)return;
            }
        }
        else str++;
    }
}







/***************************************
函数: DisChar8x16
功能: 在屏幕上显示一个8x16的ASCII码字符
说明: 1.屏幕根据8x16字符,划分为X轴上16个单位,Y轴上4个单位
      2.全屏能显示64个8x16字符
      3.Y取值范围:0~6,X取值范围:0~15
      4.输入显示坐标超出范围，则退出
***************************************//*
void DisChar8x16(uchar Y,uchar X,uchar Character)
{
    uchar i,j,k;
    //------------------
    if(Y>6||X>15)return;
    //------------------
    if( (Character>=0x20)&&(Character<0x7f) )
    {
        k=Character-0x20;
        for(i=0;i<2;i++) 
        {
            for(j=0;j<8;j++)
            DisBuf[Y+i][(8*X)+j] = ASCII_Table_8X16[(2*k)+i][j];//把显示数据写入缓存
        }
    }
}

/***************************************
函数: DisString8x16
功能: 在屏幕上显示8x16的ASCII码字符串
说明: 1.屏幕容量同DisChar8x16
      2.Y取值范围:0~6,X取值范围:0~15
      3.输入显示坐标超出范围，则退出
      4.能够自动换行，超出最后一行则退出
***************************************//*
void DisString8x16(uchar Y,uchar X,uchar *str)
{
    uchar i,j,k;
    //------------------
    if(Y>6||X>15)return;
    //------------------
    while(*str>0x00)
    {
        if( (*str>=0x20)&&(*str<0x7f) )
        {
            k=*str-0x20;
            for(i=0;i<2;i++) 
            {
                for(j=0;j<8;j++)
                DisBuf[Y+i][(8*X)+j] = ASCII_Table_8X16[(2*k)+i][j];//把显示数据写入缓存
            }
            //-----------------
            str++;
            X++;
            if(X>15)
            {
                Y=Y+2;
                X=0;
                //------------ 超出最后一行承载能力则退出
                if(Y>6)return;
            }
        }
        else str++;
    }
}







/***************************************
函数: DisDraw6x8
功能: 在屏幕上显示一个6x8的图形
说明: 1.屏幕根据6x8图形,划分为X轴上21个单位,Y轴上8个单位
      2.全屏能显示168个6x8图形
      3.Y取值范围:0~7,X取值范围:0~20
      4.输入显示坐标超出范围，则退出
***************************************//*
void DisDraw6x8(uchar Y,uchar X,uchar *dp)
{
    uchar j;
    //------------------
    if(Y>7||X>20)return;
    //------------------
    for(j=0;j<6;j++)
    {
        DisBuf[Y][(6*X)+j] = *dp;//把显示数据写入缓存
        dp++;
    }
}

/***************************************
函数: DisDraw8x16
功能: 在屏幕上显示一个8x16的图形
说明: 1.屏幕根据8x16图形,划分为X轴上16个单位,Y轴上4个单位
      2.全屏能显示64个8x16图形
      3.Y取值范围:0~6,X取值范围:0~15
      4.输入显示坐标超出范围，则退出
***************************************//*
void DisDraw8x16(uchar Y,uchar X,uchar *dp)
{
    uchar i,j;
    //------------------
    if(Y>6||X>15)return;
    //------------------
    for(i=0;i<2;i++) 
    {
        for(j=0;j<8;j++)
        {
            DisBuf[Y+i][(8*X)+j] = *dp;//把显示数据写入缓存
            dp++;
        }
    }
}









/***************************************
函数: DisDraw32x32
功能: 在屏幕上显示一个32x32的字符或图形(ICO)
说明: 1.显示位置没有单位坐标的限制
      2.输入参数超出屏幕范围,则退出
      3.图形大小超出屏幕的部分,会消失
***************************************//*
void DisDraw32x32(uchar page,uchar column,uchar *dp)
{
    uchar i,j;
    //------------------
    if(page>7||column>127)return;
    //------------------
    for(i=0;i<4;i++) 
    {
        if( (page+i)>7 )break;
        //--------------------
        for(j=0;j<32;j++)
        {
            //if( (column+j)>127 )break;
            if( (column+j)>127 )
            {
                dp+=(32-j);
                break;
            }
            //------------------------
            DisBuf[page+i][column+j] = *dp;//把显示数据写入缓存
            dp++;
        }
    }
}

/***************************************
函数: DisDraw64x32
功能: 在屏幕上显示一个64x32的图形(LOGO)
说明: 1.显示位置没有单位坐标的限制
      2.输入参数超出屏幕范围,则退出
      3.图形大小超出屏幕的部分,会消失
***************************************//*
void DisDraw64x32(uchar page,uchar column,uchar *dp)
{
    uchar i,j;
    //------------------
    if(page>7||column>127)return;
    //------------------
    for(i=0;i<4;i++) 
    {
        if( (page+i)>7 )break;
        //--------------------
        for(j=0;j<64;j++)
        {
            //if( (column+j)>127 )break;
            if( (column+j)>127 )
            {
                dp+=(64-j);
                break;
            }
            //------------------------
            DisBuf[page+i][column+j] = *dp;//把显示数据写入缓存
            dp++;
        }
    }
}








/***************************************
函数: Reverse6x8
功能: 取反显示n个6x8的字符或图形
说明: 1.输入同6x8型字符,n代表个数
      2.与6x8型字符或图形配套使用
***************************************//*
void Reverse6x8(uchar Y,uchar X,uchar n)
{
    uchar j,k,dat;
    //------------------
    if(Y>7||X>20)return;
    //------------------
    for(k=0;k<n;k++)
    {
        if( X>20 )break;
        //--------------
        for(j=0;j<6;j++)
        {
            dat = DisBuf[Y][(6*X)+j];
            DisBuf[Y][(6*X)+j] = ~dat; //把显示数据取反
        }
        X++;
    }
}

/***************************************
函数: Reverse8x16
功能: 取反显示n个8x16的字符或图形
说明: 1.输入同8x16型字符,n代表个数
      2.与8x16型字符或图形配套使用
***************************************//*
void Reverse8x16(uchar Y,uchar X,uchar n)
{
    uchar i,j,k,dat;
    //------------------
    if(Y>6||X>15)return;
    //------------------
    for(k=0;k<n;k++)
    {
        if( X>15 )break;
        //--------------
        for(i=0;i<2;i++) 
        {
            for(j=0;j<8;j++)
            {
                dat = DisBuf[Y+i][(8*X)+j];
                DisBuf[Y+i][(8*X)+j] =~dat; //把显示数据取反
            }
        }
        X++;
    }
}







/***************************************
函数: BoxOut32x32
功能: 框显,选中一个32x32图标的效果
***************************************//*
void BoxOut32x32(uchar page,uchar column)
{
    if(page>7||column>127)return;
    //---------------------------
    
    DisBuf[page][column]   |= 0x0f;
    DisBuf[page][column+1] |= 0x0f;
    DisBuf[page][column+2] |= 0x03;
    DisBuf[page][column+3] |= 0x03;
    
    DisBuf[page][column+31] |= 0x0f;
    DisBuf[page][column+30] |= 0x0f;
    DisBuf[page][column+29] |= 0x03;
    DisBuf[page][column+28] |= 0x03;
    
    DisBuf[page+3][column]   |= 0xf0;
    DisBuf[page+3][column+1] |= 0xf0;
    DisBuf[page+3][column+2] |= 0xc0;
    DisBuf[page+3][column+3] |= 0xc0;
    
    DisBuf[page+3][column+31] |= 0xf0;
    DisBuf[page+3][column+30] |= 0xf0;
    DisBuf[page+3][column+29] |= 0xc0;
    DisBuf[page+3][column+28] |= 0xc0;
}

*/