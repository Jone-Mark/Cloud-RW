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
#include "TM1637.h"

/** Write multiple bits in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 */
void TM1637_Delay_us(unsigned  int Nus)
{
    for(; Nus>0; Nus--)
    {
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
    }
}
//#define TM1637_CLK_Low()    GPIO_ResetBits(GPIOC,GPIO_Pin_2)
//#define TM1637_CLK_High()   GPIO_SetBits(GPIOC,GPIO_Pin_2)
//#define TM1637_DIO_Low()    GPIO_ResetBits(GPIOC,GPIO_Pin_3)
//#define TM1637_DIO_High()   GPIO_SetBits(GPIOC,GPIO_Pin_3)
void TM1637_Start(void)
{
    TM1637_DIO_OUT();
    TM1637_CLK = 1;
    TM1637_DIO = 1;
    delay_us(2);
    TM1637_DIO = 0;
}
void TM1637_Ack(void)
{
    TM1637_DIO_IN();
    TM1637_CLK = 0;
    TM1637_Delay_us(5);
    while(TM1637_READ_DIO);
    TM1637_CLK = 1;
    delay_us(2);
    TM1637_CLK = 0;
}
void TM1637_Stop(void)
{
    TM1637_DIO_OUT();
    TM1637_CLK = 0;
    delay_us(2);
    TM1637_DIO = 0;
    delay_us(2);
    TM1637_CLK = 1;
    delay_us(2);
    TM1637_DIO = 1;
}
void TM1637_WriteByte(unsigned char oneByte)
{
    unsigned char i;
    TM1637_DIO_OUT();
    for(i=0; i<8; i++)
    {
        TM1637_CLK = 0;
        if(oneByte&0x01)
        {
            TM1637_DIO = 1;
        }
        else
        {
            TM1637_DIO = 0;
        }
        delay_us(3);
        oneByte=oneByte>>1;
        TM1637_CLK = 1;
        delay_us(3);
    }
}

unsigned char TM1637_ScanKey(void)
{
    unsigned char reKey,i;
    TM1637_Start();
    TM1637_WriteByte(0x42);
    TM1637_Ack();
    TM1637_DIO = 1;
    TM1637_DIO_IN();
    for(i=0; i<8; i++)
    {
        TM1637_CLK = 0;
        reKey=reKey>>1;
        delay_us(30);
        TM1637_CLK = 1;
        if(TM1637_READ_DIO)
        {
            reKey=reKey|0x80;
        }
        else
        {
            reKey=reKey|0x00;
        }
        delay_us(30);
    }
    TM1637_Ack();
    TM1637_Stop();
    return(reKey);
}
void TM1637_NixieTubeDisplay(unsigned char number)
{
    unsigned char i;
    TM1637_Start();
    TM1637_WriteByte(0x40);
    TM1637_Ack();
    TM1637_Stop();
    TM1637_Start();
    TM1637_WriteByte(0xC0);
    TM1637_Ack();

    for(i=0; i<4; i++)
    {
        TM1637_WriteByte(number);
        TM1637_Ack();
    }
    TM1637_Stop();
    TM1637_Start();
    TM1637_WriteByte(0x8f);
    TM1637_Ack();
    TM1637_Stop();
}

void TM1637_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}



