/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI

  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "bsp_usart.h"

extern uint8_t UART_RxPtr;

/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1) {
  }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void) {
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1) {
  }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void) {
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1) {
  }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void) {
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1) {
  }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {}

// 串口中断服务函数
void DEBUG_USART_IRQHandler(void) {
  __IO uint8_t data;

  if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET) {
    data = USART_ReceiveData(DEBUG_USARTx);

    // 如果为退格键
    if (data == '\b') {
      // 如果指针不在数组的开始位置
      if (UART_RxPtr) {
        Usart_SendByte(DEBUG_USARTx, '\b');
        Usart_SendByte(DEBUG_USARTx, ' ');
        Usart_SendByte(DEBUG_USARTx, '\b');
        UART_RxPtr--;
        UART_RxBuffer[UART_RxPtr] = 0x00;
      }
    }
    // 如果不是退格键
    else {
      // 将数据填入数组UART_RxBuffer
      // 并且将后面的一个元素清零如果数组满了则写入最后一个元素为止
      if (UART_RxPtr < (UART_RX_BUFFER_SIZE - 1)) {
        UART_RxBuffer[UART_RxPtr] = data;
        UART_RxBuffer[UART_RxPtr + 1] = 0x00;
        UART_RxPtr++;
      } else {
        UART_RxBuffer[UART_RxPtr - 1] = data;
        Usart_SendByte(DEBUG_USARTx, '\b');
      }
      // 如果为回车键，则开始处理串口数据
      //		if(data == 13 || data == 10)
      if (data == 41) {
        receive_cmd = 1;
      } else {
        Usart_SendByte(DEBUG_USARTx, data);
      }
    }
    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)  // 溢出
    {
      USART_ClearFlag(USART1, USART_FLAG_ORE);  // 读SR
      USART_ReceiveData(USART1);                // 读DR
    }

    //    USART_ClearFlag(DEBUG_USARTx, USART_FLAG_RXNE);
  }
}

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
