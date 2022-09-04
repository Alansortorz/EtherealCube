/*******************************************************************************
* Filename: rtc.c
* Author  : Wu LianWei
*******************************************************************************/
#include<STC15F2K60S2.h>





//--------------------------
volatile bit T20msFlg;
volatile bit T100msFlg;
volatile bit T500msFlg;
volatile bit T1sFlg;
unsigned char idata T100msCnt;
unsigned char idata T500msCnt;
unsigned char idata T1sCnt;
//--------------------------






/***************************************
函数: Timer0_cfg
功能: Timer0配置 (使能中断)
说明: 用作RTC时钟产生
***************************************/
void Timer0_cfg(void)
{
    //定时器0计数时钟,是系统时钟/12.
    TH0 = 0x63;
    TL0 = 0xc0;  //计数40000次溢出,定时20ms.
    TMOD |= 0x03;//定时器0工作在模式3,不可屏蔽中断的16位自动重装方式
    TR0 = 1;     //定时器0计数启动
    ET0 = 1;     //定时器0溢出中断使能
}



/***************************************
函数: Timer0_isr
功能: Timer0中断
说明: 用作RTC时钟标志置位
***************************************/
void Timer0_isr(void) interrupt 1
{
    //定时器0溢出标志硬件清0.
    //20ms中断
    T20msFlg=1;
    if(++T100msCnt>=5)
    {
        T100msCnt=0;
        T100msFlg=1;
        if(++T500msCnt>=5)
        {
            T500msCnt=0;
            T500msFlg=1;
            if(++T1sCnt>=2)
            {
                T1sCnt=0;
                T1sFlg=1;
            }
        }
    }
}
