; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 12
	push	ebp
	mov	ebp, esp
	sub	esp, 12
; INT 61
	push	dword 61
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; INT 0
	push	dword 0
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DUP32
	push	dword [esp]
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 0
	push	dword 0
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
; CALL println
	call	println
; TRASH 8
	add	esp, 8
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
; EXTERN println
extern	println
