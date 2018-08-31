/**
  ******************************************************************************
  * @file    bsp_gpio.c
	* @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   DMA无中断接收串口2数据
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "./gpio/bsp_gpio.h"

#define GPIO_Pin_Uart2_Reset  GPIO_Pin_3
#define GPIO_Pin_Uart2_Boot   GPIO_Pin_1

void GPIO_BSP_U2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Uart2_Reset|GPIO_Pin_Uart2_Boot;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC,GPIO_Pin_Uart2_Reset|GPIO_Pin_Uart2_Boot);
}
void GPIO_BSP_Init(void)
{
    GPIO_BSP_U2_Config();
}

/*********************************************END OF FILE**********************/

