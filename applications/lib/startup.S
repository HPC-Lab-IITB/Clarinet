
	.text
	.globl	_reset

_reset:
	// set stack pointer
	la	t0, _reset
	li	t1, 0xf0000
	add	sp, t0, t1

#if defined(__riscv_float_abi_double) || defined(__riscv_float_abi_single)
	// set mstatus.fs to initial
	li	t0, 0x2000
	csrs	mstatus, t0
#endif

	j _start

.pushsection .tohost,"aw",@progbits
.align 6; .global tohost; tohost: .dword 0
.popsection
