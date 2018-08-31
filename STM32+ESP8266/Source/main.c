/**
  ******************************************************************************
  * @file    main.c
  * @Author  Simic
  * @version V1.0
  * @date 	 2013-xx-xx
  * @brief   主函数.
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "sys.h"
#include "dht11.h" 
#include "delay.h"
#include "main.h"
#include "esp8266_init.h"
#include "esp8266_handler.h"
#include "mqtt.h"
#include "oled.h"
// 
//#include "oledfont.h" 
xQueueHandle xQueue_USART2_Cmd;                 /**< 串口2接收消息队列*/
u8 temperature;  	    
u8 humidity;    
extern unsigned char BMP2[];
extern unsigned char BMP1[];

void vCollectSensorTask(void *ptr)
{
    while (1)
    {
      //SHT20_GetValue();
		  DHT11_Read_Data(&temperature,&humidity);	
			OLED_ShowNum(100,3,humidity,3,16);//ÏÔÊ¾ASCII×Ö·ûµÄÂëÖµ 	
			OLED_ShowNum(30,3,temperature,3,16);//			
//		debug_logI("temperature:%.1f,humilty:%.1f",sht20Info.tempreture,sht20Info.humidity);
      /*向看门狗喂狗*/
      //xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_1);
      vTaskDelay(500 / portTICK_RATE_MS);
    }
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
    /** 设备初始化 */
	delay_init();	    	 
	DHT11_Init();
  BSP_Init();
	//Adc_Init();		
	//esp8266_init();
	OLED_Init();			
	OLED_Clear(); 

	OLED_ShowCHinese(36,0,0);//云
  OLED_ShowCHinese(54,0,1);//养
	OLED_ShowCHinese(72,0,2);//狗
	
	OLED_ShowCHinese(0,3,3);//温
  OLED_ShowCHinese(16,3,4);//度
	
	OLED_ShowCHinese(66,3,5);//湿
  OLED_ShowCHinese(82,3,4);//度


	OLED_ShowString(0,6,"LED",16);
	
	LED0_OFF;
	LED1_OFF;
	LED2_OFF;
	NET_LED_OFF;
	OLED_DrawBMP(30,6,46,8,BMP1);
	OLED_DrawBMP(50,6,66,8,BMP1);
	OLED_DrawBMP(70,6,86,8,BMP1);
	OLED_DrawBMP(90,6,106,8,BMP1);

    /* 创建串口接收消息队列 */
	xQueue_USART2_Cmd = xQueueCreate(1, 128);

	xTaskCreate( vAnalysisUartData,   	"vAnalysisUartData",  	128, NULL, configMAX_PRIORITIES - 2, NULL);	
	xTaskCreate( vEsp8266_Main_Task,   	"vEsp8266_Main_Task",  	128, NULL, configMAX_PRIORITIES - 3, NULL);
	xTaskCreate( vMQTT_Handler_Task,   	"vMQTT_Handler_Task",  	256, NULL, configMAX_PRIORITIES - 4, NULL);
	xTaskCreate( vMQTT_Recive_Task,   	"vMQTT_Recive_Task",  	128, NULL, configMAX_PRIORITIES - 5, NULL);
	xTaskCreate( MqttTranmitTask,		"MqttTranmitTask",		256, NULL, configMAX_PRIORITIES - 2, NULL);
	
	xTaskCreate( vCollectSensorTask,   	"vCollectSensorTask",  	128, NULL, configMAX_PRIORITIES - 4, NULL);
    /** 启动调度，开始执行任务 */
  vTaskStartScheduler();
}

/*********************************************END OF FILE**********************/


