; TEXT
segment	.text
; ALIGN
align	4
; LABEL f
f:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; LOCAL 12
	lea	eax, [ebp+12]
	push	eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; DADD
	fld	qword [esp]
	add	esp, byte 8
	fld	qword [esp]
	faddp	st1
	fstp	qword [esp]
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
; INT 10
	push	dword 10
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; INT 3
	push	dword 3
; CALL f
	call	f
; TRASH 12
	add	esp, 12
; LDFVAL64
	sub	esp, byte 8
	fstp	qword [esp]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; INT 3
	push	dword 3
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; INT 2
	push	dword 2
; CALL f
	call	f
; TRASH 12
	add	esp, 12
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
