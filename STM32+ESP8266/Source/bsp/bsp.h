/**
  ******************************************************************************
  * @file    bsp.h
  * @author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   This file contains all common headers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H_
#define __BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"	
			
/** 串口波特率 */
#define BSP_USART_BAUDRATE_460800   460800
#define BSP_USART_BAUDRATE_115200   115200

/** 优先级管理 */

/** 优先级分组:NVIC_PriorityGroup_4   0~15  */
/** USART5 */
#define BSP_USART1_RX_NVIC_PRIORITY     		5
/** USART */
#define BSP_USART2_RX_NVIC_PRIORITY     		4
#define BSP_USART2_TX_DMA_NVIC_PRIORITY     3

/** USB */
#define BSP_USB_RX_NVIC_PRIORITY     				2
#define BSP_USB_TX_NVIC_PRIORITY     				1
/** key */
#define BSP_DEBUG_KEY_NVIC_PRIORITY     		10

/** 发送缓冲区长度 */
#define USART_DMA_TX_BUFFER_LEN   					210
/** 接收缓冲区长度 */
#define USART_DMA_RX_BUFFER_LEN   					8

void BSP_Init(void);
void ReadChipID(char* cpId, int len);
void ReconfigWIFI(void);
#ifdef __cplusplus
}
#endif

#endif /* __BSP_H_ */

/*********************************************END OF FILE**********************/

