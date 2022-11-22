#include "imx6ul.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_beep.h"
#include "bsp_key0.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_gpio.h"
#include "bsp_epit.h"
#include "key_filter.h"
#include "bsp_uart.h"
#include "bsp_ap3216c.h"
#include "stdio.h"

int main(void)
{
	int a,b;

	int_init();
	imx6u_clkinit();
	clk_enable();		/* 使能所有的时钟 			*/
	delay_init();
	led_init();		/* 初始化led 			*/
	beep_init();		/* 初始化蜂鸣器 			*/
	key0_init();
	exti_init();
	epit_init();
	key_filter_init();
	uart_init();
	ap3216c_init();


	while(1)			/* 死循环 				*/
	{	
		printf("please enter two numbers that sepated by space:");
		scanf("%d %d", &a, &b);
		printf("\r\na + b = %d\r\n", a + b);
	}

	return 0;
}
