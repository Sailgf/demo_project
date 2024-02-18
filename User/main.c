#include "OLED_I2C.h"
#include "bsp_usart.h"
#include "delay.h"
#include "stm32f10x.h"
#include "sys.h"

/**
 * @brief  主函数d
 * @param  无
 * @retval 无
 */
int main(void) {
  delay_init();
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
  OLED_Init();
  OLED_Clear();
  OLED_ShowString(0, 42, "hello world", 12);
  OLED_Refresh_Gram();

  while (1) {
  }
}
/******************************END OF FILE**********************/
