/**
 ******************************************************************************
 * @file    bsp_systick.c
 * @Author  Simic
 * @version V1.0
 * @date    2013-xx-xx
 * @brief   ????
 ******************************************************************************
 * @attention
 ******************************************************************************
 */

#include "bsp_systick.h"

#ifdef USE_SYSTICK
/** 滴答次数  */
static __IO uint32_t g_systick_time = 0;

/**
  * @brief  滴答定时器初始化
  * @param  无
  * @retval None
  */
void SYSTICK_Init(void)
{
    if (SysTick_Config(SYSTICK_INTERVAL_SECOND))
    {
        while (1);
    }

    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  This function handles SysTick Handler.
  * @retval None
  * @attention 注释掉 stm32f10x_it.c 中的 SysTick_Handler 函数
  */
void SysTick_Handler(void)
{
    g_systick_time++;
}

/**
  * @brief  获取当前滴答次数
  * @retval 滴答次数
  */
uint32_t SYSTICK_Get(void)
{
    return g_systick_time;
}

/**
  * @brief  开启软件计时器
  * @param  initParam 软件计时器结构指针
  * @param  interval 定时时间
  * @return None.
  */
void SYSTICK_Timer_Set(SYSTICK_InitTypeDef *initParam, uint32_t interval)
{
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    initParam->SYSTICK_Status = SYSTICK_OPEN;
    initParam->SYSTICK_Interval = interval;
    initParam->SYSTICK_Start = SYSTICK_Get();
}

/**
  * @brief  复位软件计时器
  * @param  initParam 软件计时器结构指针
  * @return None.
  */
void SYSTICK_Timer_Restart(SYSTICK_InitTypeDef *initParam)
{
    initParam->SYSTICK_Start = SYSTICK_Get();
    initParam->SYSTICK_Status = SYSTICK_OPEN;
}

/**
  * @brief  停止软件计时器
  * @param  initParam 软件计时器结构指针
  * @return None.
  */
void SYSTICK_Timer_Stop(SYSTICK_InitTypeDef *initParam)
{
    initParam->SYSTICK_Status = SYSTICK_CLOSE;
}

/**
  * @brief  判断定时器是否到时
  * @param  initParam 软件计时器结构指针
  * @return 定时器结果
  */
uint32_t SYSTICK_Timer_Expired(SYSTICK_InitTypeDef *initParam)
{
    if(initParam->SYSTICK_Status)
        return (uint32_t)(SYSTICK_Get() - initParam->SYSTICK_Start) >= (uint32_t)initParam->SYSTICK_Interval;
    else
        return 0;
}

/**
  * @brief   微秒延时
  * @param   us 延时时间
  * @retval  None
  */
void SYSTICK_Delay_Us( __IO uint32_t us)
{
    uint32_t i;
    SysTick_Config(SYSTICK_INTERVAL_MICROSECOND);

    for(i=0; i<us; i++)
    {
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    /** 关闭SysTick定时器  */
    SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief   毫秒延时
  * @param   ms 延时时间
  * @retval  None
  */
void SYSTICK_Delay_Ms( __IO uint32_t ms)
{
    uint32_t i;
    SysTick_Config(SYSTICK_INTERVAL_SECOND);

    for(i=0; i<ms; i++)
    {
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    /** 关闭SysTick定时器  */
    SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

#endif
/*********************************************END OF FILE**********************/

