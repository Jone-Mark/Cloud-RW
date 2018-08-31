/**
  ******************************************************************************
  * @file    bsp_usart3_dma.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口配置
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __BSP_USART3_DMA_H_
#define	__BSP_USART3_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

void USART3_DMA_Init(uint32_t usart_baudRate);
void USART3_DMA_SendBuff(uint8_t *pData, uint16_t len);
void USART3_DMA_SendOneByte(uint8_t data);

void USART3_DMA_Send_Ready(void);
void USART3_DMA_Send_Add(uint8_t *pData, uint16_t len);
void USART3_DMA_Send_Start(uint16_t len);

QueueHandle_t USART3_Get_Queue(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART3_DMA_H_ */

/*********************************************END OF FILE**********************/

