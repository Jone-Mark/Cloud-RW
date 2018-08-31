/**
  ******************************************************************************
  * @file    bsp_usart.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口配置
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __BSP_USART1_H_
#define	__BSP_USART1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

//typedef void (*USART1_RX_CALLBACK)(uint8_t* pData, uint32_t len);

typedef void (*usart_recv_callback)(uint8_t ch);
typedef void (*dma_recv_callback);
#define USART2_RX_BUFF_SIZE   (256)              /*串口2接收数据缓冲区*/	

void USART1_Init(uint32_t usart_baudRate);
void USART1_Send(const uint8_t* const pData, uint32_t len);

int USART1_SendChar(int ch);
int USART1_GetChar(void);
	
void USART2_Init(uint32_t usart_baudRate,usart_recv_callback cb, dma_recv_callback dma_cb);
void USART2_Send(const char* const pData, uint32_t len);
void USART2_SendData(const uint8_t* const pData, uint32_t len);
#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_H_ */

/*********************************************END OF FILE**********************/

