/**
  ******************************************************************************
  * @file    user_config.h
  * @author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   This file contains all switch.
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
#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include <stdio.h>	
#include <string.h>

#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
	
	
#include "./gpio/bsp_gpio.h"
#include "./printf/bsp_printf.h"
#include "./usart/bsp_usart1.h"
#include "./led/bsp_led.h"
#include "./key/bsp_key_exti.h"
#include "./dwt/bsp_dwt.h"	
#include "./iic/bsp_iic.h"
#include "./sht20/bsp_sht20.h"	
#include "esp8266_handler.h"//以后修改目录结构
	
#ifdef __cplusplus
}
#endif

#endif /* __BSP_CONFIG_H__ */

/*********************************************END OF FILE**********************/

