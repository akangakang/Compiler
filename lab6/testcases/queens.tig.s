	.text
	.globl tigermain
	.type tigermain, @function
tigermain:
	pushq %rbp
	movq %rsp, %rbp
	subq $56, %rsp
.L34:
	movq %rbx, -56(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq $8, %r11
	movq %r11, -16(%rbp)
	movq $-24, %rax
	movq %rbp, %rbx
	addq %rax, %rbx
	movq $0, %rsi
	movq $-16, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rdi
	call initArray
	movq %rax, (%rbx)
	movq $-32, %rax
	movq %rbp, %rbx
	addq %rax, %rbx
	movq $0, %rsi
	movq $-16, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	call initArray
	movq %rax, (%rbx)
	movq $-40, %rax
	movq %rbp, %rbx
	addq %rax, %rbx
	movq $0, %rsi
	movq $-16, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	movq $-16, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %r11
	addq %r11, %rdi
	movq $1, %r11
	subq %r11, %rdi
	call initArray
	movq %rax, (%rbx)
	movq $-48, %rax
	movq %rbp, %rbx
	addq %rax, %rbx
	movq $0, %rsi
	movq $-16, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	movq $-16, %r10
	movq %rbp, %r11
	addq %r10, %r11
	movq (%r11), %r11
	addq %r11, %rdi
	movq $1, %r11
	subq %r11, %rdi
	call initArray
	movq %rax, (%rbx)
	movq $0, %rsi
	movq %rbp, %rdi
	call L2
	movq -56(%rbp), %rbx #spill


	leave
	ret

	.text
	.globl L2
	.type L2, @function
L2:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
.L36:
	movq %rbx, -32(%rbp) #spill
	movq %r12, -16(%rbp) #spill
	movq %r15, -24(%rbp) #spill
	movq %rdi, -8(%rbp)
	movq %rsi, %r12
	movq $-8, %r15
	movq %rbp, %rax
	addq %r15, %rax
	movq (%rax), %rax
	movq $-16, %rbx
	addq %rbx, %rax
	movq (%rax), %rax
	cmp %rax, %r12
	je .L30
.L31:
	movq $0, %rbx
	movq $-8, %rax
	movq %rbp, %r15
	addq %rax, %r15
	movq (%r15), %rax
	movq $-16, %r15
	addq %r15, %rax
	movq (%rax), %r15
	movq $1, %r11
	subq %r11, %r15
	cmp %r15, %rbx
	jg .L15
.L28:
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
	movq (%rax), %rax
	movq $0, %r11
	cmp %r11, %rax
	je .L16
.L17:
	movq $0, %r11
.L18:
	movq $0, %r10
	cmp %r10, %r11
	jne .L21
.L22:
	movq $0, %r11
.L23:
	movq $0, %r10
	cmp %r10, %r11
	jne .L26
.L27:
	cmp %r15, %rbx
	je .L15
.L29:
	movq $1, %r11
	addq %r11, %rbx
	jmp .L28
.L30:
	movq $-8, %rax
	movq %rbp, %r15
	addq %rax, %r15
	movq (%r15), %rdi
	call L1
.L32:
	movq -32(%rbp), %rbx #spill
	movq -16(%rbp), %r12 #spill
	movq -24(%rbp), %r15 #spill
	jmp .L35
.L16:
	movq $1, %r11
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rax
	movq $-40, %r10
	addq %r10, %rax
	movq (%rax), %rcx
	movq %rbx, %rax
	addq %r12, %rax
	movq $8, %rdx
	imulq %rdx, %rax
	addq %rax, %rcx
	movq (%rcx), %r10
	movq $0, %rax
	cmp %rax, %r10
	je .L19
.L20:
	movq $0, %r11
.L19:
	jmp .L18
.L21:
	movq $1, %r11
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rax
	movq $-48, %r10
	addq %r10, %rax
	movq (%rax), %r10
	movq $7, %r9
	movq %rbx, %rax
	addq %r9, %rax
	subq %r12, %rax
	movq $8, %rcx
	imulq %rcx, %rax
	addq %rax, %r10
	movq (%r10), %r10
	movq $0, %rax
	cmp %rax, %r10
	je .L24
.L25:
	movq $0, %r11
.L24:
	jmp .L23
.L26:
	movq $1, %r10
	movq $-8, %r11
	movq %rbp, %rax
	addq %r11, %rax
	movq (%rax), %rax
	movq $-24, %r11
	addq %r11, %rax
	movq (%rax), %r11
	movq $8, %r9
	movq %rbx, %rax
	imulq %r9, %rax
	addq %rax, %r11
	movq %r10, (%r11)
	movq $1, %r11
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rax
	movq $-40, %r10
	addq %r10, %rax
	movq (%rax), %rcx
	movq %rbx, %rax
	addq %r12, %rax
	movq $8, %rdx
	imulq %rdx, %rax
	addq %rax, %rcx
	movq %r11, (%rcx)
	movq $1, %rcx
	movq $-8, %rdx
	movq %rbp, %rax
	addq %rdx, %rax
	movq (%rax), %rax
	movq $-48, %r11
	addq %r11, %rax
	movq (%rax), %rdx
	movq $7, %r11
	movq %rbx, %rax
	addq %r11, %rax
	subq %r12, %rax
	movq $8, %rsi
	imulq %rsi, %rax
	addq %rax, %rdx
	movq %rcx, (%rdx)
	movq $-8, %r11
	movq %rbp, %rax
	addq %r11, %rax
	movq (%rax), %rax
	movq $-32, %rcx
	addq %rcx, %rax
	movq (%rax), %r11
	movq $8, %rcx
	movq %r12, %rax
	imulq %rcx, %rax
	addq %rax, %r11
	movq %rbx, (%r11)
	movq $1, %r11
	movq %r12, %rsi
	addq %r11, %rsi
	movq $-8, %rax
	movq %rbp, %r11
	addq %rax, %r11
	movq (%r11), %rdi
	call L2
	movq $0, %r10
	movq $-8, %r11
	movq %rbp, %rax
	addq %r11, %rax
	movq (%rax), %rax
	movq $-24, %r11
	addq %r11, %rax
	movq (%rax), %r11
	movq $8, %r9
	movq %rbx, %rax
	imulq %r9, %rax
	addq %rax, %r11
	movq %r10, (%r11)
	movq $0, %r11
	movq $-8, %rcx
	movq %rbp, %rax
	addq %rcx, %rax
	movq (%rax), %rax
	movq $-40, %r10
	addq %r10, %rax
	movq (%rax), %rcx
	movq %rbx, %rax
	addq %r12, %rax
	movq $8, %rdx
	imulq %rdx, %rax
	addq %rax, %rcx
	movq %r11, (%rcx)
	movq $0, %rcx
	movq $-8, %rdx
	movq %rbp, %rax
	addq %rdx, %rax
	movq (%rax), %rax
	movq $-48, %r11
	addq %r11, %rax
	movq (%rax), %rdx
	movq $7, %r11
	movq %rbx, %rax
	addq %r11, %rax
	subq %r12, %rax
	movq $8, %rsi
	imulq %rsi, %rax
	addq %rax, %rdx
	movq %rcx, (%rdx)
	jmp .L27
.L15:
	movq $0, %rax
	jmp .L32
.L35:


	leave
	ret

	.text
	.globl L1
	.type L1, @function
L1:
	pushq %rbp
	movq %rsp, %rbp
	subq $40, %rsp
.L38:
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
	jg .L3
.L13:
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
	jg .L4
.L10:
	movq $-8, %r11
	movq %rbp, %rax
	addq %r11, %rax
	movq (%rax), %rax
	movq $-32, %rcx
	addq %rcx, %rax
	movq (%rax), %r11
	movq $8, %rcx
	movq %r15, %rax
	imulq %rcx, %rax
	addq %rax, %r11
	movq (%r11), %rax
	cmp %rbx, %rax
	je .L7
.L8:
	leaq .L6(%rip), %rdi
.L9:
	call print
	cmp %r14, %rbx
	je .L4
.L11:
	movq $1, %r11
	addq %r11, %rbx
	jmp .L10
.L7:
	leaq .L5(%rip), %rdi
	jmp .L9
.L4:
	leaq .L12(%rip), %rdi
	call print
	cmp %r12, %r15
	je .L3
.L14:
	movq $1, %r14
	addq %r14, %r15
	jmp .L13
.L3:
	leaq .L12(%rip), %rdi
	call print
	movq -40(%rbp), %rbx #spill
	movq -24(%rbp), %r12 #spill
	movq -16(%rbp), %r14 #spill
	movq -32(%rbp), %r15 #spill


	leave
	ret

.section .rodata
.L12:
.string "\001\000\000\000\012"
.section .rodata
.L6:
.string "\002\000\000\000 ."
.section .rodata
.L5:
.string "\002\000\000\000 O"
