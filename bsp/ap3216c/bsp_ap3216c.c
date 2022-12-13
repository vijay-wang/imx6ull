#include "bsp_ap3216c.h"
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
	mdelay(50);
	ap3216c_writeonebyte(AP3216C_ADDR, AP3216_SYSTEMCONF, 0x03); /*开启ALS，PS，IR模块*/

	data = ap3216c_readonebyte(AP3216C_ADDR, AP3216_SYSTEMCONF);
	if (data == 0x03) {
		printf("Initialization successful\r\n");
		return 0; /*ap3216c初始化正常*/
	} else {
		printf("Initialization faild\r\n");
		return 1; /*ap3216c初始化失败*/
	}
	return data;
}

unsigned char ap3216c_writeonebyte(unsigned char addr, unsigned char reg,
		unsigned char data)
{
	unsigned char status = 0;	
	unsigned char wdata = data;	
	struct i2c_transfer wxfer;

	wxfer.direction = KI2C_Write;
	wxfer.slaveAddress = addr;
	wxfer.subaddress = reg;
	wxfer.subaddressSize = 1;
	wxfer.data = &wdata;
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
	int i = 0;

	/*循环读取所有传感器数据*/
	for (i = 0; i < 6; i++) {
		buf[i] = ap3216c_readonebyte(AP3216C_ADDR, AP3216_IRDATALOW + i);
	}

	*ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0x3);

	*als = ((unsigned short)buf[3] << 8) |buf[2];

	/* ps datalow[IR_OF] 或 ps data high[IR_OF]则ps数据无效
	 * If the IR is high intensity, it will affect the PS data and cause the 
	 * PS data invalid. There is an overflow flag (IR_OF) to indicate the 
	 * PS data to see if it is valid or not in high IR light. If the IR_OF
	 * is set to 1, the device will force the PS object status as away state.. */
	if ((buf[4] & 0x4) || (buf[4] & 0x4)) /*ps数据无效*/
		*ps = 0;
	else
		*ps = (((unsigned short)buf[5] & 0x3f) << 4) | (buf[4] & 0xf);


}

