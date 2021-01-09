	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L10:
	movq %rdi, -8(%rbp)
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
.L12:
	movq %rdi, -8(%rbp)
	movq $56, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $23, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $71, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $72, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $173, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $181, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $281, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $659, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $729, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $947, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print
	movq $945, %rsi
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %rdi
	call L1
	movq %rax, %rdi
	call printi
	leaq .L8(%rip), %rdi
	call print


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
.L14:
	movq %rdi, -8(%rbp)
	movq $1, %r10
	movq $2, %r11
	movq $2, %r9
movq %rsi, %rax
cqto
idivq %r9
	movq %rax, %rcx
	cmp %rcx, %r11
	jg .L3
.L6:
movq %rsi, %rax
cqto
idivq %r11
	imulq %r11, %rax
	cmp %rsi, %rax
	je .L4
.L5:
	cmp %rcx, %r11
	je .L3
.L7:
	movq $1, %r9
	addq %r9, %r11
	jmp .L6
.L4:
	movq $0, %r10
.L3:
	movq %r10, %rax


	leave
	ret

