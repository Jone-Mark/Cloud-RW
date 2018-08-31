/**
  ******************************************************************************
  * @file    bsp_printf.h
  * @author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重定向C库函数printf到串口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_PRINTF_H_
#define __BSP_PRINTF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

	/** 使用PRINTF */
#define PRINTF_ABLE  	

#ifdef PRINTF_ABLE
	
#define PRINTF_DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#define PRINTF_LOG(format,...)  printf(format, ##__VA_ARGS__)
#define PRINTF_HEX(buffer, len)   if (LOG_LOCAL_LEVEL >= LOG_VERBOSE) { hexPrint(buffer, len);}

#else

#define PRINTF_DEBUG(format,...)     
#define PRINTF_LOG(format,...) 
#define PRINTF_HEX(buffer, len)

#endif

#ifdef __cplusplus
}
#endif

#endif /* __BSP_PRINTF_H_ */

/*********************************************END OF FILE**********************/

