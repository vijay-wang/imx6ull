#ifndef __BSP_ICM20608_H
#define __BSP_ICM20608_H

#include "imx6ul.h"
#include "bsp_spi.h"
#include "bsp_gpio.h"

#define ICM20608_CSN(n)    (n ? gpio_pinwrite(GPIO1, 20, 1) : gpio_pinwrite(GPIO1, 20, 0))
#define ICM20608G_ID	0xaf
#define ICM20608D_ID	0xae


enum ICM20608_ADDR {
	SMPLRT_DIV	= 0x19,
	CONFIG          = 0x1a,
	GYRO_CONFIG 	= 0x1b,
	ACCEL_CONFIG 	= 0x1c,
	ACCEL_CONFIG2 	= 0x1d,
	LP_MODE_CFG	= 0x1e,
	FIFO_EN         = 0x23,
	ACCEL_XOUT_H	= 0x3b,
	ACCEL_XOUT_L	= 0x3c,
	ACCEL_YOUT_H	= 0x3d,
	ACCEL_YOUT_L	= 0x3e,
	ACCEL_ZOUT_H	= 0x3f,
	ACCEL_ZOUT_L	= 0x40,
	TEMP_OUT_H  	= 0x41,
	TEMP_OUT_L  	= 0x42,
	GYRO_XOUT_H 	= 0x43,
	GYRO_XOUT_L 	= 0x44,
	GYRO_YOUT_H 	= 0x45,
	GYRO_YOUT_L 	= 0x46,
	GYRO_ZOUT_H 	= 0x47,
	GYRO_ZOUT_L 	= 0x48,
	PWR_MGMT_1	= 0x6b,
	PWR_MGMT_2	= 0x6c,
	WHO_AM_I	= 0x75,
};

struct gyro_accel_data {
	int x;
	int y;
	int z;
};

void icm20608_init(void);

void icm20608_write_reg(unsigned char reg, unsigned char value);

unsigned char icm20608_read_reg(unsigned char reg);

void icm20608_read_len(unsigned char reg, unsigned char *buf, unsigned char len);

float icm20608_gyro_sensitivityget(void);

unsigned short icm20608_accel_sensitivityget(void);

void icmgyro_get_data(void);

void icmaccel_get_data(void);

void icm_getdata(struct gyro_accel_data *data, enum ICM20608_ADDR gyro_accel_addr);

void display_fs_div_sens(void);

#endif
