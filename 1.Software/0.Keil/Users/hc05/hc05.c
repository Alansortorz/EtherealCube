#include "hc05.h"
#include <string.h>
#include <stdio.h>
#include "oled.h"

extern BLTDev bltDevList;

/**
 * @brief �豸��ʼ��
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
 * @brief 32���������������
 * @note
 **/
uint8_t HC05_Send_CMD(char *cmd, uint8_t clean){
	uint8_t retry = 5;
	uint8_t i, ret=1;
	while(retry--){
		//�����������ҷ��͵��������KEY��������Ϊ�ߵ�ƽ
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		Delay_ms(10);
		//���ڷ����ַ���
		Usart_SendString(USART2, (uint8_t *)cmd);
		for(i=0; i<20; i++){
			uint16_t len;
			char *redata;
			Delay_ms(10);
			//��ȡ�������ַ�����
			redata = get_rebuff(&len);
			HC05_DEBUG("len:%d", len);
			if(len > 0){ 
				if(redata[0] != 0){
					//�������������ݴ�ӡ������
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
					;//ɶҲ����
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
 * @brief ͸��
 **/
void HC05_SendString(char *str){
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	Usart_SendString(USART2, (uint8_t *)str);
}

/**
 * @brief ��ʼ��GPIO�����HC05ģ��
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
  * @brief  �ѽ��յ����ַ���ת����16������ʽ�����ֱ���(��Ҫ����ת��������ַ)
	* @param  ����������ַ���
  * @retval ת��������ֱ���	
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
  * @brief  ��str�У�������ǰ���prefix�ַ���,
						��strΪ"abcdefg",prefixΪ"abc"������ñ������󷵻�ָ��"defg"��ָ��
	* @param  str,ԭ�ַ���
	* @param  str_length���ַ�������
	* @param 	prefix��Ҫ�������ַ���
  * @retval ����prefix����ַ���ָ��
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
  * @brief  ��HC05д����������ģ�����Ӧ
	* @param  command ��Ҫ���͵�����
	* @param  arg�����������Ϊ0ʱ������������commandҲΪ0ʱ������"AT"����
  * @retval ��
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
  * @brief  ��stream�л�ȡһ���ַ�����line��
	* @param  line,�洢����е��ַ�������
	* @param  stream��ԭ�ַ���������
	* @param 	max_size��stream�Ĵ�С 
  * @retval line�ĳ��ȣ���stream��û�С�\0����'\r'��'\n'���򷵻�0
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

