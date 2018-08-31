/**
  ******************************************************************************
  * @file    bsp_usart_dma_timer.c
	* @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口配置
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "bsp_usart_dma_timer.h"

//#if defined(USE_USART2_DMA)

//超时时间定义
#define        UART2_TimeoutComp 1 

#define SRC_USART2_DR (&(USART2->DR)) //串口接收寄存器作为源头

#define USART_DMA_BUFFER_SIZE   200

uint16_t UART2_Flag=0;
uint8_t uart2_data[200];

uint8_t UART2_Timeout;
uint16_t UART2_FlagTemp;
uint8_t uart2_data_temp[200];

uint16_t uart2_Flag_last=0;

void DMA6_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel6); //将DMA的通道1寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_USART2_DR; //源头BUF
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart2_data_temp; //目标BUF 既是要写在哪个个数组之中
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设作源头//外设是作为数据传输的目的地还是来源
    DMA_InitStructure.DMA_BufferSize = USART_DMA_BUFFER_SIZE; //DMA缓存的大小 单位在下边设定
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设字节为单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //内存字节为单位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //4优先级之一的(高优先)VeryHigh/High/Medium/Low
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //非内存到内存
    DMA_Init(DMA1_Channel6, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道1寄存器
//    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE); //DMA6传输完成中断
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); //使能USART2的接收DMA请求

    DMA_Cmd(DMA1_Channel6, ENABLE); //正式允许DMA
}

/**
 * @brief  USART GPIO 配置初始化
 * @param  无
 * @retval 无
 */
void USART2_DMA_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /** 打开串口GPIO的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

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

    /** 配置串口的工作参数 */
    /** 配置波特率 */
    USART_InitStructure.USART_BaudRate = USART_BAUDRATE_115200;
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

    /** 使能串口 */
    USART_Cmd(USART2, ENABLE);

    DMA6_Init();
}
//#endif

void USART2_DMA_Receive_Handle(void)
{
    uint16_t i;
    uint16_t count=0;

    UART2_Timeout++;

    count=DMA_GetCurrDataCounter(DMA1_Channel6);
    DMA_ClearITPendingBit(DMA1_IT_GL6); //清除全部中断标志

    if(count!=uart2_Flag_last)  //未完成传输
    {
        UART2_Timeout=0;
        uart2_Flag_last=count;
    }
    else
    {
			
        if(UART2_Timeout>UART2_TimeoutComp)  //产生超时
        {
            if(count<USART_DMA_BUFFER_SIZE) //有数据接收到
            {
                UART2_FlagTemp=USART_DMA_BUFFER_SIZE-count;  //得到接收到的字节数

                for(i=0; i<UART2_FlagTemp; i++) //将数据拷贝到缓冲区
                {
                    uart2_data[i]=uart2_data_temp[i];
                }
                UART2_Flag=UART2_FlagTemp;

                DMA_ClearFlag(DMA1_FLAG_TC6);
                DMA_Cmd(DMA1_Channel6, DISABLE); //正式允许DMA
                DMA6_Init();
            }
            UART2_Timeout=0;
        }
    }
}
void USART2_DMA_Receive_Init(void)
{
    USART2_DMA_Config();
    TIMER_Start(TIMER2,20,TIMER_PRECISION_MS,USART2_DMA_Receive_Handle);
}
/**
  * @brief  串口发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART2_DMA_Send(const uint8_t* const pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, pData[i]);
    }
}
/*********************************************END OF FILE**********************/

