/**
  ******************************************************************************
  * @file    bsp_usart2_dma.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口配置
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __BSP_USART2_DMA_H_
#define	__BSP_USART2_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

void USART2_DMA_Init(uint32_t usart_baudRate);
void USART2_DMA_SendBuff(uint8_t *pData, uint16_t len);
void USART2_DMA_SendOneByte(uint8_t data);

void USART2_DMA_Send_Ready(void);
void USART2_DMA_Send_Add(uint8_t *pData, uint16_t len);
void USART2_DMA_Send_Start(uint16_t len);

//void USART2_Send(const uint8_t* const pData, uint32_t len);
//int USART2_SendChar(uint8_t ch);

QueueHandle_t USART2_Get_Queue(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_DMA_H_ */

/*********************************************END OF FILE**********************/

