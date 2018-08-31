/**
  ******************************************************************************
  * @file    bsp_usart2.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口配置
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __BSP_USART2_H_
#define	__BSP_USART2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

void USART2_Init(uint32_t usart_baudRate);
void UART2_SendOneByte(uint8_t ch);
QueueHandle_t USART2_Get_Queue(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_DMA_H_ */

/*********************************************END OF FILE**********************/

