/**
  ******************************************************************************
  * @file    bsp_usart1.c
	* @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口1配置,配合printf
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "./usart/bsp_usart1.h"
uint8_t Usart2_Rx_Buf[USART2_RX_BUFF_SIZE];
extern xQueueHandle xQueue_USART2_Cmd;                 /**< AT命令消息队列*/

usart_recv_callback recv_cb1;
dma_recv_callback recv_cb2;
/**
 * @brief  USART GPIO 配置初始化
 * @param  usart_baudRate 波特率 USART_BAUDRATE_115200/USART_BAUDRATE_9600
 * @retval None
 */
void USART1_Init(uint32_t usart_baudRate)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /** 打开串口GPIO的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /** 打开串口外设的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /** 将USART Tx的GPIO配置为推挽复用模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /** 将USART Rx的GPIO配置为浮空输入模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

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
    USART_Init(USART1, &USART_InitStructure);

    /** 使能串口 */
    USART_Cmd(USART1, ENABLE);

    /** 使能串口中断接受 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_USART1_RX_NVIC_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
/**
  ******************************************************************************
  * @brief  串口中断函数
  * @retval None
  ******************************************************************************/
void USART1_IRQHandler(void)
{
    uint8_t ch;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        ch = USART_ReceiveData(USART1);
		USART1_SendChar(ch);

        /** Read one byte from the receive data register */
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
/**
  * @brief  串口发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART1_Send(const uint8_t* const pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, pData[i]);
    }
}
/**
  * @brief  串口发送字符
  * @param  ch 要发送的字符
  * @retval 发送的字符
  */
int USART1_SendChar(int ch)
{
    USART_SendData(USART1, (uint8_t) ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}

/**
  * @brief  串口接受字符
  * @retval 接受的字符
  */
int USART1_GetChar(void)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}

void USART2_Init(uint32_t usart_baudRate,usart_recv_callback cb, dma_recv_callback dma_cb)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    recv_cb2 = dma_cb;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = usart_baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

//    USART_Init(USART3, &USART_InitStructure);

    USART_ClearFlag(USART2, USART_FLAG_TC);


    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //采用DMA方式接收
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USART2, ENABLE);

    /*dma配置*/
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//串口收DMA配置//启动DMA时钟

        DMA_DeInit(DMA1_Channel6); //DMA1通道3配置

        DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);			//外设地址
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart2_Rx_Buf;			//内存地址
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//dma传输方向单向
        DMA_InitStructure.DMA_BufferSize = USART2_RX_BUFF_SIZE; 				//设置DMA在传输时缓冲区的长度
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//设置DMA的外设递增模式，一个外设，地址不自增
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//设置DMA的内存递增模式
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据字长
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//内存数据字长
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//设置DMA的传输模式
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//设置DMA的优先级别
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 							//设置DMA的2个memory中的变量互相访问
        DMA_Init(DMA1_Channel6, &DMA_InitStructure);

        DMA_Cmd(DMA1_Channel6, ENABLE);//使能通道6
    }
}

/**
  ******************************************************************************
  * @brief  串口2中断函数
  * @retval None
  ******************************************************************************/
void USART2_IRQHandler(void)
{
    uint32_t len;

    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA1_Channel6,DISABLE);
        USART2->SR;
        USART2->DR; 												                //读SR后读DR 清USART_IT_IDLE标志
        if(Usart2_Rx_Buf[0]!=0 && strlen((char *)Usart2_Rx_Buf) > 2)
        {
            len = USART2_RX_BUFF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);      //计算实际接收数据长度
//			printf("len = %d\r\n",len);
//			printf("%s\r\n",Usart2_Rx_Buf);
            Usart2_Rx_Buf[USART2_RX_BUFF_SIZE - 1]= 0;
//            recv_cb2(Usart2_Rx_Buf, &Boardstring_e,len >= USART2_RX_BUFF_SIZE ? (USART2_RX_BUFF_SIZE - 1) : len);
            xQueueSendFromISR(xQueue_USART2_Cmd, (void *)Usart2_Rx_Buf,0);
            memset(Usart2_Rx_Buf, 0x00, USART2_RX_BUFF_SIZE);
        }

        if(USART2_RX_BUFF_SIZE == len)
        {
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        }
        else
        {
            DMA_SetCurrDataCounter(DMA1_Channel6,USART2_RX_BUFF_SIZE);	//设置传输数据长度
            DMA_Cmd(DMA1_Channel6,ENABLE);								//打开DMA
        }
    }
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);

        DMA_SetCurrDataCounter(DMA1_Channel6,USART2_RX_BUFF_SIZE);	//设置传输数据长度
        DMA_Cmd(DMA1_Channel6,ENABLE);								//打开DMA
    }
}

/**
  * @brief  串口2发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART2_Send(const char* const pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, pData[i]);
    }
}
/**
  * @brief  串口2发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART2_SendData(const uint8_t* const pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, pData[i]);
    }
}
/**
  * @brief  串口发送字符
  * @param  ch 要发送的字符
  * @retval 发送的字符
  */
int USART2_SendChar(int ch)
{
    USART_SendData(USART2, (uint8_t) ch);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

    return ch;
}

/*********************************************END OF FILE**********************/

