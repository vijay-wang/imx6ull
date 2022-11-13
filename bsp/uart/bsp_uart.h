#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "imx6ul.h"
#include "bsp_gpio.h"

void uart_init(void);

void uart_io_init(void);

void putc(unsigned char c);

unsigned char getc(void);

//void puts(char *str);
void raise(int sig_nr);

#endif
