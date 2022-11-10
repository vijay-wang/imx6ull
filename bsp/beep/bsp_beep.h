#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "imx6ul.h"

extern int beep_state;

void beep_init(void);

void beep_switch(int status);

#endif
