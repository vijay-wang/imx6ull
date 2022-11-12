#include "bsp_delay.h"

void delay_init(void)
{
	GPT1->CR = 0;
	GPT1->CR |= (1 << 15);
	while ((GPT1->CR >> 15) & 0x1);

	GPT1->CR |= (1 << 1);//GPT counter value is reset to 0 when it is disabled.

	GPT1->CR |= (1 << 6);//Choose Peripheral Clock (ipg_clk) as clk source.

	GPT1->PR = 65;//

	GPT1->OCR[0] = 0xffffffff;

	GPT1->CR |= (0x1 << 0);
#if 0
	GPT1->OCR[0] = 1000000/2;//设置中断周期为500ms

	GPT1->IR = 1; //打开比较1中断

	GIC_EnableIRQ(GPT1_IRQn);
	system_register_irqhandler(GPT1_IRQn, gpt1_irq_handler, NULL);
	GPT1->CR |= (0x1 << 0);
#endif


}

void udelay(int usec)
{
	unsigned long old_usec, new_usec;
	old_usec = GPT1->CNT;

	do {
		new_usec = GPT1->CNT;
		if(new_usec < old_usec) {
			new_usec = new_usec + 0xffffffff - old_usec;
		}
	}
	while (new_usec - old_usec < usec);
}

void mdelay(int msec)
{
	udelay(msec * 1000);
}

#if 0
	GPT1->OCR[0] = 1000000/2;//设置中断周期为500ms
void  gpt1_irq_handler(unsigned int giccIar, void *param)
{
	if (GPT1->SR & (0x1 << 0)) {
		//比较中断1发生
		if (led_state == ON)
			led_switch(OFF);
		else
			led_switch(ON);
	}

	//clear interrupt bit
	GPT1->SR |= 0x1;
}
#endif

void delay_short(volatile unsigned int n)
{
	while(n--){}
}

void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}


