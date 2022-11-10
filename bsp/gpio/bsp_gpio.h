#ifndef __GPIO_H
#define __GPIO_H
#include "imx6ul.h"

/*引脚输入或输出*/
typedef enum {
	GPIO_INPUT,
	GPIO_OUTPUT
} gpio_pin_direction_t;

/*GPIO中断触发类型*/
typedef enum {
	GPIO_NOINT_NODE,
	GPIO_INT_LOW_LEVEL,
	GPIO_INT_HIGH_LEVEL,
	GPIO_INT_RISING_EDGE,
	GPIO_INT_FALLING_EDGE,
	GPIO_INT_RISING_OR_FALLING_EDGE
} gpio_interrupt_mode_t;

/*GPIO电气属性*/
typedef struct {
	gpio_pin_direction_t direction;
	uint8_t output_logic;
	gpio_interrupt_mode_t interrupt_mode;
} gpio_pin_config_t;

void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
void gpio_pinwrite(GPIO_Type *base, int pin, int val);
void gpio_intconfig(GPIO_Type *base, int pin, gpio_interrupt_mode_t pin_int_mode);
void gpio_enableint(GPIO_Type *base, int pin);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_clearintflags(GPIO_Type *base, unsigned int pin);

#endif
