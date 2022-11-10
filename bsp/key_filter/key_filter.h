#ifndef __KEY_FILTER_H
#define __KEY_FILTER_H
#include "imx6ul.h"
#include "bsp_exti.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_epit.h"


void key_filter_init(void);
void key_filter_irqhandler(unsigned int giccIar, void *param);

#endif
