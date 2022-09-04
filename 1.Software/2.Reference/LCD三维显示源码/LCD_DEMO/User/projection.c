/*******************************************************************************
* 文档: projection.c
* 作者: Wu LianWei
* 描述: 1.3D转2D的投影算法
*       2.纯算法
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
函数: OrtProject
功能: 正射投影(Orthographic projection)
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



//基于透视投影的标准模型
#define FOCAL_DISTANCE 128 //视点到视平面的距离
int xdata XOrigin;
int xdata YOrigin;
/***************************************
函数: PerProject
功能: 透视投影(Perspective projection)
说明: 1.又称为中心投影法
      2.XOrigin,YOrigin为投影后的图形中心的屏幕坐标
***************************************/
_2D PerProject(_3D xdata Space)
{
    _2D xdata Screen;
    //-------------------
    if(Space.z==0)Space.z=1; //被除数不能为零
    Screen.x = (int)( FOCAL_DISTANCE * Space.x / (Space.z + FOCAL_DISTANCE) ) + XOrigin;
    Screen.y = (int)( FOCAL_DISTANCE * Space.y / (Space.z + FOCAL_DISTANCE) ) + YOrigin;
    //-------------------
    return Screen;
}
