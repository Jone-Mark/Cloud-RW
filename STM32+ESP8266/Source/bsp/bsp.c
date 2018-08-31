/**
  ******************************************************************************
  * @file    common.c
  * @author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   This file contains all common init.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/**
  * @brief  读取芯片ID号
  * @param  存放ID号的数组地址
  * @param  芯片ID号字符串长度
  * @retval None
  */
void ReadChipID(char* cpId, int len)
{
    uint32_t ChipId[3] = {0};

    ChipId[0] = *(uint32_t *)(0x1ffff7e8);
    ChipId[1] = *(uint32_t *)(0x1ffff7ec);
    ChipId[2] = *(uint32_t *)(0x1ffff7f0);

    snprintf(cpId, len, "%08x%08x%08x", ChipId[0], ChipId[1], ChipId[2]);
}

/**
  * @brief  打印所有任务详情
  * @param  None
  * @retval None
  */
void COMMON_Show_Info(void)
{
    char InfoBuffer[256];
    vTaskList(InfoBuffer);				//获取所有任务的信息
    PRINTF_LOG("\r\n-------------------------------------------\r\n");
    PRINTF_LOG("Name\t\tState\tPrior\tStack\tNum\r\n");
    PRINTF_LOG("-------------------------------------------\r\n");
    PRINTF_LOG("%s",InfoBuffer);
    PRINTF_LOG("-------------------------------------------\r\n");
    PRINTF_LOG("   ** Usable RAM size is %d Bytes **\r\n", xPortGetFreeHeapSize());
    PRINTF_LOG("-------------------------------------------\r\n");
}

/**
  * @brief  总初始化
  * @param  None
  * @retval None
  */
void BSP_Init(void)
{
    /** 优先分组4 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    /** 初始化DWT */
    bsp_InitDWT();
    /** 串口初始化 */
    USART1_Init(BSP_USART_BAUDRATE_115200);
	USART2_Init(BSP_USART_BAUDRATE_115200,NULL,NULL);

    /** 按键/任务信息*/
    KEY_Init(COMMON_Show_Info,ReconfigWIFI);
    PRINTF_LOG("\r\n===========================================\r\n");
    PRINTF_LOG("FreeRTOS Version : %s\r\n", tskKERNEL_VERSION_NUMBER);
    PRINTF_LOG("Compile Time : %s  %s\r\n", __DATE__, __TIME__);
    PRINTF_LOG("===========================================\r\n");
	LED_Config();
	LED0_ON;
	LED1_ON;
	LED2_ON;
	NET_LED_ON;
	//IIC_Init();
}

/*********************************************END OF FILE**********************/

