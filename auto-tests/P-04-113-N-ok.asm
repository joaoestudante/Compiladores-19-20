; BSS
segment	.bss
; ALIGN
align	4
; LABEL f
f:
; SALLOC 4
	resb	4
; DATA
segment	.data
; ALIGN
align	4
; LABEL d
d:
; SDOUBLE 4.00000
	dq	4.00000
; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 8
	push	ebp
	mov	ebp, esp
	sub	esp, 8
; INT 7
	push	dword 7
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ALLOC
	pop	eax
	sub	esp, eax
; SP
	push	esp
; DUP32
	push	dword [esp]
; ADDR f
	push	dword $f
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; INT 0
	push	dword 0
; DUP32
	push	dword [esp]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; LABEL _L1
_L1:
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 6
	push	dword 6
; LT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
; JZ _L2
	pop	eax
	cmp	eax, byte 0
	je	near _L2
; ADDR d
	push	dword $d
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; DUP64
; SP
	push	esp
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; ADDR f
	push	dword $f
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; LABEL _L3
_L3:
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; DUP32
	push	dword [esp]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; JMP _L1
	jmp	dword _L1
; LABEL _L2
_L2:
; ADDR d
	push	dword $d
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; ADDR d
	push	dword $d
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; DMUL
	fld	qword [esp]
	add	esp, byte 8
	fld	qword [esp]
	fmulp	st1
	fstp	qword [esp]
; DUP64
; SP
	push	esp
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; ADDR f
	push	dword $f
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; INT 0
	push	dword 0
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; LABEL _L4
_L4:
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 7
	push	dword 7
; LT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
; JZ _L5
	pop	eax
	cmp	eax, byte 0
	je	near _L5
; ADDR f
	push	dword $f
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; LABEL _L6
_L6:
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; DUP32
	push	dword [esp]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; JMP _L4
	jmp	dword _L4
; LABEL _L5
_L5:
; INT 0
	push	dword 0
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN printd
extern	printd
