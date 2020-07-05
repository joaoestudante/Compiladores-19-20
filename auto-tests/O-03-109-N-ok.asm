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
; INT 9
	push	dword 9
; I2D
	fild	dword [esp]
	sub	esp, byte 4
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
; CALL f
	call	f
; TRASH 8
	add	esp, 8
; LDFVAL64
	sub	esp, byte 8
	fstp	qword [esp]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; INT 2
	push	dword 2
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; CALL f
	call	f
; TRASH 8
	add	esp, 8
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
