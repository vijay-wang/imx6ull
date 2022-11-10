#include "imx6ull.h"

/*使能时钟*/
void clk_enable(void)
{
	CCM->CCM_CCGR0 = 0xffffffff;
	CCM->CCM_CCGR1 = 0xffffffff;
	CCM->CCM_CCGR2 = 0xffffffff;
	CCM->CCM_CCGR3 = 0xffffffff;
	CCM->CCM_CCGR4 = 0xffffffff;
	CCM->CCM_CCGR5 = 0xffffffff;
	CCM->CCM_CCGR6 = 0xffffffff;
}

/*初始化LED*/
void led_init(void)
{
	IOMUXC_MUX->GPIO1_IO03 = 0x00000005;
	IOMUXC_PAD->GPIO1_IO03 = 0x000010b0;

	GPIOx[1]->GPIOx_GDIR = 0X00000008;
	GPIOx[1]->GPIOx_DR = 0X00000000;
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
	GPIOx[1]->GPIOx_DR &= ~(1<<3);//bit3清零
}

void led_off(void)
{
	GPIOx[1]->GPIOx_DR |= (1<<3);//bit3设置0
}

int main(void)
{
	/*初始化LED*/
	clk_enable();
	led_init();

	/*设置LED闪烁*/

	while (1) {
		led_on();
		delay(5000);	
		led_off();
		delay(5000);
	}

	return 0;
}
