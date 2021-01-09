	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L4:
	movq %rdi, -8(%rbp)
	movq $2, %rsi
	movq %rbp, %rdi
	call L1
	movq %rax, %rdi
	call printi


	leave
	ret

	.text
	.globl L1
	.type L1, @function
L1:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
.L6:
	movq %rdi, -8(%rbp)
	movq %rsi, -16(%rbp)
	movq $3, %rsi
	movq %rbp, %rdi
	call L2


	leave
	ret

	.text
	.globl L2
	.type L2, @function
L2:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L8:
	movq %rdi, -8(%rbp)
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rax
	movq $-16, %rcx
	addq %rcx, %rax
	movq (%rax), %rax
	addq %rsi, %rax


	leave
	ret

