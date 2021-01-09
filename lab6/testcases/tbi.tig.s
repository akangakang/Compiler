	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
.L15:
	movq %rdi, -8(%rbp)
	movq $8, %rax
	movq %rax, -16(%rbp)
	movq %rbp, %rdi
	call L1


	leave
	ret

	.text
	.globl L1
	.type L1, @function
L1:
	pushq %rbp
	movq %rsp, %rbp
	subq $40, %rsp
.L17:
	movq %rbx, -40(%rbp) #spill
	movq %r12, -24(%rbp) #spill
	movq %r14, -16(%rbp) #spill
	movq %r15, -32(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $0, %r15
	movq $-8, %rax
	movq %rbp, %r14
	addq %rax, %r14
	movq (%r14), %r14
	movq $-16, %r12
	addq %r12, %r14
	movq (%r14), %r12
	movq $1, %r14
	subq %r14, %r12
	cmp %r12, %r15
	jg .L2
.L12:
	movq $0, %rbx
	movq $-8, %rax
	movq %rbp, %r14
	addq %rax, %r14
	movq (%r14), %r14
	movq $-16, %r11
	addq %r11, %r14
	movq (%r14), %r14
	movq $1, %r11
	subq %r11, %r14
	cmp %r14, %rbx
	jg .L3
.L9:
	cmp %rbx, %r15
	jg .L6
.L7:
	leaq .L5(%rip), %rdi
.L8:
	call print
	cmp %r14, %rbx
	je .L3
.L10:
	movq $1, %r11
	addq %r11, %rbx
	jmp .L9
.L6:
	leaq .L4(%rip), %rdi
	jmp .L8
.L3:
	leaq .L11(%rip), %rdi
	call print
	cmp %r12, %r15
	je .L2
.L13:
	movq $1, %r14
	addq %r14, %r15
	jmp .L12
.L2:
	leaq .L11(%rip), %rdi
	call print
	movq -40(%rbp), %rbx #spill
	movq -24(%rbp), %r12 #spill
	movq -16(%rbp), %r14 #spill
	movq -32(%rbp), %r15 #spill


	leave
	ret

.section .rodata
.L11:
.string "\001\000\000\000\012"
.section .rodata
.L5:
.string "\001\000\000\000y"
.section .rodata
.L4:
.string "\001\000\000\000x"
