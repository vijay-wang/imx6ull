#include "bsp_i2c.h"

void i2c_init(I2C_Type *base)
{
	/*disable i2c block*/
	base->I2CR &= ~(0x1 << 7);

	/*set the baud rate to around 100K*/
	base->IFDR = 0x15;

	/*enable i2c block*/
	base->I2CR |= (0x1 << 7);
}

unsigned char i2c_master_start(I2C_Type *base, unsigned char addr, enum i2c_direction direction)
{
	if (base->I2SR & (0x1 << 5)) //i2c busy
		return 1;
	
	/*set i2c block to master and set the direction of master to write*/
	base->I2CR |= (0x1 << 5) | (0x1 << 4);

	/*产生start信号并传输地址和设定传输方向(start信号不用手动发送，在发送
	 * 地址的时候会自动同步发送)*/
	base->I2DR  = ((unsigned int)addr << 1) | (direction == KI2C_Read ? KI2C_Read : KI2C_Write );

	return 0;
}

unsigned char i2c_master_stop(I2C_Type *base)
{
	unsigned short timeout = 0xffff;
	/*清除bit[5-3]*/
	base->I2CR &= ~(0x7 << 3);

	/*等待忙结束*/
	while ((base->I2SR & (0x1 << 5))) {
		timeout--;
		if (timeout == 0)	
			return I2C_STATUS_TIMEOUT;
	}
	return I2C_STATUS_OK;	
}

unsigned char i2c_master_repeated_start(I2C_Type *base, unsigned char addr,
					enum i2c_direction direction)
{
	/*检查I2C是否忙或工作在从机模式下*/
	if (base->I2SR & (0x1 << 5) || (((base->I2CR) & (0x1 << 5)) == 0))
		return 1;

	/*设置发送，和发送repeat start*/
	base->I2CR |= (0x1 << 4) | (0x1 << 2);

        base->I2DR  = ((unsigned int)addr << 1) | (direction == KI2C_Read ? KI2C_Read : KI2C_Write );

	return I2C_STATUS_OK;
}

unsigned char i2c_check_and_clear_error(I2C_Type *base, unsigned int status)
{
	/*检查是否为仲裁丢失*/
	if (status & (0x1 << 4)) {
		base->I2SR &= ~(0x1 << 4);
		base->I2CR &= ~(0x1 << 4);
		base->I2SR |= (0x1 << 4);
		return I2C_STATUS_ARBITRATIONLOST;
	} else if (status & (0x1 << 0)) {
		return I2C_STATUS_NAK;
	}

	return I2C_STATUS_OK;
}


/*发送数据*/
void i2c_master_write(I2C_Type *base, const unsigned char *buf, unsigned int size)
{
	/*等待传输完成*/
	while (!(base->I2SR & (0x1 << 7)));

	base->I2CR &= ~(0x1 << 1); /*清除中断标志,手册写着有，传输1byte完成后会产生中断*/
	base->I2CR |= 0x1 << 4; /*Transmit/Receive mode select bit. 0 Receive，1 Transmit*/

	while (size--) {
		base->I2DR = *buf++;

		while (!(base->I2SR & (0x1 << 1))); /*等待传输完成*/
		base->I2CR &= ~(0x1 << 1); /*清除中断标志,手册写着有，传输1byte完成后会产生中断*/

		if(i2c_check_and_clear_error(base,base->I2SR)) /*检查ACK*/
			break;
	}

	base->I2SR &= ~(0x1 << 1); /*此处需要再次清理中断，因为跳出while循环的时候可能发生了仲裁丢失等
					情况，不一定是传输完成才产生的中断*/
	i2c_master_stop(base); /*发送停止信号*/

}

void i2c_master_read(I2C_Type *base, unsigned char *buf, unsigned int size)
{
	volatile int8_t dummy = 0;
	dummy++; /*防止编译报错*/

	while (!(base->I2SR & (0x1 << 7))); /*等待传输完成*/

	base->I2SR &= ~(0x1 << 1); /*清除中断挂起位*/
	base->I2CR &= ~(0x1 << 4); /*设置传输方向为Receive*/

	/* Writing TXAK applies only when the I2C bus is a receiver
	 * An acknowledge signal is sent to the bus at the ninth 
	 * clock bit after receiving one byte of data.
	 * 也就是打开ACK功能，每收到1 byte就发送一个ack*/
	base->I2CR &= ~(0x1 << 3);

	/*从机在接收倒数第二字节数据的时候就设置TXAK位为1，那么下一个字节发送之后就会发送NACK*/
	if (size ==1)
		base->I2CR |= (0x1 << 3);

	while (size--) {
		while (!(base->I2SR & (0x1 << 1))); /*等待传输完成*/
		base->I2SR &= ~(0x1 <<1); /*清除中断标志位*/

		if (size == 0) {
			i2c_master_stop(base);
		}

		if (size == 1) {
			base->I2CR |= (0x1 << 3);
		}
		
		*buf++ = base->I2DR;
	}
}

void wait_for_xfer_complete(I2C_Type *base)
{
	while(!(base->I2SR & (0X1 << 7)));	
}

void clear_iff(I2C_Type *base)
{
	base->I2SR &= ~(1 << 1);
}

unsigned i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer)
{
	unsigned char ret = 0;
	enum i2c_direction direction = xfer->direction;	

	base->I2SR &= ~((1 << 1) | (1 << 4)); /* 清除中断标志位和仲裁丢失位 */	

	wait_for_xfer_complete(base);

	/*如果是读的话，要先发送寄存器地址，所以要先将方向改为写*/
	if ((xfer->subaddressSize > 0) && (xfer->direction == KI2C_Read))
		direction = KI2C_Write;

	/*发送开始信号和slave设备地址*/
	ret = i2c_master_start(base, xfer->slaveAddress, direction);
	if (ret)
		return ret;

	/*等待传输完成*/
	while (!(base->I2SR & (0x1 << 1)));

	/*检查是否出现错误*/
	ret = i2c_check_and_clear_error(base, base->I2SR);
	if (ret) {
		i2c_master_stop(base);
		return ret;
	}

	/*发送寄存器地址*/
	if (xfer->subaddressSize) {
		do {
			clear_iff(base);	
			xfer->subaddressSize--;
			/*因为传输是以小端模式，所以要先发送高字节*/
			base->I2DR  = (xfer->subaddress >> (8 * xfer->subaddressSize));
			while (!(base->I2SR & (1 << 1)));
			              /* 检查是否有错误发生 */
			ret = i2c_check_and_clear_error(base, base->I2SR);
			if(ret)
			{
				i2c_master_stop(base); /* 发送停止信号 */
				return ret;
			}

		} while ((xfer->subaddressSize > 0) && (ret == I2C_STATUS_OK));
	}

 	/* 发送repeated_start和从机地址 */
	if(xfer->direction == KI2C_Read) {
		base->I2SR &= ~(1 << 1); /* 清除中断挂起位 */
		i2c_master_repeated_start(base, xfer->slaveAddress, KI2C_Read);
		while(!(base->I2SR & (1 << 1))); /* 等待传输完成 */

		/* 检查是否有错误发生 */
		ret = i2c_check_and_clear_error(base, base->I2SR);

		if(ret) {
			ret = I2C_STATUS_ADDRNAK;
			i2c_master_stop(base);          /* 发送停止信号 */
			return ret;
		}
	}


      /* 发送数据 */
      if ((xfer->direction == KI2C_Write) && (xfer->dataSize > 0))
      {
          i2c_master_write(base, xfer->data, xfer->dataSize);
      }

      /* 读取数据 */
      if ((xfer->direction == KI2C_Read) && (xfer->dataSize > 0))
      {
          i2c_master_read(base, xfer->data, xfer->dataSize);
      }

      return 0;
}

