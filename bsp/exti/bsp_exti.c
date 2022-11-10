#include "bsp_exti.h"
#include "bsp_gpio.h"
#include "bsp_beep.h"
#include "bsp_led.h"
#include "bsp_delay.h"

void exti_init(void)
{
	gpio_pin_config_t key_config;

	/*设置gpio复用*/
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);

	/*初始化GPIO为中断模式*/
	key_config.direction = GPIO_INPUT;
	key_config.output_logic = 1;
	key_config.interrupt_mode = GPIO_INT_FALLING_EDGE;
	gpio_init(GPIO1, 18, &key_config);

	GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);//使能gpio18（中断id为67+32）reference:IMX6ULL参考手册 chapter3.2 Cortex A7 interrupts

	/*使能interrupt之前，必须要先注册中断处理函数，假如先使能中断，再注册函数，那么在注册函数之前可能中断就会到来，那么就会出现错误*/
	system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_io18_irqhandler, NULL);

	gpio_enableint(GPIO1, 18); //使能gpio，通过操作GPIOx_IMR寄存器
}

void gpio1_io18_irqhandler(unsigned int giccIar, void *param)
{
	start_epit(0.01);
	gpio_clearintflags(GPIO1, 18);//处理完中断以后必须清理中断，不然中断会一直到来
}
