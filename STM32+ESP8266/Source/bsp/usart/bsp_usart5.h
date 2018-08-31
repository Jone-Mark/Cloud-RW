#ifndef __BSP_USART5_H_
#define	__BSP_USART5_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

/**
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
  */


// 串口5-UART5
#define  DEBUG_USARTx                   UART5
#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

#define  DEBUG_USART_TX_GPIO_PORT         GPIOC
#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_12
#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

#define  DEBUG_USART_IRQ                UART5_IRQn
#define  DEBUG_USART_IRQHandler         UART5_IRQHandler


void USART5_Init(uint32_t usart_baudRate);
void USART5_Send(const uint8_t* const pData, uint32_t len);

int USART5_SendChar(int ch);
int USART5_GetChar(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
