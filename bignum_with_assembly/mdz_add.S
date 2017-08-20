.text

.globl mdz_add



// rdi rsi rdx
//  c   a   b
mdz_add:
	pushq %rbx
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	mov (8+4)(%rsi), %eax
	mov (8+4)(%rdx), %ebx

	cmp %eax, %ebx
	jnae   noswap      ;//??????
	movq %rdx, %r15
	movq %rsi, %rdx
	movq %r15, %rsi
noswap:

	movq (%rdi), %r12   ;// c->n
	movq (%rsi), %r10   ;// a->n
	movq (%rdx), %r11   ;// b->n

	clc                     ;//subq %r15, %r15     ;// This sets CF to 0.
	mov  (8+4)(%rdx), %ecx   ;// ecx = b->len
myloop:
	movq (%r10), %r14  ;// r14 = a[b->len-ecx]
	movq (%r11), %r15  ;// r15 = b[b->len-ecx]
	adcq %r14, %r15
	movq $0, %rbx       ;// to store the carry bit
	adcq $0, %rbx
	movq %r15, (%r12)
	addq $8, %r10
	addq $8, %r11
	addq $8, %r12
	dec  %ecx
	cmp  $0, %ecx
	jne  myloop

	mov  (8+4)(%rsi), %ecx   ;// ecx = a->len
	sub  (8+4)(%rdx), %ecx   ;// ecx = rcx - b->len
myloop2:
	movq (%r10), %r14  ;// r14 = a[b->len-rcx]
	adcq $0, %r14
	movq $0, %rbx       ;// to store the carry bit
	adcq $0, %rbx
	movq %r14, (%r12)
	addq $8, %r10
	addq $8, %r12
	dec  %ecx
	cmp  $0, %ecx
	jne  myloop2

	movq %rbx, (%r12)   ;// c->n[b->len] = carry

	mov  (8+4)(%rsi), %ecx
	add  %ebx, %ecx
	mov %ecx, (8+4)(%rdi)    ;//c->len = b->len + carry

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	popq %rbx
ret



// rax rbx rcx rbp rsp r8 r9 r10 r11 r12 r13 r14 r15
//      *       *   -                 *   *   *   *


//typedef struct{
//	ui *n;
//	int sign;
//	int len;    //The number of used digits.
//	int size;   //The number of allocated digits.
//} MDZ_t[1], *MDZ;
