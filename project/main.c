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
#include "bsp_icm20608.h"

/*开启浮点运算*/
void imx6ul_hardfpu_enable(void)
{

        uint32_t cpacr;
        uint32_t fpexc;

	/* 使能NEON和FPU */
        cpacr = __get_CPACR();
        cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk))
                   |  (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
	__set_CPACR(cpacr);
	fpexc = __get_FPEXC();
	fpexc |= 0x40000000UL;
	__set_FPEXC(fpexc);
}

int main(void)
{
	int_init();
	imx6u_clkinit();
	clk_enable();		/* 使能所有的时钟 			*/
	imx6ul_hardfpu_enable();
	delay_init();
	led_init();		/* 初始化led 			*/
	beep_init();		/* 初始化蜂鸣器 			*/
	key0_init();
	exti_init();
	epit_init();
	key_filter_init();
	uart_init();
	ap3216c_init();
	icm20608_init();



	while(1)			/* 死循环 				*/
	{	
		display_fs_div_sens();
		mdelay(2000);
	}

	return 0;
}
