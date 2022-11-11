#include "key_filter.h"


void key_filter_init(void)
{
	/*Clear CR register*/
	EPIT1->CR = 0x00000000;	

	EPIT1->CMPR = 0x00000000;

	//设置1分频
//	EPIT1->CR |= (0x0);

	EPIT1->CR |= 0x0100000e;

        GIC_EnableIRQ(EPIT1_IRQn);//使能EPIT1_OUT

	system_register_irqhandler(EPIT1_IRQn, key_filter_irqhandler, NULL);
}

void key_filter_irqhandler(unsigned int giccIar, void *param)
{
	if (EPIT1->SR & 0x1) {
		stop_epit();
		if (gpio_pinread(GPIO1, 18) == 0) {
			if (led_state == OFF)
				led_switch(ON);
			else
				led_switch(OFF);

			if (beep_state == OFF)
				beep_switch(ON);
			else
				beep_switch(OFF);

		}

	}
	EPIT1->SR |= 1;
}

