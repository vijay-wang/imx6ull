.global _start

_start:
	/*设置处理器进入SVC模式*/
	mrs r0, cpsr
	bic r0, r0, #0x1f	@将低五位清零
	orr r0, r0, #0x12	@设置低五位
	msr cpsr, r0		@将svc模式写入cpsr寄存器


	/*设置sp指针*/
	ldr sp, =0x80200000
	b main			@跳转到main函数	


