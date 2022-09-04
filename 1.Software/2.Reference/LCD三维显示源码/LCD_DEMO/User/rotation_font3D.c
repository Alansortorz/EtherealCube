/*******************************************************************************
* 文档: rotation_font3D.c
* 作者: Wu LianWei
* 描述: 在三维空间中旋转一个字形或图片
*******************************************************************************/
#include"public.h"
#include"font.h"
#include"dis_graphics.h"
#include"transform_3D.h"
#include"projection.h"



/***************************************
函数: RotatePic32x32
功能: 在三维空间中旋转一个32x32的字符或图形
说明: 1.根据设定的方式变换图片每个点的坐标,并显示
      2.旋转角度(0~360)
      3.az取正值为顺时针旋转
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
    Translate_3D(gmat,-16,-16,0); //32X32图片
    Rotate_3D(gmat,ax,ay,az);
    Translate_3D(gmat,0,0,32);
    //---------------------
    XOrigin = 64;
    YOrigin = 32;
    //--------------------- 变换32X32图片中,每个显示点的初始坐标
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
                    Point1 = VEC_MultMatrix(Point0,gmat); //坐标变换
                    PointDis = PerProject(Point1);        //透视投影
                    //---------------------
                    DisDot(PointDis.y,PointDis.x);
                }
            }
        }
        dp+=32;
    }
}
