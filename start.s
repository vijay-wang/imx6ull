.global _start

.global _bss_start
_bss_start:
	.word __bss_start

.global _bss_end
_bss_end:
	.word __bss_end

_start:
	/*设置处理器进入SVC模式*/
	mrs r0, cpsr
	bic r0, r0, #0x1f	@将低五位清零
	orr r0, r0, #0x12	@设置低五位
	msr cpsr, r0		@将svc模式写入cpsr寄存器

	/*清除bss段*/
	ldr r0, _bss_start
	ldr r1, _bss_end
	mov r2, #0x0
bss_loop:
	stmia r0!, {r2}
	cmp r0, r1
	ble bss_loop


	/*设置sp指针*/
	ldr sp, =0x80200000
	b main			@跳转到main函数	


