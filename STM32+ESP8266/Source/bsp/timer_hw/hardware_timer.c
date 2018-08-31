/**
  ******************************************************************************
  * @file    hardware_timer.c
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ????
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "hardware_timer.h"

#define RCC_APB1Periph_TIMx(x)     (RCC_APB1Periph_TIM2 << (x))
#define TIMx(x) ((TIM_TypeDef *)   (TIM2_BASE+(x*0x0400)))

#define TIMER_RUNNING    1
#define TIMER_STOP       0

/** 回调函数 */
static TIMER_CALLBACK g_callBack[MAX_TIMER];
/** 定义 4 个通用定时器 */
static const uint8_t TIMx_IRQn[]= {TIM2_IRQn, TIM3_IRQn,TIM4_IRQn, TIM5_IRQn,};
/** 记录定时器状态*/
static __IO uint8_t g_isRunning[MAX_TIMER]= {TIMER_STOP, TIMER_STOP,TIMER_STOP, TIMER_STOP,};

void TIMER_irqHandler(TIMER_TIMERx timerId);

void TIMER_irqHandler(TIMER_TIMERx timerId)
{
    if(TIM_GetITStatus(TIMx(timerId), TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIMx(timerId), TIM_IT_Update);
        g_callBack[timerId]();
    }
}

/** 中断响应函数 */
void TIM2_IRQHandler(void)
{
    TIMER_irqHandler(TIMER2);
}
void TIM3_IRQHandler(void)
{
    TIMER_irqHandler(TIMER3);
}
void TIM4_IRQHandler(void)
{
    TIMER_irqHandler(TIMER4);
}
void TIM5_IRQHandler(void)
{
    TIMER_irqHandler(TIMER5);
}

/**
  * @brief  中断控制器初始化
  * @param  timerId 通用定时器ID
  * @retval None
  */
void TIMER_NVIC_Config(uint32_t timerId)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel =TIMx_IRQn[timerId];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  定时器配置
 * @param  timerId 通用定时器ID
 * @param  timeout 定时时间
 * @param  precision 定时精度us/ms
 * @retval None
 */
void TIMER_Config(uint32_t timerId, uint16_t timeout, uint16_t precision)
{
    uint16_t timerPeriod = 0;
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx(timerId) , ENABLE);
    TIM_DeInit(TIMx(timerId));
    switch(precision)
    {
        case TIMER_PRECISION_US:
            timerPeriod = 1;
            break;
        case TIMER_PRECISION_MS:
            timerPeriod = 2;
            break;
        default:
            break;
    }
    TIM_TimeBaseStructure.TIM_Period=timeout * timerPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler=precision;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx(timerId), &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIMx(timerId), TIM_FLAG_Update);
    TIM_ITConfig(TIMx(timerId),TIM_IT_Update,ENABLE);
    TIM_Cmd(TIMx(timerId), ENABLE);
}

/**
 * @brief  开始一个新定时器
 * @param  timerId 通用定时器ID
 * @param  timeout 定时时间
 * @param  precision 定时精度us/ms
 * @param  TIMER_CALLBACK 回调函数
 * @retval None
 */
void TIMER_Start(uint32_t timerId, uint16_t timeout, uint16_t precision, TIMER_CALLBACK callBack)
{
    g_callBack[timerId] = callBack;
    TIMER_NVIC_Config(timerId);
    TIMER_Config(timerId, timeout, precision);
    g_isRunning[timerId] = TIMER_RUNNING;
}

/**
  * @brief  停止定时器
  * @param  timerId 通用定时器ID
  * @retval None
  */
void TIMER_Stop(uint32_t timerId)
{
    if(g_isRunning[timerId])
    {
        g_isRunning[timerId] = TIMER_STOP;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx(timerId) , DISABLE);
        TIM_DeInit(TIMx(timerId));
        TIM_ITConfig(TIMx(timerId),TIM_IT_Update,DISABLE);
        TIM_Cmd(TIMx(timerId), DISABLE);
    }
}

/*********************************************END OF FILE**********************/

