/**
  ******************************************************************************
  * @file    bsp_led.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LED_H__
#define	__BSP_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */

#define LED0_GPIO_PORT    	GPIOB		              /* GPIO端口 */
#define LED0_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED0_GPIO_PIN		GPIO_Pin_7			        /* 连接到SCL时钟线的GPIO */

#define LED1_GPIO_PORT    	GPIOB		              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED1_GPIO_PIN		GPIO_Pin_4			        /* 连接到SCL时钟线的GPIO */

#define LED2_GPIO_PORT    	GPIOB		              /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED2_GPIO_PIN		GPIO_Pin_5			        /* 连接到SCL时钟线的GPIO */

#define NET_LED_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define NET_LED_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define NET_LED_GPIO_PIN		GPIO_Pin_3			        /* 连接到SCL时钟线的GPIO */

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED0_TOGGLE			digitalToggle(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_OFF			digitalHi(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_ON				digitalLo(LED0_GPIO_PORT,LED0_GPIO_PIN)


#define LED1_TOGGLE			digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF			digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON				digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)


#define LED2_TOGGLE			digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF			digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON				digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)


#define NET_LED_TOGGLE			digitalToggle(NET_LED_GPIO_PORT,NET_LED_GPIO_PIN)
#define NET_LED_OFF		    digitalHi(NET_LED_GPIO_PORT,NET_LED_GPIO_PIN)
#define NET_LED_ON			    digitalLo(NET_LED_GPIO_PORT,NET_LED_GPIO_PIN)

typedef enum
{
	LED_NETWORK_NOK = 0,
	LED_NETWORK_OK
}LEDNetworkStatus;

void LED_Config(void);
void SetLedConnectNet(LEDNetworkStatus status);
#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_H__ */
