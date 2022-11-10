#include "bsp_gpio.h"


void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
	base->IMR &= ~(0x1 << pin);//disable interrupt 

	if (config->direction == GPIO_INPUT) {
		base->GDIR &= ~(0x1 << pin);//设置引脚为输入
	} else {
		base->GDIR |= (0x1 << pin); //设置引脚为输出
		gpio_pinwrite(base, 18, config->output_logic); //设置默认输出高电平
	}

	gpio_intconfig(base, pin, config->interrupt_mode);
}


void gpio_pinwrite(GPIO_Type *base, int pin, int val)
{
	if (val == 0) {
		base->DR &= ~(0X00000001 << pin); //输出低电平
	} else {
		base->DR |= (0X00000001 << pin); //输出高电平
	}
}

void gpio_intconfig(GPIO_Type *base, int pin, gpio_interrupt_mode_t pin_int_mode)
{
	volatile uint32_t *icr;
	uint32_t icr_shift;

	icr_shift = pin;

	base->EDGE_SEL &= ~(0x1 << pin);

	if (pin < 16) {
		icr = &(base->ICR1);
	} else {
		icr = &(base->ICR2);
		icr_shift -= 16;
	}

	switch (pin_int_mode) {
		case GPIO_INT_LOW_LEVEL:
			*icr &= ~(0x3 << (2 * icr_shift));
			break;	

		case GPIO_INT_HIGH_LEVEL:
			*icr = ((*icr & (~(0x3 << (2 * icr_shift)))) | (0x1 << (2 * icr_shift)));
			break;	

		case GPIO_INT_RISING_EDGE:
			*icr = ((*icr | (0x3 << (2 * icr_shift))) & (~(0x1 << (2 * icr_shift)))); 
			break;	

		case GPIO_INT_FALLING_EDGE:
			*icr |= (0x3 << (2 * icr_shift));
			break;	

		case GPIO_INT_RISING_OR_FALLING_EDGE:
			base->EDGE_SEL |= (0x1 << pin);
			break;	
		default:
			break;
	}
}

void gpio_enableint(GPIO_Type *base, int pin)
{
	base->IMR |= (0x1 << pin);
}

int gpio_pinread(GPIO_Type *base, int pin)
{
	return (((base->DR) >> pin) & 0x1);
}

void gpio_clearintflags(GPIO_Type *base, unsigned int pin)
{
	base->ISR |= (0x1 << pin);
}
