#include "bsp_ap3216.h"
#include "stdio.h"
#include "cc.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"

unsigned char ap3216c_init(void)
{
	unsigned char data = 0;	

	/*最后一位SOIN设置为1的作用是：在GPIO口配置为
	 * 输出的情况下，仍然可以通过GPIO pad status 
	 * register (PSR)读取当前IO口的电平。*/
	IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);
	IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);

	IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0x70B0);
	IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0X70B0);

	i2c_init(I2C1); /* 初始化I2C1 */

	/*初始化ap3216c*/
	ap3216c_writeonebyte(AP3216C_ADDR, AP3216_SYSTEMCONF, 0x04); /*复位AP3216C1*/
	mdelay(10);

	ap3216c_writeonebyte(AP3216C_ADDR, AP3216_SYSTEMCONF, 0x03); /*开启ALS，PS，IR模块*/

	data = ap3216c_readonebyte(AP3216C_ADDR, AP3216_SYSTEMCONF);
	if (data == 0x03) {
		printf("Initialization successful\n");
		return 0; /*ap3216c初始化正常*/
	} else {
		printf("Initialization faild\n");
		return 1; /*ap3216c初始化失败*/
	}
}

unsigned char ap3216c_writeonebyte(unsigned char addr, unsigned char reg,
		unsigned char data)
{
	unsigned char status = 0;	
	struct i2c_transfer wxfer;

	wxfer.direction = KI2C_Write;
	wxfer.slaveAddress = addr;
	wxfer.subaddress = reg;
	wxfer.subaddressSize = 1;
	wxfer.data = &data;
	wxfer.dataSize = 1;

	if (i2c_master_transfer(I2C1, &wxfer))
		status = 1;

	return status;
}

unsigned char ap3216c_readonebyte(unsigned char addr, unsigned char reg)
{
	unsigned char data;
	struct i2c_transfer rxfer;

	rxfer.direction = KI2C_Read;
	rxfer.slaveAddress = addr;
	rxfer.subaddress = reg;
	rxfer.subaddressSize = 1;
	rxfer.data = &data;
	rxfer.dataSize = 1;

	i2c_master_transfer(I2C1, &rxfer);

	return data;
}

void ap3216c_readdata(unsigned short *ir, unsigned short *ps, unsigned short *als)
{
	unsigned char buf[6];	

	/*循环读取所有传感器数据*/
	for (int i = 0; i < 6; i++) {
		buf[i] = ap3216c_readonebyte(AP3216C_ADDR, AP3216_IRDATALOW + i);
	}

	if (buf[0] & 0x80) /*IR_OF位为1, 则数据无效*/
		*ir = 0;
	else
		*ir = 0;
}
