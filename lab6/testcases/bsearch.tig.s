	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
.L16:
	movq %rbx, -32(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $16, %r11
	movq %r11, -16(%rbp)
	movq $-24, %rax
	movq %rbp, %rbx
	addq %rax, %rbx
	movq $0, %rsi
	movq $-16, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	call initArray
	movq %rax, (%rbx)
	movq %rbp, %rdi
	call L4
	movq -32(%rbp), %rbx #spill


	leave
	ret

	.text
	.globl L4
	.type L4, @function
L4:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L18:
	movq %rdi, -8(%rbp)
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L2
	movq $7, %rcx
	movq $-8, %rdx
	movq %rbp, %rax
	addq %rdx, %rax
	movq (%rax), %rax
	movq $-16, %r11
	addq %r11, %rax
	movq (%rax), %rdx
	movq $1, %r11
	subq %r11, %rdx
	movq $0, %rsi
	movq $-8, %rdi
	movq %rbp, %rax
	addq %rdi, %rax
	movq (%rax), %rdi
	call L3
	movq %rax, %rdi
	call printi
	leaq .L5(%rip), %rdi
	call print


	leave
	ret

	.text
	.globl L3
	.type L3, @function
L3:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L20:
	movq %rdi, -8(%rbp)
	movq %rdx, %rdi
	cmp %rdi, %rsi
	je .L12
.L13:
	movq %rsi, %rax
	addq %rdi, %rax
	movq $2, %r8
cqto
idivq %r8
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %r11
	movq $-24, %rdx
	addq %rdx, %r11
	movq (%r11), %rdx
	movq $8, %r8
	movq %rax, %r11
	imulq %r8, %r11
	addq %r11, %rdx
	movq (%rdx), %rdx
	cmp %rcx, %rdx
	jl .L9
.L10:
	movq %rax, %rdx
	movq $-8, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	call L3
.L11:
	movq %rax, %rsi
.L14:
	movq %rsi, %rax
	jmp .L19
.L12:
	jmp .L14
.L9:
	movq %rdi, %rdx
	movq $1, %rdi
	movq %rax, %rsi
	addq %rdi, %rsi
	movq $-8, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	call L3
	jmp .L11
.L19:


	leave
	ret

	.text
	.globl L2
	.type L2, @function
L2:
	pushq %rbp
	movq %rsp, %rbp
	subq $24, %rsp
.L22:
	movq %rbx, -16(%rbp) #spill
	movq %r15, -24(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $0, %rbx
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %r15
	movq $-16, %r11
	addq %r11, %r15
	movq (%r15), %r15
	movq $1, %r11
	subq %r11, %r15
	cmp %r15, %rbx
	jg .L6
.L7:
	movq $2, %r10
	movq %rbx, %r11
	imulq %r10, %r11
	movq $1, %rax
	addq %rax, %r11
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rax
	movq $-24, %r10
	addq %r10, %rax
	movq (%rax), %r10
	movq $8, %r9
	movq %rbx, %rax
	imulq %r9, %rax
	addq %rax, %r10
	movq %r11, (%r10)
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	cmp %r15, %rbx
	je .L6
.L8:
	movq $1, %r11
	addq %r11, %rbx
	jmp .L7
.L6:
	movq $0, %rax
	movq -16(%rbp), %rbx #spill
	movq -24(%rbp), %r15 #spill


	leave
	ret

	.text
	.globl L1
	.type L1, @function
L1:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L24:
	movq %rdi, -8(%rbp)
	leaq .L5(%rip), %rdi
	call print


	leave
	ret

.section .rodata
.L5:
.string "\000\000\000\000"
