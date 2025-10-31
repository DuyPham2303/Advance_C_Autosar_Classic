	.file	"ex1.c"
	.text
	.globl	_a
	.section .rdata,"dr"
	.align 4
_a:
	.space 4
	.globl	_str
LC0:
	.ascii "hello\0"
	.data
	.align 4
_str:
	.long	LC0
	.globl	_str1
	.section .rdata,"dr"
	.align 4
_str1:
	.ascii "hello\0"
	.globl	_data1
	.data
	.align 4
_data1:
	.long	1
	.align 4
_data2:
	.long	2
	.globl	_static_data1
	.bss
	.align 4
_static_data1:
	.space 4
	.globl	_static_data2
	.align 4
_static_data2:
	.space 4
.lcomm _static_data3,4,4
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB122:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$16, %esp
	call	___main
	movl	$0, 8(%esp)
	movl	$0, 12(%esp)
	movl	$1, (%esp)
	movl	$2, 4(%esp)
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE122:
	.def	___main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (MinGW-W64 i686-ucrt-posix-dwarf, built by Brecht Sanders, r2) 14.2.0"
