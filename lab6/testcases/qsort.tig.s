	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
.L34:
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
	subq $24, %rsp
.L36:
	movq %rbx, -24(%rbp) #spill
	movq %r15, -16(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $-8, %r15
	movq %rbp, %rax
	addq %r15, %rax
	movq (%rax), %rdi
	call L2
	movq $-8, %r15
	movq %rbp, %rax
	addq %r15, %rax
	movq (%rax), %rax
	movq $-16, %rbx
	addq %rbx, %rax
	movq (%rax), %rdx
	movq $1, %rax
	subq %rax, %rdx
	movq $0, %rsi
	movq $-8, %r15
	movq %rbp, %rax
	addq %r15, %rax
	movq (%rax), %rdi
	call L3
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
	jg .L30
.L31:
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %r11
	movq $-24, %rax
	addq %rax, %r11
	movq (%r11), %rax
	movq $8, %rcx
	movq %rbx, %r11
	imulq %rcx, %r11
	addq %r11, %rax
	movq (%rax), %rdi
	call printi
	leaq .L5(%rip), %rdi
	call print
	cmp %r15, %rbx
	je .L30
.L32:
	movq $1, %r11
	addq %r11, %rbx
	jmp .L31
.L30:
	leaq .L5(%rip), %rdi
	call print
	movq -24(%rbp), %rbx #spill
	movq -16(%rbp), %r15 #spill


	leave
	ret

	.text
	.globl L3
	.type L3, @function
L3:
	pushq %rbp
	movq %rsp, %rbp
	subq $24, %rsp
.L38:
	movq %rbx, -24(%rbp) #spill
	movq %r15, -16(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq %rdx, %rbx
	movq %rsi, %r15
	movq %rbx, %rcx
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %r11
	movq $-24, %rax
	addq %rax, %r11
	movq (%r11), %r11
	movq $8, %rax
	movq %rsi, %r10
	imulq %rax, %r10
	addq %r10, %r11
	movq (%r11), %rax
	cmp %rbx, %rsi
	jl .L28
.L29:
	movq $0, %rax
	movq -24(%rbp), %rbx #spill
	movq -16(%rbp), %r15 #spill
	jmp .L37
.L28:
.L26:
	cmp %rcx, %r15
	jl .L27
.L9:
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %r11
	movq $-24, %rcx
	addq %rcx, %r11
	movq (%r11), %r11
	movq $8, %rcx
	movq %r15, %r10
	imulq %rcx, %r10
	addq %r10, %r11
	movq %rax, (%r11)
	movq $1, %rax
	movq %r15, %rdx
	subq %rax, %rdx
	movq $-8, %r11
	movq %rbp, %rax
	addq %r11, %rax
	movq (%rax), %rdi
	call L3
	movq %rbx, %rdx
	movq $1, %r11
	movq %r15, %rsi
	addq %r11, %rsi
	movq $-8, %r15
	movq %rbp, %rax
	addq %r15, %rax
	movq (%rax), %rdi
	call L3
	jmp .L29
.L27:
.L16:
	cmp %rcx, %r15
	jl .L10
.L11:
	movq $0, %r11
.L12:
	movq $0, %rdx
	cmp %rdx, %r11
	jne .L17
.L15:
	movq $-8, %rdx
	movq %rbp, %r11
	addq %rdx, %r11
	movq (%r11), %r11
	movq $-24, %r10
	addq %r10, %r11
	movq (%r11), %r11
	movq $8, %r9
	movq %rcx, %r10
	imulq %r9, %r10
	addq %r10, %r11
	movq (%r11), %r10
	movq $-8, %r9
	movq %rbp, %r11
	addq %r9, %r11
	movq (%r11), %r11
	movq $-24, %rdx
	addq %rdx, %r11
	movq (%r11), %r9
	movq $8, %rdx
	movq %r15, %r11
	imulq %rdx, %r11
	addq %r11, %r9
	movq %r10, (%r9)
.L24:
	cmp %rcx, %r15
	jl .L18
.L19:
	movq $0, %r11
.L20:
	movq $0, %rdx
	cmp %rdx, %r11
	jne .L25
.L23:
	movq $-8, %rdx
	movq %rbp, %r11
	addq %rdx, %r11
	movq (%r11), %r11
	movq $-24, %r10
	addq %r10, %r11
	movq (%r11), %rdx
	movq $8, %r10
	movq %r15, %r11
	imulq %r10, %r11
	addq %r11, %rdx
	movq (%rdx), %r10
	movq $-8, %r9
	movq %rbp, %r11
	addq %r9, %r11
	movq (%r11), %r11
	movq $-24, %rdx
	addq %rdx, %r11
	movq (%r11), %r9
	movq $8, %rdx
	movq %rcx, %r11
	imulq %rdx, %r11
	addq %r11, %r9
	movq %r10, (%r9)
	jmp .L26
.L10:
	movq $1, %r11
	movq $-8, %r9
	movq %rbp, %r10
	addq %r9, %r10
	movq (%r10), %r10
	movq $-24, %rdx
	addq %rdx, %r10
	movq (%r10), %r9
	movq $8, %rdx
	movq %rcx, %r10
	imulq %rdx, %r10
	addq %r10, %r9
	movq (%r9), %rdx
	cmp %rdx, %rax
	jle .L13
.L14:
	movq $0, %r11
.L13:
	jmp .L12
.L17:
	movq $1, %rdx
	subq %rdx, %rcx
	jmp .L16
.L18:
	movq $1, %r11
	movq $-8, %r9
	movq %rbp, %r10
	addq %r9, %r10
	movq (%r10), %r10
	movq $-24, %rdx
	addq %rdx, %r10
	movq (%r10), %rdx
	movq $8, %rdi
	movq %r15, %r10
	imulq %rdi, %r10
	addq %r10, %rdx
	movq (%rdx), %rdx
	cmp %rdx, %rax
	jge .L21
.L22:
	movq $0, %r11
.L21:
	jmp .L20
.L25:
	movq $1, %rdx
	addq %rdx, %r15
	jmp .L24
.L37:


	leave
	ret

	.text
	.globl L2
	.type L2, @function
L2:
	pushq %rbp
	movq %rsp, %rbp
	subq $24, %rsp
.L40:
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
	movq $-8, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %r11
	movq $-16, %rax
	addq %rax, %r11
	movq (%r11), %r11
	subq %rbx, %r11
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
.L42:
	movq %rdi, -8(%rbp)
	leaq .L5(%rip), %rdi
	call print


	leave
	ret

.section .rodata
.L5:
.string "\000\000\000\000"
