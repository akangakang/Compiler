	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L6:
	movq %rdi, -8(%rbp)
	movq $4, %rdx
	movq $9, %rsi
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
	subq $8, %rsp
.L8:
	movq %rdi, -8(%rbp)
	movq %rsi, %rax
	cmp %rdx, %rax
	jg .L2
.L3:
	movq %rdx, %rax
.L4:
	jmp .L7
.L2:
	jmp .L4
.L7:


	leave
	ret

