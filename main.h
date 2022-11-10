#ifndef __MAIN_H
#define __MAIN_H

/*定义要使用的始终寄存器*/
#define CCM_CCGR0			*((volatile unsigned int *)0x020C4068)
#define CCM_CCGR1			*((volatile unsigned int *)0x020C406c)
#define CCM_CCGR2			*((volatile unsigned int *)0x020C4070)
#define CCM_CCGR3			*((volatile unsigned int *)0x020C4074)
#define CCM_CCGR4			*((volatile unsigned int *)0x020C4078)
#define CCM_CCGR5			*((volatile unsigned int *)0x020C407c)
#define CCM_CCGR6			*((volatile unsigned int *)0x020C4080)

/*定义IOMUX相关寄存器*/
#define SW_MUX_CTL_PAD_GPIO1_IO03	*((volatile unsigned int *)0x020e0068)
#define SW_PAD_CTL_PAD_GPIO1_IO03	*((volatile unsigned int *)0x020e02f4)


/*定义GPIO1相关寄存器*/
#define GPIO1_GDIR			*((volatile unsigned int *)0x0209c004)
#define GPIO1_DR			*((volatile unsigned int *)0x0209c000)

#endif
