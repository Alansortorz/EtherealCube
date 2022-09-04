#include "oled.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];
uint8_t Bef[3];//保存前一个数据的几个参数1.要写在第几页2.0x01要移动几位3.写什么数据
uint8_t Cur[3];//当前前一个数据1.要写在第几页2.0x01要移动几位3.写什么数据

#if OLED_MODE==1
/**
  * @brief 向SSD1106写入一个字节
	* @param dat:要写入的数据/命令  cmd:数据/命令标志 0,表示命令;1,表示数据
	* @retval None
  */
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
	DATAOUT(dat);
	if(cmd)
	  OLED_DC_Set();
	else
		OLED_DC_Clr();
	OLED_CS_Clr();
	OLED_WR_Clr();
	OLED_WR_Set();
	OLED_CS_Set();
	OLED_DC_Set();
}
#else
/**
  * @brief 向SSD1106写入一个字节数据
  * @param data:写入的数据或者命令   cmd:0表示写入数据 1表示写入命令
  */
void OLED_WR_Byte(uint8_t data, uint8_t cmd){
    uint8_t i = 0;
    if(cmd){
        OLED_DC_Set();
    } else
        OLED_DC_Clr();
    for(i=0; i<8; ++i){
        OLED_SCL_Clr();
        if(data & 0x80){
            OLED_SDA_Set();
        } else
            OLED_SDA_Clr();
        OLED_SCL_Set();
        data <<= 1;
    }
    OLED_DC_Set();
}
#endif


//更新显存到OLED
void OLED_Refresh(void)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i, OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00, OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10, OLED_CMD);   //设置高列起始地址
        for(n=0;n<128;n++)
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t i, m, n;
    i= y/8;
    m = y%8;
    n = 1<<m;
    if(t){
        OLED_GRAM[x][i] |= n;
    }
    else{
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}

//x:0~127
//y:0~63
//t:1 填充 0,清空
//x1, y1:起点坐标
//x2, y2:结束坐标
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    uint16_t t;
    int xerr=0, yerr=0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    uint8_t temp;
    if(x1>=128)
        x1 = 128-1;
    else if(x2>=128)
        x2 = 128-1;
    else if(y1>=64)
        y1 = 64-1;
    else if(y2>=64)
        y2 = 64-1;
    if(y1 > y2){
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    delta_x = x2-x1; //计算坐标增量
    delta_y = y2-y1;
    uRow = x1;//画线起点坐标
    uCol = y1;
    if(delta_x > 0)
        incx = 1; //设置单步方向
    else if(delta_x == 0)
        incx = 0;//垂直线
    else{
        incx=-1;
        delta_x = -delta_x;
    }
    if(delta_y > 0)
        incy = 1;
    else if(delta_y == 0)
        incy = 0;//水平线
    else{
        incy = -1;
        delta_y = -delta_x;
    }
    if(delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;
    for(t=0; t<distance+1; t++){
        OLED_DrawPoint(uRow, uCol, mode);//画点
        xerr += delta_x;
        yerr += delta_y;
        if(xerr > distance){
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance){
            yerr -= distance;
            uCol += incy;
        }
    }
}



void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

/**
  * @brief
  */
void OLED_Display_On(void){
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
  * @brief 清屏函数
  */
void OLED_Display_Off(void){
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}


/**
  * @brief 清屏
  */
void OLED_Clear(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        for(n=0; n<128; n++)
        {
            OLED_GRAM[n][i]=0;//清除所有数据
        }
    }
    OLED_Refresh();//更新显示
}

/**
  * @brief 在指定位置显示一个字符,包括部分字符
	* @param x:0~127   y:0~63   mode:0,反白显示  1,正常显示	 size:选择字体 16/12
	* @retval None
  */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr){
    unsigned char c = 0, i = 0;
    c=chr - ' ';														//得到偏移后的值
    if(x > Max_Column-1){
        x=0;
        y=y+2;
    }
    if(SIZE==16){
        OLED_Set_Pos(x,y);
        for(i=0;i<8;i++)
            OLED_WR_Byte(D8X16[c*16+i],OLED_DATA);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
            OLED_WR_Byte(D8X16[c*16+i+8],OLED_DATA);
    }
    else{
        OLED_Set_Pos(x,y+1);
        for(i=0;i<6;i++){
            OLED_WR_Byte(D6X8[c][i],OLED_DATA);
        }
    }
}

/**
  * @brief m^n函数
	* @param None
	* @retval None
  */
uint32_t oled_pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}

/**
 * @brief 波形绘制函数-设置起始位置
 * */
void Wave_SetPos(unsigned char x, unsigned char y){
    OLED_WR_Byte(0xB0+x, OLED_CMD);
    OLED_WR_Byte((y&0x0F)|0x00, OLED_CMD);
    OLED_WR_Byte(((y&0xF0)>>4)|0x10, OLED_CMD);
}

/**
 * @brief 波形绘制函数-描点函数
 * */
void Wave_DrawPoint(uint8_t x, uint8_t y){
    if(x>127 & y>63)
        return;
    uint8_t  page, move, data;
    page = 7-y/8;
    move = 7-y%8;
    data = 0x01<<move;
    Wave_DrawPoint(page, x);
    OLED_WR_Byte(data, OLED_DATA);
}

/**
 * @brief 求出前一个数据的有关参数
 * @note 写入数据
 */
void Before_State_Update(uint8_t y){
    Bef[0]=7-y/8;
    Bef[1]=7-y%8;
    Bef[2]=1<<Bef[1];
}

void Current_State_Update(uint8_t y){
    Cur[0]=7-y/8;//数据写在第几页
    Cur[1]=7-y%8;//0x01要移动的位数
    Cur[2]=1<<Cur[1];//要写什么数据
}

/**
  * @brief 波形绘制函数
  *
  */
void OLED_DrawWave(int8_t x, int8_t y){
    int8_t page_sub;
    uint8_t page_buff, i, j;
    y += 32;
    if(x>127 | y>63){
        x = 127;
        y = 63;
    }
    if(x<0 | y<0){
        x = 0;
        y = 0;
    }
    Current_State_Update(y);
    page_sub = Bef[0]-Cur[0];
    if(page_sub>0){
        page_buff = Bef[0];
        Wave_SetPos(page_buff, x);
        OLED_WR_Byte(Bef[2]-0x01, OLED_DATA);
        page_buff--;
        for(i=0;i<page_sub-1;i++){
            Wave_SetPos(page_buff,x);
            OLED_WR_Byte(0xff, OLED_DATA);
            page_buff--;
        }
        Wave_SetPos(page_buff,x);
        OLED_WR_Byte(0xff<<Cur[1], OLED_DATA);
    }
    else if(page_sub == 0){
        if(Cur[1] == Bef[1]){
            Wave_SetPos(Cur[0], x);
            OLED_WR_Byte(Cur[2], OLED_DATA);
        }
        else if(Cur[1] > Bef[1]){
            Wave_SetPos(Cur[0], x);
            OLED_WR_Byte((Cur[2]-Bef[2])|Cur[2], OLED_DATA);
        }
        else if(Cur[1] < Bef[1]){
            Wave_SetPos(Cur[0], x);
            OLED_WR_Byte(Bef[2]-Cur[2], OLED_DATA);
        }
    }
    else if(page_sub < 0){
        page_buff = Cur[0];
        Wave_SetPos(page_buff, x);
        OLED_WR_Byte((Cur[2]<<1)-0x01, OLED_DATA);
        page_buff--;
        for(i=0; i<0-page_sub-1; i++){
            Wave_SetPos(page_buff, x);
            OLED_WR_Byte(0xff, OLED_DATA);
            page_buff--;
        }
        Wave_SetPos(page_buff, x);
        OLED_WR_Byte(0xff<<(Bef[1]+1), OLED_DATA);
    }
    Before_State_Update(y);
    //数据清除
    for(i=0; i<8; ++i){
        Wave_SetPos(i, x+1) ;
        for(j=0; j<1; j++)
            OLED_WR_Byte(0x00, OLED_DATA);
    }
}

/**
 * @brief 绘制坐标系
 * */
void Wave_DrawCoordinate(void){

};



/**
  * @brief 显示2个数字
	* @param x,y :起点坐标
	*				 len :数字的位数，即显示几位有效数字
  *				 size:字体大小
  *				 mode:模式	0,填充模式;1,叠加模式
  *        num:数值(0~4294967295);
	* @retval None
  */
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size){
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0; t<len; t++){
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0 && t<(len-1)){
            if(temp == 0){
                OLED_ShowChar(x+(size/2)*t, y, ' ');
                continue;
            }else
                enshow=1;
        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0');
    }
}

/**
  * @brief 显示一个字符号串
	* @param
	* @retval None
  */
void OLED_ShowString(uint8_t x,uint8_t y,char *chr){
    unsigned char j=0;
    while(chr[j]!='\0'){
        OLED_ShowChar(x,y,chr[j]);
        x+=8;
        if(x>120){
            x=0;
            y+=2;
        }
        j++;
    }
}

/**
  * @brief 显示汉字
	* @param
	* @retval None
  */
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no){
    uint8_t t, adder=0;
    OLED_Set_Pos(x, y);
    for(t=0; t<32; t++){
        OLED_WR_Byte(SHOW[2*no][t], OLED_DATA);
        adder += 1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0; t<32; t++){
        OLED_WR_Byte(SHOW[2*no+1][t], OLED_DATA);
        adder += 1;
    }
}

/**
  * @brief 显示显示BMP图片
	* @param 显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
	* @retval None
  */
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]){
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8 == 0)
        y = y1/8;
    else
        y = y1/8+1;
    for(y=y0; y<y1; y++){
        OLED_Set_Pos(x0, y);
        for(x=x0; x<x1; x++){
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}
void OLED_InitConfig(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin|OLED_RST_Pin|OLED_DC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    OLED_RST_Set();
    HAL_Delay(200);
    OLED_RST_Clr();
    HAL_Delay(200);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

    OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_Set_Pos(0, 0);
}