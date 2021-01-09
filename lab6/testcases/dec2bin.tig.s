	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
.L8:
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
.L10:
	movq %rdi, -8(%rbp)
	movq $100, %rdi
	call printi
	leaq .L5(%rip), %rdi
	call print
	movq $100, %rsi
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rdi
	call L1
	leaq .L6(%rip), %rdi
	call print
	movq $200, %rdi
	call printi
	leaq .L5(%rip), %rdi
	call print
	movq $200, %rsi
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rdi
	call L1
	leaq .L6(%rip), %rdi
	call print
	movq $789, %rdi
	call printi
	leaq .L5(%rip), %rdi
	call print
	movq $789, %rsi
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rdi
	call L1
	leaq .L6(%rip), %rdi
	call print
	movq $567, %rdi
	call printi
	leaq .L5(%rip), %rdi
	call print
	movq $567, %rsi
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rdi
	call L1
	leaq .L6(%rip), %rdi
	call print


	leave
	ret

.section .rodata
.L6:
.string "\001\000\000\000\012"
.section .rodata
.L5:
.string "\004\000\000\000\011->\011"
	.text
	.globl L1
	.type L1, @function
L1:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
.L12:
	movq %rbx, -16(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq %rsi, %rbx
	movq $0, %rax
	cmp %rax, %rbx
	jg .L3
.L4:
	movq $0, %rax
	movq -16(%rbp), %rbx #spill
	jmp .L11
.L3:
	movq $2, %rcx
movq %rbx, %rax
cqto
idivq %rcx
movq %rax, %rsi
	movq $-8, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	call L1
	movq $2, %rcx
movq %rbx, %rax
cqto
idivq %rcx
	movq $2, %rcx
	imulq %rcx, %rax
	movq %rbx, %rdi
	subq %rax, %rdi
	call printi
	jmp .L4
.L11:


	leave
	ret

