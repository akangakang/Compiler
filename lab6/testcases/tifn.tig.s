	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
.L10:
	movq %rdi, -8(%rbp)
	movq $5, %rax
	movq %rax, -16(%rbp)
	movq $-16, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $2, %rsi
	movq %rbp, %rdi
	call L1
	movq $-16, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call printi


	leave
	ret

.section .rodata
.L8:
.string "\001\000\000\000\012"
	.text
	.globl L1
	.type L1, @function
L1:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L12:
	movq %rdi, -8(%rbp)
	movq $1, %rax
	movq $3, %rcx
	cmp %rcx, %rsi
	jg .L2
.L3:
	movq $0, %rax
.L2:
	movq $0, %r11
	cmp %r11, %rax
	jne .L5
.L6:
	movq $4, %r10
	movq $-8, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rax
	movq %r10, -16(%rax)
	movq $0, %rax
.L7:
	jmp .L11
.L5:
	leaq .L4(%rip), %rdi
	call print
	jmp .L7
.L11:


	leave
	ret

.section .rodata
.L4:
.string "\024\000\000\000hey! Bigger than 3!\012"
