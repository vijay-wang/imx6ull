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

int main(void)
{

	int_init();
	imx6u_clkinit();
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();		/* 初始化led 			*/
	beep_init();		/* 初始化蜂鸣器 			*/
	//key0_init();
	exti_init();
	//epit_init();
	key_filter_init();

	while(1)			/* 死循环 				*/
	{	
	}

	return 0;
}
