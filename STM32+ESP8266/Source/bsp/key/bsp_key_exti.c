#include "./key/bsp_key_exti.h"

static TimerHandle_t	g_OneShotTimer_Handle;
static TimerHandle_t 	g_OneLongTimer_Handle;

static KEY_EXTI_CALLBACK g_key1_exti_callback;
static KEY_EXTI_CALLBACK g_key2_exti_callback;

/**
 * @brief  配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置中断源：按键1 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
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
void KEY_EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*开启按键GPIO口的时钟*/
    RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);

    /* 配置 NVIC 中断*/
    NVIC_Configuration();

    /*--------------------------KEY1配置-----------------------------*/
 	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void KEY1_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
    {
        xTimerStop(g_OneShotTimer_Handle,0);
		xTimerStop(g_OneLongTimer_Handle,0);
		
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
		
        xTimerStart(g_OneShotTimer_Handle,0);
		xTimerStart(g_OneLongTimer_Handle,0);
    }
}

void vKEYTask(TimerHandle_t xTimer)
{
    if(GPIO_ReadInputDataBit(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN))
    {
		printf("shortKey1\r\n");
        g_key1_exti_callback();
    }
}

void vKEYTask2(TimerHandle_t xTimer)
{
    if(!GPIO_ReadInputDataBit(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN))
    {
        g_key2_exti_callback();
    }
}
void KEY_Init(KEY_EXTI_CALLBACK key1_exti_callback, KEY_EXTI_CALLBACK key2_exti_callback)
{
    g_key1_exti_callback = key1_exti_callback;
	g_key2_exti_callback = key2_exti_callback;
    KEY_EXTI_Config();
    g_OneShotTimer_Handle=xTimerCreate((const char*			)"vKEYTask",
                                       (TickType_t			)250, //250ms内抬起有效，超过250ms小于5S未释放按键则无效
                                       (UBaseType_t			)pdFALSE,
                                       (void*					)1,
                                       (TimerCallbackFunction_t)vKEYTask);
									   
	g_OneLongTimer_Handle=xTimerCreate((const char*			)"vKEYTask",
								   (TickType_t			)5000, //5s
								   (UBaseType_t			)pdFALSE,
								   (void*					)1,
								   (TimerCallbackFunction_t)vKEYTask2);
}


/*********************************************END OF FILE**********************/
