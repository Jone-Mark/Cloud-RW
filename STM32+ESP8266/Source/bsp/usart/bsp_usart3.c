/**
  ******************************************************************************
  * @file    bsp_usart.c
	* @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   DMA无中断接收串口2数据
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "bsp_usart3.h"

QueueHandle_t g_USART3_Queue; //RX数据接受消息队列

void USART3_IRQHandler(void)                	//串口1中断服务程序
{
    uint8_t Res;
    BaseType_t xHigherPriorityTaskWoken;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
        xQueueSendFromISR(g_USART3_Queue,&Res,&xHigherPriorityTaskWoken);
    }
}

void USART3_GPIO_Config(uint32_t usart_baudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART1，GPIOb时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOb时钟
    USART_DeInit(USART3);  //复位串口1
    //USART3_TX   Pb.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //Pb1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化Pb10

    //USART3_RX	  Pb.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化Pb11

    //Usart3 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=BSP_USART3_RX_NVIC_PRIORITY ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = usart_baudRate;//一般设置为;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_Even;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口
    USART_ClearFlag(USART3, USART_FLAG_TC);
}

int UART3_SendOneByte(int ch)
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);
    USART_SendData(USART3,(uint8_t)ch);
    return ch;
}
QueueHandle_t USART3_Get_Queue(void)
{
    return g_USART3_Queue;
}
void USART3_Init(uint32_t usart_baudRate)
{
    g_USART3_Queue=xQueueCreate(5,sizeof(uint8_t));
    USART3_GPIO_Config(usart_baudRate);
}

/*********************************************END OF FILE**********************/

