#include "bsp_icm20608.h"
#include "bsp_delay.h"
#include "bsp_spi.h"
#include "stdio.h"
#include "bsp_delay.h"

void icm20608_init(void)
{
	unsigned char regvalue;
	gpio_pin_config_t cs_config;
	/*1、SPI引脚的初始化*/	

	/* 1、ESPI3 IO初始化 
 	 * ECSPI3_SCLK 	-> UART2_RXD
 	 * ECSPI3_MISO 	-> UART2_RTS
 	 * ECSPI3_MOSI	-> UART2_CTS
 	 */
	IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);
	IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);
	IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0);
	
	/* 配置SPI   SCLK MISO MOSI IO属性	
	 *bit 16: 0 HYS关闭
	 *bit [15:14]: 00 默认100K下拉
	 *bit [13]: 0 keeper功能
	 *bit [12]: 1 pull/keeper使能 
	 *bit [11]: 0 关闭开路输出
 	 *bit [7:6]: 10 速度100Mhz
 	 *bit [5:3]: 110 驱动能力为R0/6
	 *bit [0]: 1 高转换率
 	 */
	IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10b1);
	IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10b1);
	IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10b1);

	/*设置片选,设置软件片选，不要设置硬件片选*/
	IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);
	IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0x10b0);
	cs_config.direction = GPIO_OUTPUT;
	cs_config.output_logic = 0;
	gpio_init(GPIO1, 20, &cs_config);

	/*2、SPI控制器的初始化*/
	spi_init(ECSPI3);
 	icm20608_write_reg(PWR_MGMT_1, 0x80); 
	mdelay(50);
 	icm20608_write_reg(PWR_MGMT_1, 0x01); 
	mdelay(50);

	regvalue = icm20608_read_reg(WHO_AM_I);
	printf("icm20608 id = %#X\r\n", regvalue);

	/*3、IMC20608初始化*/
	icm20608_write_reg(SMPLRT_DIV, 0x00);     /* 输出速率是内部采样率 */
	icm20608_write_reg(GYRO_CONFIG, 0x18);    /* 陀螺仪±2000dps量程 */
	icm20608_write_reg(ACCEL_CONFIG, 0x18);   /* 加速度计±16G量程 */
	icm20608_write_reg(CONFIG, 0x04);         /* 陀螺仪低通滤波BW=20Hz */
	icm20608_write_reg(ACCEL_CONFIG2, 0x04);  /* 加速度计低通滤波BW=21.2Hz */
	icm20608_write_reg(PWR_MGMT_2, 0x00);     /* 打开加速度计和陀螺仪所有轴 */
	icm20608_write_reg(LP_MODE_CFG, 0x00);    /* 关闭低功耗 */
	icm20608_write_reg(FIFO_EN, 0x00);        /* 关闭FIFO   */
}

void icm20608_write_reg(unsigned char reg, unsigned char value)
{	
	reg &= ~0x80;

   	ICM20608_CSN(0); /* 使能SPI传输 */

	spich0_readwrite_byte(ECSPI3, reg);
	spich0_readwrite_byte(ECSPI3, value);

   	ICM20608_CSN(1);
}

/* The first byte contains the SPI Address, and the following 
 * byte(s) contain(s) the SPI data. The first bit of the first 
 * byte contains the Read/Write bit and indicates the Read (1) 
 * or Write (0) operation. The following 7 bits contain the 
 * Register Address*/
unsigned char icm20608_read_reg(unsigned char reg)
{
	unsigned char reg_val;

	reg |= 0x80;
	
   	ICM20608_CSN(0); /* 使能SPI传输 */

	spich0_readwrite_byte(ECSPI3, reg);
	reg_val = spich0_readwrite_byte(ECSPI3, 0xff);

	ICM20608_CSN(1);

	return reg_val;
}

void icm20608_read_len(unsigned char reg, unsigned char *buf, unsigned char len)
{
	int i = 0;

	reg |= 0x80;
	
   	ICM20608_CSN(0); /* 使能SPI传输 */

	spich0_readwrite_byte(ECSPI3, reg);

	/*连续读取多个字节*/
	for (i = 0; i < len; i++) {
		buf[i] = spich0_readwrite_byte(ECSPI3, 0xff);
	}

	ICM20608_CSN(1);
}

float icm20608_gyro_sensitivityget(void)
{
	unsigned char full_scale = 0;
	float gyro_sens = 0;
	
	full_scale = icm20608_read_reg(GYRO_CONFIG);

	switch (full_scale) {
		case 0:	
			gyro_sens = 131;
			break;
		case 1:	
			gyro_sens = 65.5;
			break;
		case 2:	
			gyro_sens = 32.8;
			break;
		case 3:	
			gyro_sens = 16.4;
			break;
	}

	return gyro_sens;
}

unsigned short icm20608_accel_sensitivityget(void)
{
	unsigned char full_scale = 0;
	unsigned short accel_sens = 0;
	
	full_scale = icm20608_read_reg(ACCEL_CONFIG);

	switch (full_scale) {
		case 0:	
			accel_sens = 16384;
			break;
		case 1:	
			accel_sens = 8192;
			break;
		case 2:	
			accel_sens = 4096;
			break;
		case 3:	
			accel_sens = 2048;
			break;
	}

	return accel_sens;
}

/*参数传入gyro的寄存器地址GYRO_XOUT_H或者传入
 * accel的寄存器地址ACCEL_XOUT_H*/
void icm_getdata(struct gyro_accel_data *data, enum ICM20608_ADDR gyro_accel_addr)
{
	unsigned char buf[6] = { 0 };
	int i = 0;

//	for (i = 0; i < 6; i++)
//		buf[i] = icm20608_read_reg(gyro_accel_addr + i);

	icm20608_read_len(gyro_accel_addr,buf , 6);

	data->x = ((short)buf[0] << 8) | buf[1];
	data->y = ((short)buf[2] << 8) | buf[3];
	data->z = ((short)buf[4] << 8) | buf[5];
}

void icmgyro_get_data(void)
{
	struct gyro_accel_data gyro_data;		
	float gyro_sens = icm20608_gyro_sensitivityget();

	icm_getdata(&gyro_data, GYRO_XOUT_H);

	printf("gyro_x: %d\r\n", ((int)(gyro_data.x) / gyro_sens) * 100);
	printf("gyro_y: %d\r\n", ((int)(gyro_data.y) / gyro_sens) * 100);
	printf("gyro_z: %d\r\n", ((int)(gyro_data.z) / gyro_sens) * 100);
}

void icmaccel_get_data(void)
{
	
	struct gyro_accel_data accel_data;		
	unsigned short accel_sens = icm20608_accel_sensitivityget();

	icm_getdata(&accel_data, ACCEL_XOUT_H);

	printf("accel_x:%d\r\n", ((int)(accel_data.x) / accel_sens ) * 100);
	printf("accel_y:%d\r\n", ((int)(accel_data.y) / accel_sens ) * 100);
	printf("accel_z:%d\r\n", ((int)(accel_data.z) / accel_sens ) * 100);
}

void display_fs_div_sens(void)
{
	icmgyro_get_data();
	icmaccel_get_data();
}
