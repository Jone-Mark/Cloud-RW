#ifndef __BSP_KEY_EXTI_H__
#define	__BSP_KEY_EXTI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

typedef void (*KEY_EXTI_CALLBACK)(void);

//引脚定义
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
	
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler

void KEY_Init(KEY_EXTI_CALLBACK key1_exti_callback,KEY_EXTI_CALLBACK key2_exti_callback);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H__ */

