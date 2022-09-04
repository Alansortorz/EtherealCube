/*******************************************************************************
* �ĵ�: rotation_font3D.c
* ����: Wu LianWei
* ����: ����ά�ռ�����תһ�����λ�ͼƬ
*******************************************************************************/
#include"public.h"
#include"font.h"
#include"dis_graphics.h"
#include"transform_3D.h"
#include"projection.h"



/***************************************
����: RotatePic32x32
����: ����ά�ռ�����תһ��32x32���ַ���ͼ��
˵��: 1.�����趨�ķ�ʽ�任ͼƬÿ���������,����ʾ
      2.��ת�Ƕ�(0~360)
      3.azȡ��ֵΪ˳ʱ����ת
***************************************/
void RotatePic32x32( uchar *dp, float ax, float ay,float az )
{
    uchar idata i,j,k,temp;
    //---------------------
    float xdata gmat[4][4];
    _3D xdata Point0;
    _3D xdata Point1;
    _2D xdata PointDis;
    //---------------------
    Identity_3D(gmat);
    Translate_3D(gmat,-16,-16,0); //32X32ͼƬ
    Rotate_3D(gmat,ax,ay,az);
    Translate_3D(gmat,0,0,32);
    //---------------------
    XOrigin = 64;
    YOrigin = 32;
    //--------------------- �任32X32ͼƬ��,ÿ����ʾ��ĳ�ʼ����
    for(i=0;i<4;i++) 
    {
        for(k=0;k<8;k++)
        {
            temp=_crol_(0x01,k);
            for(j=0;j<32;j++)
            {
                if( *(dp+j) & temp )//DisDot(Y+(i*8)+k,X+j);
                {
                    Point0.x = j;
                    Point0.y = (i*8)+k;
                    Point0.z = -24;
                    //---------------------
                    Point1 = VEC_MultMatrix(Point0,gmat); //����任
                    PointDis = PerProject(Point1);        //͸��ͶӰ
                    //---------------------
                    DisDot(PointDis.y,PointDis.x);
                }
            }
        }
        dp+=32;
    }
}
