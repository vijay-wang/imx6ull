.global _start


_start:
	/*Reference:ARM Cortex-A Series Programmer’s Guide P165 Table11-1*/
	ldr pc, =Reset_Handler		/* 复位中断 					*/	
	ldr pc, =Undefined_Handler	/* 未定义中断 					*/
	ldr pc, =SVC_Handler		/* SVC(Supervisor)中断 		*/
	ldr pc, =PrefAbort_Handler	/* 预取终止中断 					*/
	ldr pc, =DataAbort_Handler	/* 数据终止中断 					*/
	ldr pc, =NotUsed_Handler	/* 未使用中断					*/
	ldr pc, =IRQ_Handler		/* IRQ中断 					*/
	ldr pc, =FIQ_Handler		/* FIQ(快速中断)未定义中断 			*/


Reset_Handler:
	cpsid i //关中断
	
        mrc     p15, 0, r0, c1, c0, 0     /* 读取CP15的C1寄存器到R0中,此时c1寄存器用作SCTRL寄存器 reference:Cortex-A7 MPCore Technical Reference Manual P108 */
        bic     r0,  r0, #(0x1 << 12)     /* 清除C1寄存器的bit12位(I位)，关闭I Cache                    */
        bic     r0,  r0, #(0x1 <<  2)     /* 清除C1寄存器的bit2(C位)，关闭D Cache                               */
        bic     r0,  r0, #0x2             /* 清除C1寄存器的bit1(A位)，关闭对齐                                          */
        bic     r0,  r0, #(0x1 << 11)     /* 清除C1寄存器的bit11(Z位)，关闭分支预测                                     */
        bic     r0,  r0, #0x1             /* 清除C1寄存器的bit0(M位)，关闭MMU                                   */
        mcr     p15, 0, r0, c1, c0, 0     /* 将r0寄存器中的值写入到CP15的C1寄存器中    

	/*设置中断向量表偏移，相对于0x00000000*/
	isb 				/*Instructions sychronization barrier*/
	ldr r0, =0x87800000
	mcr p15, 0, r0, c12, c0, 0	/*reference:Cortex-A7 MPCore Technical Reference Manual P67. c12 is Vector Base Adress Register(VBAR)*/	
	isb				

	/*CPSR可以用来设置处理器模式 Reference:ARM Cortex-A Series Programmer’s Guide P47*/
	/*ARM Processor Mode   reference:ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition P1139*/
	/*Set sp of irq Mode*/
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x12
	msr cpsr, r0
	ldr sp, =0x80200000

	/*Set sp of Supervisor Mode(SVC)*/
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x13
	msr cpsr, r0
	ldr sp, =0x80400000
		
	/*Set sp of System Mode*/
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x1f
	msr cpsr, r0
	ldr sp, =0x80600000

	cpsie i //开中断
#if 0
.global _bss_start
_bss_start:
	.word __bss_start

.global _bss_end
_bss_end:
	.word __bss_end

	/*清除bss段*/
	ldr r0, _bss_start
	ldr r1, _bss_end
	mov r2, #0x0
bss_loop:
	stmia r0!, {r2}
	cmp r0, r1
	ble bss_loop
#endif
	b main


Undefined_Handler:
	ldr r0, =Undefined_Handler
	bx r0

SVC_Handler:
	ldr r0, =SVC_Handler
	bx r0

PrefAbort_Handler:
	ldr r0, =PrefAbort_Handler
	bx r0

DataAbort_Handler:
	ldr r0, =DataAbort_Handler
	bx r0

NotUsed_Handler:
	ldr r0, =NotUsed_Handler
	bx r0

IRQ_Handler:
	push {lr}                                       /* 保存lr地址 */
        push {r0-r3, r12}                       /* 保存r0-r3，r12寄存器 */

        mrs r0, spsr                            /* 读取spsr寄存器 */
        push {r0}                                       /* 保存spsr寄存器 */

        mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C0寄存器内的值到R1寄存器中
                                                                * 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
                                                                * Cortex-A7 Technical ReferenceManua.pdf P68 P138
                                                                */
        add r1, r1, #0X2000                     /* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
        ldr r0, [r1, #0XC]                      /* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器， GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据 * 这个中断号来绝对调用哪个中断服务函数 */
        push {r0, r1}                           /* 保存r0,r1 */

        cps #0x13                                       /* 进入SVC模式，允许其他中断再次进去 */

        push {lr}                                       /* 保存SVC模式的lr寄存器 */
        ldr r2, =system_irqhandler      /* 加载C语言中断处理函数到r2寄存器中*/
        blx r2                                          /* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中 */

        pop {lr}                                        /* 执行完C语言中断服务函数，lr出栈 */
        cps #0x12                                       /* 进入IRQ模式 */
        pop {r0, r1}
        str r0, [r1, #0X10]                     /* 中断执行完成，写EOIR */

        pop {r0}
        msr spsr_cxsf, r0                       /* 恢复spsr */

        pop {r0-r3, r12}                        /* r0-r3,r12出栈 */
        pop {lr}                                        /* lr出栈 */
        subs pc, lr, #4                         /* 将lr-4赋给pc */


FIQ_Handler:
	ldr r0, =FIQ_Handler
	bx r0
