.global _start @全局标号

_start:
	/*使能所有外设时钟*/
	ldr r0, =0x020c4068 @CCGR0
	ldr r1, =0xffffffff
	str r1, [r0]

	ldr r0, =0x020c406c @CCGR1
	str r1, [r0]

	ldr r0, =0x020c4070 @CCGR2
	str r1, [r0]

	ldr r0, =0x020c4074 @CCGR3
	str r1, [r0]

	ldr r0, =0x020c4078 @CCGR4
	str r1, [r0]

	ldr r0, =0x020c407c @CCGR5
	str r1, [r0]

	ldr r0, =0x020c4080 @CCGR6
	str r1, [r0]

	/*复用GPIO1_IO03 PIN为GPIO，设置寄存器IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03值为5*/
	ldr r0, =0x020e0068
	ldr r1, =0x00000005
	str r1, [r0]

	/*设置GPIO1_IO03的电气属性，设置寄存器IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03*/
	/*
	*SRE 	bit0:	0 Slow Slew Rate
	*DSE 	bit5:3	110 R0/6 
	*SPEED	bit7:6	10 100MHz
	*ODE	bit11	0 关闭开路输出
	*PKE	bit12	1 Pull/Keeper Enable
	*PUE	bit13	0 
	*PUS	bit15:14 00 100K Ohm Pull Down 
	*HYS	bit16	0
	*/
	ldr r0, =0x020e02f4
	ldr r1, =0x000010b0
	str r1, [r0]

	/*设置GPIO功能
	*设置GPIO1_GDIR寄存器的第三位为1,表示PIN GPIO1_IO03输出
	*GPIOx_GDIR寄存器用于设置GPIO PIN为输入还是输出，一共有五个这样的寄存器
	*/
	ldr r0, =0x0209c004
	ldr r1, =0x00000008
	str r1, [r0]

	/*打开LED，也就是设置GPIO1_IO03为0
	*也就是设置GPIOX_DR的第三位为0,当GPIO1_IO03 PIN的输出信号为低电平的时候LED开启
	*/
@	ldr r0, =0x0209C000
@	ldr r1, =0x00000008
@	str r1, [r0]

loop:
	b loop

