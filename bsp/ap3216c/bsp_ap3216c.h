#ifndef __BSP_AP3216_H
#define __BSP_AP3216_H
#include "imx6ul.h"
#include "bsp_i2c.h"

#define AP3216C_ADDR			0x1e

#define AP3216_SYSTEMCONF		0x00	
#define AP3216_INTSTATUS		0x01
#define AP3216_INTCLEARMANNER		0x02
#define AP3216_IRDATALOW		0x0A
#define AP3216_IRDATAHIGH		0x0B
#define AP3216_ALSDATALOW		0x0C
#define AP3216_ALSDATAHIGH		0x0D
#define AP3216_PSDATALOW		0x0E
#define AP3216_PSDATAHIGH		0x0F

unsigned char ap3216c_init(void);

unsigned char ap3216c_writeonebyte(unsigned char addr, unsigned char reg,
		unsigned char data);

unsigned char ap3216c_readonebyte(unsigned char addr, unsigned char reg);

void ap3216c_readdata(unsigned short *ir, unsigned short *ps, unsigned short *als);

#endif
