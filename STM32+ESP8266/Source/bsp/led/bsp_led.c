/**
  ******************************************************************************
  * @file    bsp_led.c
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "./led/bsp_led.h"

/**
 * @brief  LED GPIO 配置初始化
 * @retval None
 */
void LED_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( LED0_GPIO_CLK|LED1_GPIO_CLK|LED2_GPIO_CLK|NET_LED_GPIO_CLK, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    // 改变制定管脚的映射GPIO_Remap_SWJ_Disable SWJ 完全禁用(JTAG+SW-DP)
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    // 改变制定管脚的映射GPIO_Remap_SWJ_JTAGDisable ,JTAG-DP 禁用+ SW-DP 使能

    GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = NET_LED_GPIO_PIN;
    GPIO_Init(NET_LED_GPIO_PORT, &GPIO_InitStructure);

    /** 关闭所有led灯	*/
    GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN);
    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_SetBits(NET_LED_GPIO_PORT, NET_LED_GPIO_PIN);
}

void SetLedConnectNet(LEDNetworkStatus status)
{
	switch(status)
	{
		case LED_NETWORK_OK:
			NET_LED_ON;
		break;
		case LED_NETWORK_NOK:
		  NET_LED_OFF;
		break;
		default:
		break;
	}
}

/*********************************************END OF FILE**********************/


