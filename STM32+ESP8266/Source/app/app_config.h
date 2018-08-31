/**
  ******************************************************************************
  * @file    rtos_config.h
  * @author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   This file contains all include.
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
#ifndef __RTOS_CONFIG_H__
#define __RTOS_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"	

	
/** 相对延时 */
#define USER_TASKDELAY_100MS    pdMS_TO_TICKS(100)
#define USER_TASKDELAY_500MS    pdMS_TO_TICKS(500)
#define USER_TASKDELAY_1S   	  pdMS_TO_TICKS(1000)

/** 任务优先级 0 ~ (configMAX_PRIORITIES-1)  */
#define  USER_THREAD_PRIORITY_5     (5)
#define  USER_THREAD_PRIORITY_4     (4)
#define  USER_THREAD_PRIORITY_3     (3)
#define  USER_THREAD_PRIORITY_2     (2)
#define  USER_THREAD_PRIORITY_1     (1)

#ifdef __cplusplus
}
#endif

#endif /* __RTOS_CONFIG_H__ */

/*********************************************END OF FILE**********************/

