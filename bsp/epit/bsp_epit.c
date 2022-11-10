#include "bsp_epit.h"

void epit_init(void)
{
	/*Clear CR register*/
	EPIT1->CR = 0x00000000;	

	EPIT1->CMPR = 0x00000000;

	//设置1分频
	EPIT1->CR |= (0x0);

	EPIT1->CR |= 0x0100000e;

//	if (epit_mode == SET_AND_FORGET)
//		EPIT1->CR |= 0x01c2000e;
//	else if (epit_mode == FREE_RUNNING)
//		EPIT1->CR |= 0x01c20006;
//	else
//		return;


#if 0
	/*Disable EPIT*/
	EPIT1->CR &= ~(0x1 << 0);

	/*Counter starts counting from the value it had when it was disabled.*/
	EPIT1->CR &= ~(0x1 << 1);

	/* enables the generation of interrupt on occurrence of compare event.*/
	EPIT1->CR |= (0x1 << 2);


	/*EPIT is out of reset*/
	EPIT1->CR &= ~(0x1 << 16);

	/*When this bit is set, all writes to the load register overwrites the counter contents and the counter starts
subsequently counting down from the programmed value.
*/
	EPIT1->CR |= (0x1 << 17);

	/*Inactive in debug mode*/
	EPIT1->CR &= ~(0x1 << 18);

	/*EPIT is disabled in wait mode*/
	EPIT1->CR &= ~(0x1 << 19);
	
	/*EPIT is disabled in stop mode*/
	EPIT1->CR &= ~(0x1 << 20);

	/*Set output pin*/
	EPIT1->CR |= (0x3 << 22);

	/*Select clock source*/
	EPIT1->CR &= ~(0x3 << 24);
	EPIT1->CR |= (0x1 << 24);

#endif
	/*Set CR register
	 * @bit[0]	0	Disable EPIT
	 * @bit[1]	1	Counter starts counting from load value
	 * @bit[2]	1	Compare interrupt enabled
	 * @bit[3]	1	Set-and-forget mode	
	 * @bit[15-4]	0x000	Divede by 1
	 * @bit[16]	0	EPIT is out of reset
	 * @bit[17]	0	
	 * @bit[18]	0	Inactive in debug mode
	 * @bit[19]	0	EPIT is disabled in wait mode
	 * @bit[20]	0	reservede
	 * @bit[21]	0	EPIT is disabled in stop mode
	 * @bit[23-22]	0x0	
	 * @bit[25-24]	0x1	Select clock source
	 * @bit[31-26]	000000	Reserved
	 * */
        GIC_EnableIRQ(EPIT1_IRQn);//使能EPIT1_OUT

	system_register_irqhandler(EPIT1_IRQn,  epit1_out_irqhandler, NULL);

}

void enable_epit(void)
{
	EPIT1->CR |= 0x1;
}

void disable_epit(void)
{
	EPIT1->CR &= ~(0x1);
}

void start_epit(int sec)
{
	EPIT1->LR = (sec * 66000000);
	enable_epit();
}

void stop_epit(void)
{
	disable_epit();
}

void epit1_out_irqhandler(unsigned int giccIar, void *param)
{
//	if(EPIT1->SR & (1 << 0)) {
	disable_epit();
		EPIT1->SR |= (1 << 0);
		beep_switch(ON);
		led_switch(ON);
		delay(500);
		led_switch(OFF);
		beep_switch(OFF);
//	}
//	EPIT1->SR |= (1 << 0);
}

