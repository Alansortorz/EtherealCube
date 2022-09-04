#include "hc05.h"
#include <string.h>
#include <stdio.h>
#include "oled.h"

extern BLTDev bltDevList;

/**
 * @brief 设备初始化
 **/
void HC05_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief 32向蓝牙发送命令函数
 * @note
 **/
uint8_t HC05_Send_CMD(char *cmd, uint8_t clean){
	uint8_t retry = 5;
	uint8_t i, ret=1;
	while(retry--){
		//告诉蓝牙，我发送的是命令，将KEY引脚设置为高电平
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		Delay_ms(10);
		//串口发送字符串
		Usart_SendString(USART2, (uint8_t *)cmd);
		for(i=0; i<20; i++){
			uint16_t len;
			char *redata;
			Delay_ms(10);
			//获取缓冲区字符长度
			redata = get_rebuff(&len);
			HC05_DEBUG("len:%d", len);
			if(len > 0){ 
				if(redata[0] != 0){
					//将缓冲区的数据打印到串口
					OLED_ShowString(0, 4, "                ");
					OLED_ShowString(0, 4, cmd);
					OLED_ShowString(0, 6, "                ");
					OLED_ShowString(0, 6, redata);
				}
				if(strstr(redata, "OK")){
					if(clean == 1)
						buff_clear();
					return 0;
				}else{
					;//啥也不干
				}
			}
			else{
				Delay_ms(10);
			}
		}
		HC05_DEBUG("HC05 send CMD fail %d times", retry);
	}
	HC05_DEBUG("HC05 send CMD fail ");
	if(clean == 1)
		buff_clear();
	return ret;
}

/**
 * @brief 透传
 **/
void HC05_SendString(char *str){
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	Usart_SendString(USART2, (uint8_t *)str);
}

/**
 * @brief 初始化GPIO并检测HC05模块
 **/
uint8_t HC05_Init(void){
	uint8_t i;
	HC05_GPIO_Config();
	Delay_ms(100);
	Usart_InitConfig();
	Delay_ms(100);
	for(i=0; i<BLTDEV_MAX_NUM; ++i){
		sprintf(bltDevList.unpraseAddr[i], " ");
		sprintf(bltDevList.name[i], " ");
	}
	bltDevList.num = 0;
	Delay_ms(100);
	return HC05_Send_CMD("AT\r\n", 1);
}

/**
  * @brief  把接收到的字符串转化成16进制形式的数字变量(主要用于转化蓝牙地址)
	* @param  纯粹的数字字符串
  * @retval 转化后的数字变量	
  */
unsigned long htoul(const char *str)
{


  long result = 0;

  if (!str)
    return 0;

  while (*str)
  {
    uint8_t value;

    if (*str >= 'a' && *str <= 'f')
      value = (*str - 'a') + 10;
    else if (*str >= 'A' && *str <= 'F')
      value = (*str - 'A') + 10;
    else if (*str >= '0' && *str <= '9')
      value = *str - '0';
    else
      break;

    result = (result * 16) + value;
    ++str;
  }

  return result;
}


 /**
  * @brief  在str中，跳过它前面的prefix字符串,
						如str为"abcdefg",prefix为"abc"，则调用本函数后返回指向"defg"的指针
	* @param  str,原字符串
	* @param  str_length，字符串长度
	* @param 	prefix，要跳过的字符串
  * @retval 跳过prefix后的字符串指针
  */
char *skipPrefix(char *str, size_t str_length, const char *prefix)
{

  uint16_t prefix_length = strlen(prefix);

  if (!str || str_length == 0 || !prefix)
    return 0;

  if (str_length >= prefix_length && strncmp(str, prefix, prefix_length) == 0)
    return str + prefix_length;

  return 0;
}

/**
  * @brief  向HC05写入命令，不检查模块的响应
	* @param  command ，要发送的命令
	* @param  arg，命令参数，为0时不带参数，若command也为0时，发送"AT"命令
  * @retval 无
  */
void writeCommand(const char *command, const char *arg)
{
		char str_buf[50];

		BLT_KEY_HIGHT;
		Delay_ms(10);

		if (arg && arg[0] != 0)
			sprintf(str_buf,"AT+%s%s\r\n",command,arg);
		else if (command && command[0] != 0)
		{
			sprintf(str_buf,"AT+%s\r\n",command);
		}
		else
			sprintf(str_buf,"AT\r\n");

		HC05_DEBUG("CMD send:%s",str_buf);
		
		Usart_SendString(USART2,(uint8_t *)str_buf);

}


 


 /**
  * @brief  从stream中获取一行字符串到line中
	* @param  line,存储获得行的字符串数组
	* @param  stream，原字符串数据流
	* @param 	max_size，stream的大小 
  * @retval line的长度，若stream中没有‘\0’，'\r'，'\n'，则返回0
  */
int get_line(char* line, char* stream ,int max_size)  
{  
		char *p;	
    int len = 0;  
		p=stream;
    while( *p != '\0' && len < max_size ){  
        line[len++] = *p;  
				p++;
        if('\n' == *p || '\r'==*p)  
            break;  
    }
	
		if(*p != '\0' && *p != '\n' && *p != '\r')
			return 0;
  
      
    line[len] = '\0';  
    return len;  
} 

