; DATA
segment	.data
; ALIGN
align	4
; LABEL r
r:
; SDOUBLE 61.0000
	dq	61.0000
; TEXT
segment	.text
; ALIGN
align	4
; LABEL main
main:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; LOCAL 8
	lea	eax, [ebp+8]
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
; STFVAL64
	fld	qword [esp]
	add	esp, byte 8
; LEAVE
	leave
; RET
	ret
; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; ADDR r
	push	dword $r
; CALL main
	call	main
; TRASH 4
	add	esp, 4
; LDFVAL64
	sub	esp, byte 8
	fstp	qword [esp]
; CALL printd
	call	printd
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
