/**
  ******************************************************************************
  * @file    bsp_usart_dma_timer.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口配置
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __BSP_USART_DMA_H_
#define	__BSP_USART_DMA_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
	
void USART2_DMA_Receive_Init(void);
void USART2_DMA_Send(const uint8_t* const pData, uint32_t len);
	
#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_DMA_H_ */

/*********************************************END OF FILE**********************/

