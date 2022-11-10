#ifndef __MAIN_H
#define __MAIN_H

#define CCM_CCGR_BASE				(0x020c4068) 
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_BASE	(0x020e005c) 
#define IOMUXC_SW_PAD_CTL_PAD_GPIO1_BASE	(0x020e02e8) 
#define GPIOx_BASE				(0x0209c000) 

#define CCM					((CCM_CCGR_TYPE *)CCM_CCGR_BASE)
#define IOMUXC_MUX				((IOMUXC_SW_MUX_CTL_PAD_GPIO1_TYPE *)IOMUXC_SW_MUX_CTL_PAD_GPIO1_BASE)
#define IOMUXC_PAD				((IOMUXC_SW_PAD_CTL_PAD_GPIO1_TYPE *)IOMUXC_SW_PAD_CTL_PAD_GPIO1_BASE)
#define GPIOx					((GPIOx_TYPE *)GPIOx_BASE)


/*定义要使用的时钟寄存器*/
typedef struct {
	volatile unsigned int CCM_CCGR0; 
	volatile unsigned int CCM_CCGR1;
	volatile unsigned int CCM_CCGR2;
	volatile unsigned int CCM_CCGR3;
	volatile unsigned int CCM_CCGR4;
	volatile unsigned int CCM_CCGR5;
	volatile unsigned int CCM_CCGR6;
} CCM_CCGR_TYPE;


/*定义IOMUX相关寄存器*/
typedef struct {
	volatile unsigned int GPIO1_IO00;
	volatile unsigned int GPIO1_IO01;
	volatile unsigned int GPIO1_IO02;
	volatile unsigned int GPIO1_IO03;
	volatile unsigned int GPIO1_IO04;
	volatile unsigned int GPIO1_IO05;
	volatile unsigned int GPIO1_IO06;
	volatile unsigned int GPIO1_IO07;
	volatile unsigned int GPIO1_IO08;
	volatile unsigned int GPIO1_IO09;
} IOMUXC_SW_MUX_CTL_PAD_GPIO1_TYPE;


typedef struct {
	volatile unsigned int GPIO1_IO00;
	volatile unsigned int GPIO1_IO01;
	volatile unsigned int GPIO1_IO02;
	volatile unsigned int GPIO1_IO03;
	volatile unsigned int GPIO1_IO04;
	volatile unsigned int GPIO1_IO05;
	volatile unsigned int GPIO1_IO06;
	volatile unsigned int GPIO1_IO07;
	volatile unsigned int GPIO1_IO08;
	volatile unsigned int GPIO1_IO09;
} IOMUXC_SW_PAD_CTL_PAD_GPIO1_TYPE;




/*定义GPIO1相关寄存器*/
typedef struct {
	volatile unsigned int GPIOx_DR;
	volatile unsigned int GPIOx_GDIR;
	volatile unsigned int GPIOx_PSR;
	volatile unsigned int GPIOx_ICR1;
	volatile unsigned int GPIOx_ICR2;
	volatile unsigned int GPIOx_IMR;
	volatile unsigned int GPIOx_ISR;
	volatile unsigned int GPIOx_EDGE_SEL;
} GPIOx_TYPE[5];

#endif
