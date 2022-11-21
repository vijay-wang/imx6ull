#ifndef __BSP_I2C_H
#define __BSP_I2C_H 
#include "imx6ul.h"

/*定义I2C相关状态宏*/
#define I2C_STATUS_OK			(0)
#define I2C_STATUS_BUSY			(1)
#define I2C_STATUS_IDLE			(2)
#define I2C_STATUS_NAK			(3)
#define I2C_STATUS_ARBITRATIONLOST	(4)
#define I2C_STATUS_TIMEOUT		(5)
#define I2C_STATUS_ADDRNAK		(6)

enum i2c_direction {
	KI2C_Write,
	KI2C_Read
};

struct i2c_transfer {
	unsigned char slaveAddress; /*七位从机地址*/	
	enum i2c_direction direction;
	unsigned int subaddress;
	unsigned int subaddressSize; /*寄存器地址长度*/
	unsigned char *volatile data; /*数据缓冲区*/	
	volatile unsigned int dataSize; /*缓冲区长度*/
};

void i2c_init(I2C_Type *base);

unsigned char i2c_master_start(I2C_Type *base, unsigned char addr, enum i2c_direction direction);

unsigned char i2c_master_stop(I2C_Type *base);

unsigned char i2c_master_repeated_start(I2C_Type *base, unsigned char addr,
					enum i2c_direction direction);

unsigned char i2c_check_and_clear_error(I2C_Type *base, unsigned int status);

void i2c_master_write(I2C_Type *base, const unsigned char *buf, unsigned int size);

void i2c_master_read(I2C_Type *base, unsigned char *buf, unsigned int size);

unsigned i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer);

void wait_for_xfer_complete(I2C_Type *base);

void clear_iff(I2C_Type *base);

#endif
