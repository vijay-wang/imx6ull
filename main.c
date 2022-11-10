#include "main.h"

/*使能时钟*/
void clk_enable(void)
{
	CCM_CCGR0 = 0xffffffff;
	CCM_CCGR1 = 0xffffffff;
	CCM_CCGR2 = 0xffffffff;
	CCM_CCGR3 = 0xffffffff;
	CCM_CCGR4 = 0xffffffff;
	CCM_CCGR5 = 0xffffffff;
	CCM_CCGR6 = 0xffffffff;
}

/*初始化LED*/
void led_init(void)
{
	SW_MUX_CTL_PAD_GPIO1_IO03 = 0x00000005;
	SW_PAD_CTL_PAD_GPIO1_IO03 = 0x000010b0;

	GPIO1_GDIR = 0X00000008;
	GPIO1_DR = 0X00000000;
}

void delay_short(volatile unsigned int n)
{
	while (n--) {}
}

void delay(volatile unsigned int n)
{
	while (n--) {//主频396MHz的时候延时在1ms
		delay_short(0x7ff);
	}
}

void led_on(void)
{
	GPIO1_DR &= ~(1<<3);//bit3清零
}

void led_off(void)
{
	GPIO1_DR |= (1<<3);//bit3设置0
}

int main(void)
{
	/*初始化LED*/
	clk_enable();
	led_init();

	/*设置LED闪烁*/

	while (1) {
		led_on();
		delay(500);	
		led_off();
		delay(500);
	}

	return 0;
}
