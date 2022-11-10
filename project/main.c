#include "imx6ul.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_clk.h"

int main(void)
{
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/

	while(1)			/* 死循环 				*/
	{	
		led_off();		/* 关闭LED 			*/
		delay(10000);		/* 延时500ms 			*/

		led_on();		/* 打开LED 			*/
		delay(10000);		/* 延时500ms 			*/
	}

	return 0;
}
