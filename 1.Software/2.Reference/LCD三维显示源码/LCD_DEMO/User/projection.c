/*******************************************************************************
* �ĵ�: projection.c
* ����: Wu LianWei
* ����: 1.3Dת2D��ͶӰ�㷨
*       2.���㷨
*******************************************************************************/
typedef struct 
{
    int x; int y;
}_2D;
typedef struct 
{
    float x; float y; float z;
}_3D;



/***************************************
����: OrtProject
����: ����ͶӰ(Orthographic projection)
***************************************/
_2D OrtProject(_3D xdata Space)
{
    _2D xdata Screen;
    //---------------
    Screen.x = (int)Space.x;
    Screen.y = (int)Space.y;
    //---------------
    return Screen;
}



//����͸��ͶӰ�ı�׼ģ��
#define FOCAL_DISTANCE 128 //�ӵ㵽��ƽ��ľ���
int xdata XOrigin;
int xdata YOrigin;
/***************************************
����: PerProject
����: ͸��ͶӰ(Perspective projection)
˵��: 1.�ֳ�Ϊ����ͶӰ��
      2.XOrigin,YOriginΪͶӰ���ͼ�����ĵ���Ļ����
***************************************/
_2D PerProject(_3D xdata Space)
{
    _2D xdata Screen;
    //-------------------
    if(Space.z==0)Space.z=1; //����������Ϊ��
    Screen.x = (int)( FOCAL_DISTANCE * Space.x / (Space.z + FOCAL_DISTANCE) ) + XOrigin;
    Screen.y = (int)( FOCAL_DISTANCE * Space.y / (Space.z + FOCAL_DISTANCE) ) + YOrigin;
    //-------------------
    return Screen;
}
