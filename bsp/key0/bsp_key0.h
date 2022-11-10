#ifndef __KEY0_H
#define __KEY0_H

#include "imx6ul.h"

enum key0 {
	KEY_DOWN,
	KEY_UP
};

void key0_init(void);

const int read_key(void);

const int key0_getval(void);

#endif
