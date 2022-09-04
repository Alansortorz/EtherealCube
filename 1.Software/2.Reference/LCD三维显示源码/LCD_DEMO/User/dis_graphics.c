/*******************************************************************************
* �ĵ�: dis_graphics.c
* ����: Wu LianWei
* ����: 1.������ͼ��ʾ�Ļ���:DisDot
*       2.�����Ļ�ͼ��ʾ����
*       3.���в�����������ʾ����DisBuf
*******************************************************************************/
#include"public.h"
extern uchar xdata DisBuf[8][128];  //1024�ֽڵ���ʾ����





/***************************************
����: DisDot
����: ����Ļ����λ����ʾһ����
˵��: 1.��ʾ��λ������(Y:0~63,X:0~127)
      2.�������귶Χ,����ʾ
***************************************/
void DisDot(uchar Y,uchar X)
{
    uchar idata page;
    uchar idata dot;
    //-------------------
    //Y&=0x3f;
    //X&=0x7f; //�����������귶Χ
    //-------------------
    if(Y>63||X>127)return; //���곬����Χ,���˳�
    //-------------------
    page=Y/8;
    dot =Y%8;
    dot =_crol_(0x01,dot);
    //-------------------
    DisBuf[page][X] |= dot;
}





/***************************************
����: DisDotMatrix
����: ��X/YΪ�������,��ʾһ���߳�ΪNxM�ĵ㷽��
˵��: 1.���λ������(Y:0~63,X:0~127)
      2.�������귶Χ,����ʾ
      3.�������λ�ڵ㷽������Ͻ�
      4.M��N����߳�,��M��N=0ʱ,����ʾ
      5.������DisDot������
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
    //X&=0x7f; //�����������귶Χ
    //-------------------
    if(Y>63||X>127)return; //���곬����Χ,���˳�
    //-------------------
    for(i=0;i<M;i++)
    {
        if( (Y+i)>63 )break; //���곬����Χ,���˳�
        row=(Y+i);
        //row&=0x3f; //�����������귶Χ
        //--------
        page=row/8;
        dot =row%8;
        dot =_crol_(0x01,dot);
        //-------------------
        for(j=0;j<N;j++)
        {
            if( (X+j)>127 )break; //���곬����Χ,���˳�
            column=(X+j);
            //column&=0x7f; //�����������귶Χ
            //-----------
            DisBuf[page][column] |= dot;
        }
    }
}

/***************************************
����: DisRecArea
����: ��ʾX1/Y1��X2/Y2Ϊ�Խ��ߵľ�����
˵��: 1.��þ������Ͻǵ�X/Y,�;��εı߳�M/N
      2.���귶Χ(Y:0~63,X:0~127)
      3.��������,������ʾ
      4.������DisDotMatrix������
      5.����߳�Ϊ��
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
����: DisRecBox
����: ��ʾX1/Y1��X2/Y2Ϊ�Խ��ߵľ��ο�
˵��: 1.��þ������Ͻǵ�X/Y,�;��εı߳�M/N
      2.������DisRecArea������
      3.�÷�ͬDisRecArea
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
            //---------------------------- �Ϳ�
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