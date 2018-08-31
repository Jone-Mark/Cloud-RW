/**
  ******************************************************************************
  * @file    bsp_usart3.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口配置
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __BSP_USART3_H_
#define	__BSP_USART3_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

void USART3_Init(uint32_t usart_baudRate);
int UART3_SendOneByte(int ch);
QueueHandle_t USART3_Get_Queue(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_DMA_H_ */

/*********************************************END OF FILE**********************/

