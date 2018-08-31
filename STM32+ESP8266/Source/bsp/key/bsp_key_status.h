#ifndef __BSP_KEY_STATUS_H
#define	__BSP_KEY_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "bsp.h"

typedef void (*KEY_STATUS_CALLBACK)(void);

#define KEY2_INT_GPIO_PORT         GPIOB
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_12
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource12
#define KEY2_INT_EXTI_LINE         EXTI_Line12
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY2_IRQHandler            EXTI15_10_IRQHandler

void KEY_Status_Init(KEY_STATUS_CALLBACK cb);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_KEY_STATUS_H */
