#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "imx6ul.h"
extern int led_state;
/*
 * @description	: 初始化LED对应的GPIO
 * @param 		: 无
 * @return 		: 无
 */
void led_init(void);

void led_switch(const int status);

#endif
