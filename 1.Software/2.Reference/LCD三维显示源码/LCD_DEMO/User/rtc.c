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
����: Timer0_cfg
����: Timer0���� (ʹ���ж�)
˵��: ����RTCʱ�Ӳ���
***************************************/
void Timer0_cfg(void)
{
    //��ʱ��0����ʱ��,��ϵͳʱ��/12.
    TH0 = 0x63;
    TL0 = 0xc0;  //����40000�����,��ʱ20ms.
    TMOD |= 0x03;//��ʱ��0������ģʽ3,���������жϵ�16λ�Զ���װ��ʽ
    TR0 = 1;     //��ʱ��0��������
    ET0 = 1;     //��ʱ��0����ж�ʹ��
}



/***************************************
����: Timer0_isr
����: Timer0�ж�
˵��: ����RTCʱ�ӱ�־��λ
***************************************/
void Timer0_isr(void) interrupt 1
{
    //��ʱ��0�����־Ӳ����0.
    //20ms�ж�
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
