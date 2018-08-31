/**
  ******************************************************************************
  * @file    bsp_key_status.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O线中断应用bsp
  ******************************************************************************
  */

#include "./key/bsp_key_status.h"

static KEY_STATUS_CALLBACK g_callback;
static TimerHandle_t	g_statusTimer_Handle;

/**
 * @brief  配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置中断源：按键2，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
    /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_DEBUG_KEY_NVIC_PRIORITY;
    /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  配置 IO为EXTI中断口，并设置中断优先级
 * @param  无
 * @retval 无
 */
void KEY_Status_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*开启按键GPIO口的时钟*/
    RCC_APB2PeriphClockCmd(KEY2_INT_GPIO_CLK,ENABLE);

    /* 配置 NVIC 中断*/
    NVIC_Configuration();

    /*--------------------------KEY2配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
void vStatusTask(TimerHandle_t xTimer)
{
    if(GPIO_ReadInputDataBit(KEY2_INT_GPIO_PORT,KEY2_INT_GPIO_PIN))
    {
        g_callback();
    }
}
void KEY2_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
    {
        xTimerStop(g_statusTimer_Handle,0);
        EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
        xTimerStart(g_statusTimer_Handle,0);
    }
}
void KEY_Status_Init(KEY_STATUS_CALLBACK cb)
{
    g_callback = cb;
    KEY_Status_Config();
    g_statusTimer_Handle=xTimerCreate((const char*			)"vStatusTask",
                                      (TickType_t			)20, //20ms
                                      (UBaseType_t			)pdFALSE,
                                      (void*					)1,
                                      (TimerCallbackFunction_t)vStatusTask);
}
/*********************************************END OF FILE**********************/
