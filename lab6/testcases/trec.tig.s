	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
.L2:
	movq %r15, -16(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $16, %rdi
	call allocRecord
	movq $3, %r15
	movq %r15, 0(%rax)
	movq $4, %rdx
	movq $1, %rcx
	movq $8, %rsi
	imulq %rsi, %rcx
	movq %rax, %r15
	addq %rcx, %r15
	movq %rdx, (%r15)
	movq %rax, %r15
	movq $0, %r11
	movq $8, %r10
	imulq %r10, %r11
	movq %r15, %rax
	addq %r11, %rax
	movq (%rax), %rdi
	call printi
	movq $1, %r11
	movq $8, %r10
	imulq %r10, %r11
	addq %r11, %r15
	movq (%r15), %rdi
	call printi
	movq -16(%rbp), %r15 #spill


	leave
	ret

