	.file	"ex1.c"
	.text
	.globl	_str
	.section .rdata,"dr"
LC0:
	.ascii "hello\0"
	.data
	.align 4
_str:
	.long	LC0
	.globl	_a
	.section .rdata,"dr"
	.align 4
_a:
	.long	21
	.globl	_data_1
	.data
	.align 4
_data_1:
	.long	21
	.align 4
_data_2:
	.long	21
	.globl	_bss_1
	.bss
	.align 4
_bss_1:
	.space 4
	.globl	_bss_2
	.align 4
_bss_2:
	.space 4
.lcomm _bss_3,4,4
.lcomm _bss_4,4,4
	.globl	_str1
	.data
	.align 4
_str1:
	.ascii "hello\0"
	.globl	_str2
	.section .rdata,"dr"
	.align 4
_str2:
	.ascii "hello\0"
	.globl	_str3
LC1:
	.ascii "world\0"
LC2:
	.ascii "ute\0"
	.data
	.align 4
_str3:
	.long	LC0
	.long	LC1
	.long	LC2
	.section .rdata,"dr"
LC3:
	.ascii "%s\0"
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
	movl	$32, _data_3.0
	movl	$LC1, _str
	movl	_str, %eax
	movl	%eax, 4(%esp)
	movl	$LC3, (%esp)
	call	_printf
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE122:
	.data
	.align 4
_data_3.0:
	.long	10
	.def	___main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (MinGW-W64 i686-ucrt-posix-dwarf, built by Brecht Sanders, r2) 14.2.0"
	.def	_printf;	.scl	2;	.type	32;	.endef
