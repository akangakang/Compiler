	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L7:
	movq %rdi, -8(%rbp)
	movq $10, %rsp
.L4:
	movq $0, %r11
	cmp %r11, %rsp
	jge .L5
.L1:
	movq $0, %rax
	jmp .L6
.L5:
	movq %rsp, %rdi
	call printi
	movq $1, %rax
	subq %rax, %rsp
	movq $2, %r11
	cmp %r11, %rsp
	je .L2
.L3:
	jmp .L4
.L2:
	jmp .L1
.L6:


	leave
	ret

