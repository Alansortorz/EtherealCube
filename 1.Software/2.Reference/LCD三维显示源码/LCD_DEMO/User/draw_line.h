/******************************************************************************
* ÎÄµµ: draw_line.h
* ×÷Õß: Wu LianWei
******************************************************************************/
#ifndef _DRAW_LINE_H_
#define _DRAW_LINE_H_

extern void BresenhamLine(int x1,int y1,int x2,int y2);
#define DrawLine(X1,Y1,X2,Y2)  BresenhamLine(X1,Y1,X2,Y2)

#endif