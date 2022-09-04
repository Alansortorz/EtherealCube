/*******************************************************************************
* Filename: rtc.h
* Author  : Wu LianWei
*******************************************************************************/
#ifndef _RTC_H_
#define _RTC_H_



extern volatile bit T20msFlg;
extern volatile bit T100msFlg;
extern volatile bit T500msFlg;
extern volatile bit T1sFlg;
//--------------------------
extern void Timer0_cfg(void);
extern void Timer0_isr(void);
#define RTC_cfg() Timer0_cfg()



#endif
