; TEXT
segment	.text
; ALIGN
align	4
; LABEL pi
pi:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; INT 314
	push	dword 314
; STFVAL32
	pop	eax
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
; CALL pi
	call	pi
; LDFVAL32
	push	eax
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; INT 0
	push	dword 0
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN printi
extern	printi