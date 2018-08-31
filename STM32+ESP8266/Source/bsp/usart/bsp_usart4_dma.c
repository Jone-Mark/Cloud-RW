/**
  ******************************************************************************
  * @file    bsp_usart_dma.c
	* @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   DMA无中断接收串口数据
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "bsp_usart4_dma.h"



/** 串口2寄存器 */
#define SRC_USART4_DR  (&(UART4->DR))
/** 接收缓冲区 */
static uint8_t g_receive_buffer[USART_DMA_RX_BUFFER_LEN];
/** 发送缓冲区 */
uint8_t g_uart4_send_buffer[USART_DMA_TX_BUFFER_LEN];

/** 发送标志位 */
static uint16_t g_USART4_TX_DMA_Finish= 1;


QueueHandle_t g_USART4_Queue; //RX数据接受消息队列
static Uart_RX g_uart4_rx;
/**
  * @brief  UART4 DMA TX配置初始化
  * @retval None
  */
void USART4_DMA_TX_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /** 关闭UART4的接收DMA请求*/
    DMA_Cmd(DMA2_Channel5,DISABLE);
    /** 打开 DMA 时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    /** 复位 DMA 控制器*/
    DMA_DeInit(DMA2_Channel5);
    /** 外设基址:串口4接收寄存器*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_USART4_DR;
    /** 存储器地址:自定义缓冲区*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_uart4_send_buffer;
    /** 设置外设位目标，内存缓冲区 -> 外设寄存器*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    /** 缓冲区大小，等于缓冲区大小*/
    DMA_InitStructure.DMA_BufferSize = USART_DMA_TX_BUFFER_LEN;
    /** 外设寄存器只有一个，地址不用递增*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /** 外设寄存器只有一个，地址不用递增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /** 外设数据单位:字节*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /** 内存数据单位:字节(跟外设数据大小相同)*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    /** 工作在模式:单次传输模式*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /** 传输通道优先级:高(当使用一个 DMA 通道时，优先级设置不影响)*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    /** 禁止存储器到存储器模式，因为是从外设到存储器*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    /** 初始化DMA*/
    DMA_Init(DMA2_Channel5, &DMA_InitStructure);
    /** 清除标志位*/
    DMA_ClearFlag(DMA2_FLAG_TC5);
    /** 关闭 DMA 通道*/
    DMA_Cmd(DMA2_Channel5, DISABLE);
    /** 开启发送DMA通道中断*/
    DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, ENABLE);

    /** DMA发送中断设置*/
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_USART4_TX_DMA_NVIC_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /** 使能UART4的DMA发送*/
    USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
}

/**
  * @brief  UART4 DMA RX配置初始化
  * @retval None
  */
void USART4_DMA_RX_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    /** 关闭UART4的接收DMA请求*/
    DMA_Cmd(DMA2_Channel3,DISABLE);
    /** 打开 DMA 时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    /** 复位 DMA 控制器*/
    DMA_DeInit(DMA2_Channel3);
    /** 外设基址:串口2接收寄存器*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_USART4_DR;
    /** 存储器地址:自定义缓冲区*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_receive_buffer;
    /** 数据源来自外设*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    /** 缓冲区大小，等于缓冲区大小*/
    DMA_InitStructure.DMA_BufferSize = USART_DMA_RX_BUFFER_LEN;
    /** 外设寄存器只有一个，地址不用递增*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /** 外设寄存器只有一个，地址不用递增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /** 外设数据单位:字节*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /** 内存数据单位:字节(跟外设数据大小相同)*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    /** 工作在模式:普通传输模式*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /** 传输通道优先级:高(当使用一个 DMA 通道时，优先级设置不影响)*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    /** 禁止存储器到存储器模式，因为是从外设到存储器*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    /** 初始化DMA*/
    DMA_Init(DMA2_Channel3, &DMA_InitStructure);
    /** 使能 DMA 通道*/
    DMA_Cmd(DMA2_Channel3, ENABLE);

    /** 使能UART4的DMA接收*/
    USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
}

/**
 * @brief  配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置USART为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_USART_RX_NVIC_PRIORITY;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief  UART4 GPIO 配置初始化
 * @param  usart_baudRate 波特率 USART_BAUDRATE_115200/USART_BAUDRATE_9600
 * @retval None
 */
void USART4_GPIO_Config(uint32_t usart_baudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /** 打开串口GPIO的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /** 打开串口外设的时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    /** 将USART Tx的GPIO配置为推挽复用模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /** 将USART Rx的GPIO配置为浮空输入模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /** 配置串口的工作参数 */
    /** 配置波特率 */
    USART_InitStructure.USART_BaudRate = usart_baudRate;
    /** 配置 针数据字长 */
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    /** 配置停止位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /** 配置校验位 */
    USART_InitStructure.USART_Parity = USART_Parity_Even ;
    /** 配置硬件流控制 */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /** 配置工作模式，收发一起 */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /** 完成串口的初始化配置 */
    USART_Init(UART4, &USART_InitStructure);

    /** 使能串口3 */
    USART_Cmd(UART4, ENABLE);

    NVIC_Configuration();
    //中断配置
    USART_ITConfig(UART4,USART_IT_TC,DISABLE);
    USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);
    USART_ITConfig(UART4,USART_IT_IDLE,ENABLE);
}

/**
  * @brief  UART4 空闲中断接收数据
  * @return None
  */
void UART4_IRQHandler(void)
{
    uint16_t i,len;
    BaseType_t xHigherPriorityTaskWoken;
    if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA2_Channel3,DISABLE);
        UART4->SR;
        UART4->DR; //读SR后读DR 清USART_IT_IDLE标志
        len = USART_DMA_RX_BUFFER_LEN-DMA_GetCurrDataCounter(DMA2_Channel3);
        g_uart4_rx.uartNum = 4;
        for(i=0; i<len; i++)
        {
            g_uart4_rx.readData = g_receive_buffer[i];
            /** 消息队列添加数据 */
            xQueueSendFromISR(g_USART4_Queue,&g_uart4_rx,&xHigherPriorityTaskWoken);
//					PRINTF_LOG("g_uart4_rx = %d \n",g_receive_buffer[i]);
        }

        //设置传输数据长度
        DMA_SetCurrDataCounter(DMA2_Channel3,USART_DMA_RX_BUFFER_LEN);
        //打开DMA
        DMA_Cmd(DMA2_Channel3,ENABLE);
    }
}

/**
  * @brief  UART4 DMA方式发送中断
  * @return None
  */
void DMA2_Channel4_5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_FLAG_TC5))
    {
        /** 清除标志位*/
        DMA_ClearFlag(DMA2_FLAG_TC5);
        /** 关闭DMA*/
        DMA_Cmd(DMA2_Channel5,DISABLE);
        /** 允许再次发送*/
        g_USART4_TX_DMA_Finish = 1;
    }
}

/**
  * @brief  设置传输数据长度并使能DMA
  * @return
  * @attention
  */
void USART4_DMA_Send_Start(uint16_t len)
{
    //设置传输数据长度
    DMA_SetCurrDataCounter(DMA2_Channel5,len);
    /** 使能 DMA 通道*/
    DMA_Cmd(DMA2_Channel5, ENABLE);
}
/**
  * @brief  等待上次发送完毕
  * @return
  * @attention
  */
void USART4_DMA_Send_Ready(void)
{
    /** 等待上次发送完毕 */
    while(g_USART4_TX_DMA_Finish == 0);
    g_USART4_TX_DMA_Finish = 0;
}

/**
  * @brief  DMA send buffer 添加数据
  * @return
  * @attention
  */
void USART4_DMA_Send_Add(uint8_t *pData, uint16_t len)
{
    uint32_t i;
    for(i=0; i<len; i++)
    {
        g_uart4_send_buffer[i] = pData[i];
    }
}

/**
  * @brief  发送数据
  * @return
  * @attention 只有g_USART4_TX_DMA_Finish 为1时执行
  */
void USART4_DMA_SendBuff(uint8_t *pData, uint16_t len)
{
    USART4_DMA_Send_Ready();
    USART4_DMA_Send_Add(pData,len);
    USART4_DMA_Send_Start(len);
}

void USART4_DMA_SendOneByte(uint8_t data)
{
    USART4_DMA_Send_Ready();
    USART4_DMA_Send_Add(&data,1);
    USART4_DMA_Send_Start(1);
}
QueueHandle_t USART4_Get_Queue(void)
{
    return g_USART4_Queue;
}

/**
  * @brief  UART4 及 其DMA初始化
  * @return None
  */
void USART4_DMA_Init(uint32_t usart_baudRate)
{
    g_USART4_Queue=xQueueCreate(20,sizeof(Uart_RX));
    /** 串口2 GPIO初始化 */
    USART4_GPIO_Config(usart_baudRate);
    /** 串口2 DMA RX初始化 */
    USART4_DMA_RX_Config();
    /** 串口2 DMA TX初始化 */
    USART4_DMA_TX_Config();
}

/*********************************************END OF FILE**********************/

