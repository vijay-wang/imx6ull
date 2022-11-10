#include "bsp_int.h"

//extern sys_irq_handler_t irq_table[NUMBER_OF_INT_VECTORS];
static unsigned int irq_nesting;

void int_init(void)
{
	GIC_Init();
	system_irqtable_init();
}


void system_irqtable_init(void)
{
	for (int i = 0; i < NUMBER_OF_INT_VECTORS; i++) {
		system_register_irqhandler((IRQn_Type)i, default_irqhandler, NULL);
	}

}

void system_register_irqhandler(IRQn_Type irq, sys_irq_handler handler, void *param)
{
	irq_table[irq].irq_hdl = handler;
	irq_table[irq].param = param;
}

/*中断处理函数*/
void system_irqhandler(unsigned int giccIar, void *param)
{
	int32_t int_num = giccIar & 0x3ff; //获取中断号
	
	if (int_num > NUMBER_OF_INT_VECTORS) {
		return;
	}

	irq_nesting++; //中断嵌套计数器加1

	irq_table[int_num].irq_hdl(int_num, irq_table[int_num].param);

	irq_nesting--;
}

void default_irqhandler(unsigned int giccIar, void *param)
{
	while (1);
}
