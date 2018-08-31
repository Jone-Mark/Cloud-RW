/* @version V1.0
* @date    2017-12-21
* @brief   esp8266初始化
******************************************************************************
*/
#include "string.h"
#include "stdio.h"
#include "esp8266_init.h"
#include "./delay/bsp_delay.h"
/**
******************************************************************************
* @brief  配置ESP8266 复位管脚
* @retval None.
****************************************************************************
**/
static void esp8266_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(ESP8266_EN_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = ESP8266_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ESP8266_EN_PORT, &GPIO_InitStructure);
}

/**
******************************************************************************
* @brief  复位ESP8266
* @retval None.
****************************************************************************
**/
static void esp8266_gpio_reset(void)
{
    uint32_t i;
    ESP8266_EN_INACTIVE;
	  DelayXms(1000);
	  DelayXms(1000);
	  DelayXms(1000);
    //for(i = 0; i<10000; i++);

    ESP8266_EN_ACTIVE;
    //for(i = 0; i<10000; i++);

}

void esp8266_init(void)
{
    esp8266_gpio_config();
    esp8266_gpio_reset();
}


