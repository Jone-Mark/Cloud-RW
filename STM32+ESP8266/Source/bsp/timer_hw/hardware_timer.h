/**
  ******************************************************************************
  * @file    hardware_timer.h
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ????
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __HARDWARE_TIMER_H_
#define __HARDWARE_TIMER_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
	
 /** 0.5ms timeout < 0xffff/2 */
#define TIMER_PRECISION_MS   (36000-1)   
 /** 0.5ms 1 us  // timeout < 0xffff */
#define TIMER_PRECISION_US   71         

 /** 通用定时器ID */
typedef enum TIMERx_Tag
{
    TIMER2 = 0,
    TIMER3,
    TIMER4,
    TIMER5,
    MAX_TIMER
} TIMER_TIMERx;

typedef void (*TIMER_CALLBACK)(void);

void TIMER_Start(uint32_t timerId, uint16_t timeout, uint16_t precision, TIMER_CALLBACK callBack);
void TIMER_Stop(uint32_t timerId);

#ifdef __cplusplus
}
#endif

#endif /* __HARDWARE_TIMER_H_ */

/*********************************************END OF FILE**********************/

