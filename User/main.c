#include "stm32f10x.h"
#include "bsp_usart.h"
#include "delay.h"
#include "sys.h"
#include "OLED_I2C.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	delay_init();
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(0,42,"hello world",12);

  OLED_Refresh_Gram();

  while(1)
	{	
  	}	
}
/*********************************************END OF FILE**********************/
