/**
  ******************************************************************************
  * @file    bsp_systick.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ????
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __SYSTICK_H_
#define __SYSTICK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/**	ST3.5.0库版本 */
/** SystemFrequency/1000   1ms中断一次 */
#define SYSTICK_INTERVAL_SECOND  (SystemCoreClock/1000)
/** SystemFrequency/1000,000   1us中断一次 */
#define SYSTICK_INTERVAL_MICROSECOND  (SystemCoreClock/1000000)

#define 	SYSTICK_OPEN   1  /**< 定时器开 */
#define 	SYSTICK_CLOSE  0  /**< 定时器关 */

/** 嘀嗒软件定时器结构*/
typedef struct
{
    uint8_t     SYSTICK_Status;          /**< 定时器工作状态 */
    uint32_t    SYSTICK_Start;           /**< 开始时间 */
    uint32_t    SYSTICK_Interval;  	     /**< 到时时间 */
} SYSTICK_InitTypeDef;

void SYSTICK_Init(void);
uint32_t SYSTICK_Get(void);
void SYSTICK_Timer_Set(SYSTICK_InitTypeDef *initParam, uint32_t interval);
void SYSTICK_Timer_Restart(SYSTICK_InitTypeDef *initParam);
void SYSTICK_Timer_Stop(SYSTICK_InitTypeDef *initParam);
uint32_t SYSTICK_Timer_Expired(SYSTICK_InitTypeDef *initParam);

void SYSTICK_Delay_Us( __IO uint32_t us);
void SYSTICK_Delay_Ms( __IO uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTICK_H_ */

/*********************************************END OF FILE**********************/

