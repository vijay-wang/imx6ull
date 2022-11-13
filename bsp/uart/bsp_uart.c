
#include "bsp_uart.h"

void uart_disable(UART_Type *base)
{
	base->UCR1 &= ~(1 << 0);
}

void uart_enable(UART_Type *base)
{
	base->UCR1 |= (1 << 0);
}

void uart_softreset(UART_Type *base)
{
	base->UCR2 &= ~(1 << 0); 
	while ((base->UCR2 & 0x1) == 0);
}

void uart_io_init(void)
{
        IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);                                               
        IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10b0);

        IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_RX, 0);                                               
        IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10b0);
}

void putc(unsigned char c)
{
	while (((UART1->USR2 >> 3) & 0x1) == 0);
	UART1->UTXD = c & 0xff;
}

void puts(char *str)
{
	char *p = str;
	while(*p)
		putc(*p++);
}

unsigned char getc(void)
{
	puts("begin=====");
	while ((UART1->USR2 & 0x1) == 0);
	puts("end=====");
	return UART1->URXD;
}

void uart_init(void)
{
	uart_io_init();

	uart_disable(UART1);
	uart_softreset(UART1);
	
	UART1->UCR1 = 0;

	UART1->UCR1 &= ~(1<<14);//关闭波特率自动检测

	UART1->UCR2 |= (1 << 14) | (1 << 5) | (1 << 2) | (1 << 1);

	UART1->UCR3 |= (1 << 2);

	//设置波特率
	/*
	 *		Ref Freq	
	 *Baud Rate = -------------------------
	 *			UBMR + 1
	 *		(16 * ------------)
	 *			UBIR + 1
	 */
	UART1->UFCR = 5 << 7;

	//必须先设置UBIR再设置UBMR
	UART1->UBIR = 71;

	UART1->UBMR = 3124;

	uart_enable(UART1);
}


