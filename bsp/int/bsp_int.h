#ifndef __BSP_INT_H
#define __BSP_INT_H
#include "imx6ul.h"
#include "core_ca7.h"

typedef void (*sys_irq_handler)(unsigned int giccIar, void *param);

typedef struct  _sys_irq_handler {
	sys_irq_handler irq_hdl;
	void *param;		
} sys_irq_handler_t;

sys_irq_handler_t irq_table[NUMBER_OF_INT_VECTORS];

void int_init(void);
void system_irqtable_init(void);
void system_register_irqhandler(IRQn_Type irq, sys_irq_handler handler, void *param);
void default_irqhandler(unsigned int giccIar, void *param);
void system_irqhandler(unsigned int giccIar, void *param);

#endif
