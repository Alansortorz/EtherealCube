/*******************************************************************************
* �ĵ�: rotation_cube.c
* ����: Wu LianWei
* ����: 1.������ת��������3D��ʾ
*       2.����3Dͼ�α任�㷨��ͶӰ�㷨
*******************************************************************************/
#include"transform_3D.h"
#include"projection.h"
#include"draw_line.h"



#define  SIZE  8
//------------------
_3D code Cube[SIZE]=
{
    {0,0,0},
    {8,0,0},
    {0,8,0},
    {8,8,0},
    
    {0,0,8},
    {8,0,8},
    {0,8,8},
    {8,8,8}
};
_3D code Triangle[3]=
{
    {4,2,0},
    {2,6,0},
    {6,6,0}
};



/***************************************
����: RotateCube
����: ��ת��������
˵��: 1.����ͶӰ
***************************************/
void RotateCube( float ax, float ay, float az )
{
    float xdata gmat[4][4];
    unsigned char idata i;
    _3D xdata temp;
    _2D xdata Cube_Dis[SIZE];
    _2D xdata Triangle_Dis[3];
    //---------------------
    Identity_3D(gmat);
    Translate_3D(gmat,-4,-4,-4);
    Scale_3D(gmat,5,5,5);
    Rotate_3D(gmat,ax,ay,az);
    Translate_3D(gmat,64,32,32);
    //---------------------
    for(i=0;i<SIZE;i++)
    {
        temp = VEC_MultMatrix(Cube[i],gmat);
        Cube_Dis[i] = OrtProject(temp);
    }
    for(i=0;i<3;i++)
    {
        temp = VEC_MultMatrix(Triangle[i],gmat);
        Triangle_Dis[i] = OrtProject(temp);
    }
    //---------------------
    DrawLine(Cube_Dis[0].x,Cube_Dis[0].y,Cube_Dis[1].x,Cube_Dis[1].y);
    DrawLine(Cube_Dis[0].x,Cube_Dis[0].y,Cube_Dis[2].x,Cube_Dis[2].y);
    DrawLine(Cube_Dis[3].x,Cube_Dis[3].y,Cube_Dis[1].x,Cube_Dis[1].y);
    DrawLine(Cube_Dis[3].x,Cube_Dis[3].y,Cube_Dis[2].x,Cube_Dis[2].y);
    //------------------------------------------
    DrawLine(Cube_Dis[0+4].x,Cube_Dis[0+4].y,Cube_Dis[1+4].x,Cube_Dis[1+4].y);
    DrawLine(Cube_Dis[0+4].x,Cube_Dis[0+4].y,Cube_Dis[2+4].x,Cube_Dis[2+4].y);
    DrawLine(Cube_Dis[3+4].x,Cube_Dis[3+4].y,Cube_Dis[1+4].x,Cube_Dis[1+4].y);
    DrawLine(Cube_Dis[3+4].x,Cube_Dis[3+4].y,Cube_Dis[2+4].x,Cube_Dis[2+4].y);
    //------------------------------------------
    DrawLine(Cube_Dis[0].x,Cube_Dis[0].y,Cube_Dis[0+4].x,Cube_Dis[0+4].y);
    DrawLine(Cube_Dis[1].x,Cube_Dis[1].y,Cube_Dis[1+4].x,Cube_Dis[1+4].y);
    DrawLine(Cube_Dis[2].x,Cube_Dis[2].y,Cube_Dis[2+4].x,Cube_Dis[2+4].y);
    DrawLine(Cube_Dis[3].x,Cube_Dis[3].y,Cube_Dis[3+4].x,Cube_Dis[3+4].y);
    //---------------------
    DrawLine(Triangle_Dis[0].x,Triangle_Dis[0].y,Triangle_Dis[1].x,Triangle_Dis[1].y);
    DrawLine(Triangle_Dis[0].x,Triangle_Dis[0].y,Triangle_Dis[2].x,Triangle_Dis[2].y);
    DrawLine(Triangle_Dis[1].x,Triangle_Dis[1].y,Triangle_Dis[2].x,Triangle_Dis[2].y);
}



/***************************************
����: RotateCube2
����: ��ת��������
˵��: 1.͸��ͶӰ
***************************************/
void RotateCube2( float ax, float ay, float az )
{
    float xdata gmat[4][4];
    unsigned char idata i;
    _3D xdata temp;
    _2D xdata Cube_Dis[SIZE];
    //_2D xdata Triangle_Dis[3];
    //---------------------
    Identity_3D(gmat);			//��λ����
    Translate_3D(gmat,-4,-4,-4);
    Scale_3D(gmat,6,6,6);
    Rotate_3D(gmat,ax,ay,az);
    Translate_3D(gmat,0,0,32);
    //---------------------
    XOrigin = 64;
    YOrigin = 32;
    //---------------------
    for(i=0;i<SIZE;i++)
    {
        temp = VEC_MultMatrix(Cube[i],gmat);
        Cube_Dis[i] = PerProject(temp);
    }
    //for(i=0;i<3;i++)
    //{
    //    temp = VEC_MultMatrix(Triangle[i],gmat);
    //    Triangle_Dis[i] = PerProject(temp);
    //}
    //---------------------
    DrawLine(Cube_Dis[0].x,Cube_Dis[0].y,Cube_Dis[1].x,Cube_Dis[1].y);
    DrawLine(Cube_Dis[0].x,Cube_Dis[0].y,Cube_Dis[2].x,Cube_Dis[2].y);
    DrawLine(Cube_Dis[3].x,Cube_Dis[3].y,Cube_Dis[1].x,Cube_Dis[1].y);
    DrawLine(Cube_Dis[3].x,Cube_Dis[3].y,Cube_Dis[2].x,Cube_Dis[2].y);
    //------------------------------------------
    DrawLine(Cube_Dis[0+4].x,Cube_Dis[0+4].y,Cube_Dis[1+4].x,Cube_Dis[1+4].y);
    DrawLine(Cube_Dis[0+4].x,Cube_Dis[0+4].y,Cube_Dis[2+4].x,Cube_Dis[2+4].y);
    DrawLine(Cube_Dis[3+4].x,Cube_Dis[3+4].y,Cube_Dis[1+4].x,Cube_Dis[1+4].y);
    DrawLine(Cube_Dis[3+4].x,Cube_Dis[3+4].y,Cube_Dis[2+4].x,Cube_Dis[2+4].y);
    //------------------------------------------
    DrawLine(Cube_Dis[0].x,Cube_Dis[0].y,Cube_Dis[0+4].x,Cube_Dis[0+4].y);
    DrawLine(Cube_Dis[1].x,Cube_Dis[1].y,Cube_Dis[1+4].x,Cube_Dis[1+4].y);
    DrawLine(Cube_Dis[2].x,Cube_Dis[2].y,Cube_Dis[2+4].x,Cube_Dis[2+4].y);
    DrawLine(Cube_Dis[3].x,Cube_Dis[3].y,Cube_Dis[3+4].x,Cube_Dis[3+4].y);
    //---------------------
    //DrawLine(Triangle_Dis[0].x,Triangle_Dis[0].y,Triangle_Dis[1].x,Triangle_Dis[1].y);
    //DrawLine(Triangle_Dis[0].x,Triangle_Dis[0].y,Triangle_Dis[2].x,Triangle_Dis[2].y);
    //DrawLine(Triangle_Dis[1].x,Triangle_Dis[1].y,Triangle_Dis[2].x,Triangle_Dis[2].y);
}
