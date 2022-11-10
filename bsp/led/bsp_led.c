#include "bsp_led.h"

/*
 * @description	: 初始化LED对应的GPIO
 * @param 		: 无
 * @return 		: 无
 */
void led_init(void)
{
	/* 1、初始化IO复用 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);		/* 复用为GPIO1_IO0 */

	/* 2、、配置GPIO1_IO03的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
     */
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0X10B0);

	/* 3、初始化GPIO,设置GPIO1_IO03设置为输出  */
	GPIO1->GDIR |= (1 << 3);	
	
	/* 4、设置GPIO1_IO03输出低电平，打开LED0 */
	GPIO1->DR |= (1 << 3);			
}

/*
 * @description	: 打开LED灯
 * @param 		: 无
 * @return 		: 无
 */

void led_switch(const int status)
{
	if (status == ON)
		GPIO1->DR &= ~(1<<3); 
	else if (status == OFF)
		GPIO1->DR |= (1<<3); 
}

