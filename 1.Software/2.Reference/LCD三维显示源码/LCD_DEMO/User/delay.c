/*******************************************************************************
* 文档: delay.c
* 作者: Wu LianWei
*******************************************************************************/
#include"public.h"



/***************************************
函数: DelayMs
功能: 毫秒延时(根据系统时钟做的延时)
***************************************/
void DelayMs(uchar n)
{
    uchar i;
    uint j;
    for(i=0;i<n;i++)
        for(j=0;j<24000;j++);
}
