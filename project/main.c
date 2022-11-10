#include "imx6ul.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_beep.h"
#include "bsp_key0.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_gpio.h"
extern int key0_state;

int main(void)
{
	int_init();
	imx6u_clkinit();
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();		/* 初始化led 			*/
	beep_init();		/* 初始化蜂鸣器 			*/
	key0_init();
	exti_init();



	while(1)			/* 死循环 				*/
	{	
//		led_switch(ON);
//		delay(5000);
//		led_switch(OFF);
//		delay(5000);
//		delay(10);
//
//		int key_val = key0_getval();
//
//		if (key_val == KEY_DOWN) {
//			led_switch(ON);		/* 打开LED 			*/
//			beep_switch(ON);
//		} else if (key_val == KEY_UP) {
//			led_switch(OFF);		/* 关闭LED 			*/
//			beep_switch(OFF);
//		}
	}

	return 0;
}
