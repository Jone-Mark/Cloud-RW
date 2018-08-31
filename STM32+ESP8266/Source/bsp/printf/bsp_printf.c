/**
  ******************************************************************************
  * @file    bsp_printf.c
  * @author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重定向C库函数printf到串口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "./printf/bsp_printf.h"

#ifdef PRINTF_ABLE

#ifdef MICROLIB_UNABLE
/** 加入以下代码,支持printf函数,而不需要选择use MicroLIB. */
#pragma import(__use_no_semihosting)
/** 标准库需要的支持函数. */
struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

/** Retargets the C library printf function to the USART. */
FILE __stdout;

/** Retargets the C library printf function to the USART. */
FILE __stdin;

/** 定义_sys_exit()以避免使用半主机模式. */
void _sys_exit(int x)
{
    x = x;
}
#endif

/** 重定向c库函数printf到串口，重定向后可使用printf函数. */
int fputc(int ch, FILE *f)
{
    return USART1_SendChar(ch);
}

/** 重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数. */
int fgetc(FILE *f)
{
    return USART1_GetChar();
}

/**
  * @brief  二进制的形式打印一个Buffer.
  * @param  str: 打印的buffer
  * @param  len: buffer的长度
  * @retval None
  */
void hexPrint(const unsigned char* buffer, int len)
{
    int i;
    for(i = 0; i < len; ++i)
    {
        if(i % 16 == 0)
        {
            if(i != 0)
            {
                printf("\r\n");
            }
            printf("%08x ", i);
        }
        printf(" %02x", buffer[i]);
    }
    printf("\r\n\r\n");
}

#endif

/*********************************************END OF FILE**********************/

