#include "imx6ul.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_beep.h"
#include "bsp_key0.h"

int main(void)
{
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();		/* 初始化led 			*/
	beep_init();		/* 初始化蜂鸣器 			*/
	key0_init();



	while(1)			/* 死循环 				*/
	{	
		delay(10);

		int key_val = key0_getval();

		if (key_val == KEY_DOWN) {
			led_switch(ON);		/* 打开LED 			*/
			beep_switch(ON);
		} else if (key_val == KEY_UP) {
			led_switch(OFF);		/* 关闭LED 			*/
			beep_switch(OFF);
		}
	}

	return 0;
}
