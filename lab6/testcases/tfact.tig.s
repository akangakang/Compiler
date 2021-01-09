	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L6:
	movq %rdi, -8(%rbp)
	movq $10, %rsi
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
.L8:
	movq %rbx, -16(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $0, %rax
	cmp %rax, %rsi
	je .L2
.L3:
	movq %rsi, %rbx
	movq $1, %rax
	subq %rax, %rsi
	movq $-8, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rcx
	movq %rbx, %rax
	imulq %rcx, %rax
.L4:
	movq -16(%rbp), %rbx #spill
	jmp .L7
.L2:
	movq $1, %rax
	jmp .L4
.L7:


	leave
	ret

