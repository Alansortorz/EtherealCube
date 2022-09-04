#include "bsp_led.h"
#include "bsp_systick.h"
#include "stm32f10x.h"
#include "bsp_key.h"
#include "oled.h"
#include "mpu6050.h"
#include "bsp_i2c.h"
#include "hc05.h"
#include "usart_blt.h"
#include "time.h"
#include "bmp.h"
#include "math.h"
#include "filter.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 


BLTDev bltDevList;
int i = 0;
int flag = 0;
short Accdata[3];
short Gyrodata[3];
char linebuff[1024];
char sendData1[64];
char sendData2[64];
char sendData3[64];
char sendData4[64];
char sendData5[64];
char sendData6[64];
char sendData7[64];
char sendData8[64];
char sendData9[64];
float pitch, roll, yaw;
unsigned int Task_Delay[NumOfTask]; 
kalman1_state KFM = {0, 0, 1, 1, 5, 0.01, 0};

int main(void)
{
	char* redata;
	uint16_t len;
	//char hc05_nameCMD[40];
	//char hc05_name[30]="Dance";
	Systick_InitConfig();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	GPIO_OLED_InitConfig();
	OLED_DrawBMP(0,0,126,8,BMP);
	LED_GPIO_Config();
	Key_GPIO_Config();
	i2c_GPIO_Config();
	MPU6050_Init();
	Delay_ms(1000);
	HC05_GPIO_Config();
	TIM3_Init(255, 0);
	Delay_ms(1000);
	
	if(HC05_Init() == 0){
		OLED_Clear();
		OLED_ShowString(0, 2, "BlueTooth Detect");
	}else{
		OLED_Clear();
		OLED_ShowString(0, 2, "BlueTooth Failed");
		while(1);
	}
	
	OLED_Clear();
	for(i=0; i<2; ++i){
		OLED_ShowCHinese(i*16, 0, i);
	}
	OLED_ShowString(32, 0, "AT");
	for(i=2; i<6; ++i){
		OLED_ShowCHinese((i+1)*16, 0, i);
	}
	
	HC05_Send_CMD("AT+VERSION?\r\n",1);
	HC05_Send_CMD("AT+ADDR?\r\n",1);
	HC05_Send_CMD("AT+UART?\r\n",1);
	HC05_Send_CMD("AT+CMODE?\r\n",1);
	HC05_Send_CMD("AT+STATE?\r\n",1);
	HC05_Send_CMD("AT+ROLE=0\r\n",1);
	//HC05_Send_CMD("AT+INIT\r\n",1);
	HC05_Send_CMD("AT+CLASS=0\r\n",1);
	HC05_Send_CMD("AT+INQM=1,9,48\r\n",1);
	
	//sprintf(hc05_nameCMD,"AT+NAME=%s\r\n",hc05_name);
	//HC05_Send_CMD(hc05_nameCMD,1);
	
	for(i=6; i<10; ++i){
		OLED_ShowCHinese((i-4)*16, 2, i);
	}
	Delay_ms(100);
	//OLED_Clear();
	
	while(mpu_dmp_init()){
		OLED_ShowString(0, 5, "mpu_dmp_error");
		Delay_ms(10);
		LED3_TOGGLE;
	}
	LED1_TOGGLE;
	OLED_ShowString(32, 6, "successful");
	
	while(1){
		
		if(Task_Delay[2]==0 && !IS_HC05_CONNECTED() && flag==0){
			//从模式
			//OLED_Clear();
			OLED_ShowString(0, 2, "Waiting For Link");
			HC05_Send_CMD("AT+INQ\r\n",1);//模块在查询状态，才能容易被其它设备搜索到
			Delay_ms(1000);
			HC05_Send_CMD("AT+INQC\r\n",1);//中断查询，防止查询的结果干扰串口透传的接收
			Task_Delay[2]=2; //此值每1ms会减1，减到0才可以重新进来这里，所以执行的周期是2s
			flag = 1;
		}
		
		if(Task_Delay[0]==0 && IS_HC05_CONNECTED() && flag == 1){
			uint16_t linelen;
			redata = get_rebuff(&len); 
			linelen = get_line(linebuff,redata,len);
			OLED_ShowString(0, 2, "                ");
			OLED_ShowString(0, 4, "                ");
			for(i=10; i<=13; ++i){
				OLED_ShowCHinese((i-8)*16, 0, i);
			}
			if(linelen<200 && linelen != 0){
				if(strcmp(redata,"ON") == 0){
					LED1_OFF;
					OLED_ShowString(0, 6, "                ");
					OLED_ShowString(0, 6, "LED ON");
				}
				else if(strcmp(redata,"OFF") == 0){
					LED1_ON;
					OLED_ShowString(0, 6, "                ");
					OLED_ShowString(0, 6, "LED OFF");
				}
				else if(strcmp(redata,"Hello") == 0){
					LED1_TOGGLE;
					OLED_ShowString(0, 6, "                ");
					OLED_ShowString(0, 6, "LED Tol");
				}
				else{
					OLED_ShowString(0, 6, "                ");
					OLED_ShowString(0, 6, redata);
				}
				buff_clear();
			}
			Task_Delay[0]=5;//此值每1ms会减1，减到0才可以重新进来这里
		}
		
		
		if(Task_Delay[1]==0 && IS_HC05_CONNECTED() && flag == 1){
			float Temp;
			char Temper[10];
			float AccData[3];
			float GyroData[3];
			//float Kalman_AccData[3];
			float g_xiamen;
			char G0[1];
			MPU6050ReadAcc(Accdata);
			MPU6050ReadGyro(Gyrodata);
			MPU6050_ReturnTemp(&Temp);
		
			/** 注释：
			 *  加速度值范围为+-2G，查询MPU6050手册，输出值为+-16384
			 *  读取值/16384得到具体的加速度值，单位为g，为当地重力加速度值
			 *  地区重力加速度值如何确定？
			 *	近似计算公式：g = g0(1 - 0.00265*cosa)/1 + 2h/R
			 *	g0 = 9.80665(m/s^2)	标准重力加速度值
			 *	a 纬度
			 *  h 海拔高度
			 *  R 地球的平均半径(R = 6379km)
			 *  地理位置：厦门(百度查询) 
			 *		纬度：北纬24°26'46〃 取北纬24.43°
			 *		厦门大学翔安校区平均海拔高度大约为18米
			 *  g_xiamen = 9.80665 * (1 - 0.00265 * cos((24.43/360)*2*3.1415926)) + 2 * 18 / 6379000 = 9.7830;
			 **/
			//g_xiamen = 9.80665 * (1 - 0.00265 * cos((24.43/360)*2*3.1415926)) + 2 * 18 / 6379000;
			
			g_xiamen = 9.7830;
			sprintf(G0, "g_xiamen:%0.4f", g_xiamen);
			OLED_ShowString(0, 2, G0);
			mpu_dmp_get_data(&pitch, &roll, &yaw);
			AccData[0] = g_xiamen*(float)Accdata[0]/16384;
			AccData[1] = g_xiamen*(float)Accdata[1]/16384;
			AccData[2] = g_xiamen*(float)Accdata[2]/16384;
			
			//ResrcData, ProcessNiose_Q, MeasureNoise_R, InitialPrediction
			//Kalman_AccData[0] = Kalman1_Filter(AccData[0], 5, 5, 0);
			//Kalman_AccData[1] = Kalman1_Filter(AccData[1], 0.05, 0.01, 0);
			//Kalman_AccData[2] = Kalman1_Filter(AccData[2], 0.05, 0.01, 0);
			
			GyroData[0] = (float)Gyrodata[0]/131.0;
			GyroData[1] = (float)Gyrodata[1]/131.0;
			GyroData[2] = (float)Gyrodata[2]/131.0;
			
			sprintf(sendData1,"%0.2f ",AccData[0]);
			sprintf(sendData2,"%0.2f ",AccData[1]);
			sprintf(sendData3,"%0.2f ",AccData[2]);
			sprintf(sendData4,"%0.2f ",GyroData[0]);
			sprintf(sendData5,"%0.2f ",GyroData[1]);
			sprintf(sendData6,"%0.2f ",GyroData[2]);
			sprintf(sendData7,"%0.2f ",pitch);
			sprintf(sendData8,"%0.2f ",roll);
			sprintf(sendData9,"%0.2f ",yaw);
			sprintf(Temper, "%0.1f\n", Temp);
			OLED_ShowString(56, 6, Temper);
			
			HC05_SendString(sendData1);
			HC05_SendString(sendData2);
			HC05_SendString(sendData3);
			HC05_SendString(sendData4);
			HC05_SendString(sendData5);
			HC05_SendString(sendData6);
			HC05_SendString(sendData7);
			HC05_SendString(sendData8);
			HC05_SendString(sendData9);
			HC05_SendString(Temper);
		}
		LED2_TOGGLE;
		Delay_ms(5);
	}
}
