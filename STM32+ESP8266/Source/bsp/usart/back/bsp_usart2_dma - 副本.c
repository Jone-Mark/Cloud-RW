/**
  ******************************************************************************
  * @file    bsp_usart_dma.c
	* @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   DMA无中断接收串口2数据
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "bsp_usart2_dma.h"

#ifdef USE_USART2

/** 串口2寄存器 */
#define SRC_USART2_DR  (&(USART2->DR))
/** 接收缓冲区长度 */
#define USART2_DMA_RX_BUFFER_LEN   32
/** 接收缓冲区 */
static uint8_t g_receive_buffer[USART2_DMA_RX_BUFFER_LEN];

/** 发送缓冲区长度 */
#define USART2_DMA_TX_BUFFER_LEN   200
/** 发送缓冲区 */
static uint8_t g_send_buffer[USART2_DMA_TX_BUFFER_LEN];
/** 发送标志位 */
static uint16_t g_USART2_TX_DMA_Finish= 1;


QueueHandle_t g_RX_Queue; //RX数据接受消息队列

/**
  * @brief  USART2 DMA TX配置初始化
  * @retval None
  */
void USART2_DMA_TX_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /** 关闭USART2的接收DMA请求*/
    DMA_Cmd(DMA1_Channel7,DISABLE);
    /** 打开 DMA 时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /** 复位 DMA 控制器*/
    DMA_DeInit(DMA1_Channel7);
    /** 外设基址:串口2接收寄存器*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_USART2_DR;
    /** 存储器地址:自定义缓冲区*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_send_buffer;
    /** 设置外设位目标，内存缓冲区 -> 外设寄存器*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    /** 缓冲区大小，等于缓冲区大小*/
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_TX_BUFFER_LEN;
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
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    /** 清除标志位*/
    DMA_ClearFlag(DMA1_FLAG_TC7);
    /** 关闭 DMA 通道*/
    DMA_Cmd(DMA1_Channel7, DISABLE);
    /** 开启发送DMA通道中断*/
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

    /** DMA发送中断设置*/
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_USART2_TX_DMA_NVIC_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /** 使能USART2的DMA发送*/
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
}

/**
  * @brief  USART2 DMA RX配置初始化
  * @retval None
  */
void USART2_DMA_RX_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    /** 关闭USART2的接收DMA请求*/
    DMA_Cmd(DMA1_Channel6,DISABLE);
    /** 打开 DMA 时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /** 复位 DMA 控制器*/
    DMA_DeInit(DMA1_Channel6);
    /** 外设基址:串口2接收寄存器*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_USART2_DR;
    /** 存储器地址:自定义缓冲区*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_receive_buffer;
    /** 数据源来自外设*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    /** 缓冲区大小，等于缓冲区大小*/
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_RX_BUFFER_LEN;
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
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    /** 使能 DMA 通道*/
    DMA_Cmd(DMA1_Channel6, ENABLE);

    /** 使能USART2的DMA接收*/
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
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
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_USART2_RX_NVIC_PRIORITY;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief  USART2 GPIO 配置初始化
 * @param  usart_baudRate 波特率 USART_BAUDRATE_115200/USART_BAUDRATE_9600
 * @retval None
 */
void USART2_GPIO_Config(uint32_t usart_baudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /** 打开串口GPIO的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

    /** 打开串口外设的时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /** 将USART Tx的GPIO配置为推挽复用模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /** 将USART Rx的GPIO配置为浮空输入模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13;				 //LED0-->PB.0,1 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.0 1
    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14);

    /** 配置串口的工作参数 */
    /** 配置波特率 */
    USART_InitStructure.USART_BaudRate = usart_baudRate;
    /** 配置 针数据字长 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /** 配置停止位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /** 配置校验位 */
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    /** 配置硬件流控制 */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /** 配置工作模式，收发一起 */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /** 完成串口的初始化配置 */
    USART_Init(USART2, &USART_InitStructure);

    /** 使能串口2 */
    USART_Cmd(USART2, ENABLE);

    NVIC_Configuration();
    //中断配置
    USART_ITConfig(USART2,USART_IT_TC,DISABLE);
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
}

/**
  * @brief  USART2 空闲中断接收数据
  * @return None
  */
void USART2_IRQHandler(void)
{
    uint16_t i,len;
    BaseType_t xHigherPriorityTaskWoken;
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA1_Channel6,DISABLE);
        USART2->SR;
        USART2->DR; //读SR后读DR 清USART_IT_IDLE标志
        len = USART2_DMA_RX_BUFFER_LEN-DMA_GetCurrDataCounter(DMA1_Channel6);
        for(i=0; i<len; i++)
        {
            /** 消息队列添加数据 */
            xQueueSendFromISR(g_RX_Queue,&g_receive_buffer[i],&xHigherPriorityTaskWoken);
//					PRINTF_LOG("rx = %d \n",g_receive_buffer[i]);
        }

        //设置传输数据长度
        DMA_SetCurrDataCounter(DMA1_Channel6,USART2_DMA_RX_BUFFER_LEN);
        //打开DMA
        DMA_Cmd(DMA1_Channel6,ENABLE);

//        if( xHigherPriorityTaskWoken )
//        {
//            portYIELD_FROM_ISR (xHigherPriorityTaskWoken);
//        }
    }
}

/**
  * @brief  USART2 DMA方式发送中断
  * @return None
  */
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_FLAG_TC7))
    {
        /** 清除标志位*/
        DMA_ClearFlag(DMA1_FLAG_TC7);
        /** 关闭DMA*/
        DMA_Cmd(DMA1_Channel7,DISABLE);
        /** 允许再次发送*/
        g_USART2_TX_DMA_Finish = 1;
    }
}

/**
  * @brief  发送数据
  * @return
  * @attention 只有g_USART2_TX_DMA_Finish 为1时执行
  */
void USART2_DMA_SendBuff(uint8_t *pData, uint16_t len)
{
    uint32_t i;
    /** 等待上次发送完毕 */
    while(g_USART2_TX_DMA_Finish == 0);
    g_USART2_TX_DMA_Finish = 0;
    for(i=0; i<len; i++)
    {
        g_send_buffer[i] = pData[i];
    }
    //设置传输数据长度
    DMA_SetCurrDataCounter(DMA1_Channel7,len);
    /** 使能 DMA 通道*/
    DMA_Cmd(DMA1_Channel7, ENABLE);
}
void USART2_DMA_SendOneByte(uint8_t data)
{
    while(g_USART2_TX_DMA_Finish == 0);
    g_USART2_TX_DMA_Finish = 0;
    g_send_buffer[0] = data;
//	PRINTF_LOG("rx = %d \n",data);
    //设置传输数据长度
    DMA_SetCurrDataCounter(DMA1_Channel7,1);
    /** 使能 DMA 通道*/
    DMA_Cmd(DMA1_Channel7, ENABLE);
}
QueueHandle_t USART2_RX_Queue_Get(void)
{
    return g_RX_Queue;
}

/**
  * @brief  串口发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
//void USART2_Send(const uint8_t* const pData, uint32_t len)
//{
//    int i;
//    for( i = 0; i<len ; i++)
//    {
//        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
//        USART_SendData(USART2, pData[i]);
//    }
//}
//int USART2_SendChar(uint8_t ch)
//{
//    USART_SendData(USART2, (uint8_t) ch);
//    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

//    return ch;
//}

/**
  * @brief  USART2 及 其DMA初始化
  * @return None
  */
void USART2_DMA_Init(uint32_t usart_baudRate)
{
    g_RX_Queue=xQueueCreate(2*USART2_DMA_RX_BUFFER_LEN,sizeof(uint8_t));
    /** 串口2 GPIO初始化 */
    USART2_GPIO_Config(usart_baudRate);
    /** 串口2 DMA RX初始化 */
    USART2_DMA_RX_Config();
    /** 串口2 DMA TX初始化 */
    USART2_DMA_TX_Config();
}

#endif
/*********************************************END OF FILE**********************/

