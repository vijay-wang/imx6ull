#ifndef __BSP__EPIT_H
#define __BSP__EPIT_H
#include "imx6ul.h"
#include "bsp_exti.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_beep.h"


typedef struct {
	int epit1_is_timeout;
	int sec;
} epit1_timer_t;

typedef enum {
	SET_AND_FORGET,
	FREE_RUNNING
} epit1_mode_t;

void epit_init(void);
void enable_epit(void);
void disable_epit(void);
void start_epit(int sec);
void stop_epit(void);
void epit1_out_irqhandler(unsigned int giccIar, void *param);

#endif
