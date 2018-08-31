/******************************Copyright (c)***********************************
*                Jiangsu Zhihai Electronic Technology Co., Ltd.
*                      Research & Development Department
*
*													www.smarthealth-tech.com
*
-------------------------------------------------------------------------------
* @file    TM1637.h
* @author  GU DONGDONG
* @date    2015-11-25
*******************************************************************************/
#ifndef TM1637_H
#define TM1637_H

#include "bsp.h"

#define TM1637_CLK           PCout(2)
#define TM1637_DIO           PCout(3)
#define TM1637_READ_DIO      PCin(3)

//IO·½ÏòÉèÖÃ
#define TM1637_DIO_IN()     {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=8<<12;}
#define TM1637_DIO_OUT()    {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=3<<12;}


void TM1637_Delay_us(unsigned  int Nus);
void TM1637_Start(void);
void TM1637_Ack(void);
void TM1637_Stop(void);
void TM1637_WriteByte(unsigned char oneByte);
unsigned char TM1637_ScanKey(void);
void TM1637_NixieTubeDisplay(unsigned char number);

void TM1637_Init(void);
void TM1637LED_Init(void);

typedef enum
{
    TM1637_0=0x3f,
    TM1637_1=0x06,
    TM1637_2=0x5b,
    TM1637_3=0x4f,
    TM1637_4=0x66,
    TM1637_5=0x6d,
    TM1637_6=0x7d,
    TM1637_7=0x07,
    TM1637_8=0x7f,
    TM1637_9=0x6f,

    TM1637_NUM_MAX,
} NUMBER_NAME;

#endif
