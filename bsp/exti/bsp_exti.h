#ifndef __EXTI_H
#define __EXTI_H
#include "imx6ul.h"
#include "bsp_int.h"

void exti_init(void);
void gpio1_io18_irqhandler(unsigned int giccIar, void *param);
void system_register_irqhandler(IRQn_Type irq, sys_irq_handler handler, void *param);


#endif
