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

struct ap3216c_data {
	unsigned short ir;
	unsigned short als;
	unsigned short ps;
};

int main(void)
{
	struct ap3216c_data sensor_data;

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
		ap3216c_readdata(&sensor_data.ir, &sensor_data.ps, &sensor_data.als);
		printf("\r\nir:%hd", sensor_data.ir);
		printf("\r\nps:%hd", sensor_data.ps);
		printf("\r\nals:%hd", sensor_data.als);

		mdelay(1000);
	}

	return 0;
}
