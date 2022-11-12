#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H
#include "imx6ul.h"
#include "bsp_int.h"
#include "bsp_led.h"

/*
 * @description	: 短时间延时函数
 * @param - n	: 要延时循环次数(空操作循环次数，模式延时)
 * @return 		: 无
 */
void delay_short(volatile unsigned int n);


/*
 * @description	: 延时函数,在396Mhz的主频下
 * 			  	  延时时间大约为1ms
 * @param - n	: 要延时的ms数
 * @return 		: 无
 */

void delay(volatile unsigned int n);

void delay_init(void);

void  gpt1_irq_handler(unsigned int giccIar, void *param);

void mdelay(int msec);
void udelay(int usec);

#endif

