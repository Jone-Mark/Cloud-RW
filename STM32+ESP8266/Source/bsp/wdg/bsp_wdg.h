/**
  ******************************************************************************
  * @file    bsp_wdg.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_WDG_H_
#define __BSP_WDG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

void IWDG_Config(uint8_t prv ,uint16_t rlv);
void IWDG_Feed(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_WDG_H_ */

/*********************************************END OF FILE**********************/

