#include "bsp_spi.h"
#include "stdio.h"

void spi_init(ECSPI_Type *base)
{
	/* bit0:	1	使能ECSPI
	 * bit3:	1	当向TXFIFO写入数据以后立即开启SPI突发
	 * bit7-4	0001	SPI通道0主模式
	 * bit19-18	00	选中通道0
	 * bit31-20	0x7	突发长度为8bit*/
	base->CONREG = 0;
	base->CONREG |= (0x1 << 0) | (0x1 << 3) | (0x1 << 4) | (0x7 << 20);	

	/*                                                                                              
	 * ECSPI通道0设置,即设置CONFIGREG寄存器                                                             
	 * bit0:    0 通道0 PHA为0                                                                          
	 * bit4:    0 通道0 SCLK高电平有效                                                                  
	 * bit8:    0 通道0片选信号 当SMC为1的时候此位无效                                                  
	 * bit12：  0 通道0 POL为0                                                                          
	 * bit16：  0 通道0 数据线空闲时高电平                                                              
	 * bit20:   0 通道0 时钟线空闲时低电平                                                              
	 */                                                                                             
	base->CONFIGREG = 0;            /* 设置通道寄存器 */

	/*设置采样间隔，也就是每传输一个字节，需要插入多少个时钟*/
	base->PERIODREG = 0x2000;

	/*SPI时钟频率设为6MHz,clk_root为60MHz*/
	base->CONREG &= ~((0xf << 12) | (0xf << 8)); /*ECSPI时钟分两步分频*/
	base->CONREG |= (0x9 << 12);
}

unsigned char spich0_readwrite_byte(ECSPI_Type *base, unsigned char txdata)
{
	uint32_t spirxdata = 0;
	uint32_t spitxdata = txdata;
	
	/*选择通道0*/
	base->CONREG &= ~(0x3 << 18);
	base->CONREG |= (0x0 << 18);
	while((base->STATREG & (0x1 << 0)) == 0) {}/*等待发送FIFO为空*/
	base->TXDATA = spitxdata;	

	while((base->STATREG & (0x1 << 3)) == 0) {}/*等待接收FIFO有数据*/
	spirxdata = base->RXDATA;

	return spirxdata;
}
