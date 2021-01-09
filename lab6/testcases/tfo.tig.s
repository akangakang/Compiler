	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
.L7:
	movq %r15, -16(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $4, %r15
	movq $0, %rsp
	cmp %r15, %rsp
	jg .L1
.L4:
	movq %rsp, %rdi
	call printi
	movq $3, %r11
	cmp %r11, %rsp
	je .L2
.L3:
	cmp %r15, %rsp
	je .L1
.L5:
	movq $1, %rax
	addq %rax, %rsp
	jmp .L4
.L2:
.L1:
	movq $0, %rax
	movq -16(%rbp), %r15 #spill


	leave
	ret

