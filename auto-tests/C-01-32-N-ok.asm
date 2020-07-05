; BSS
segment	.bss
; ALIGN
align	4
; LABEL x
x:
; SALLOC 4
	resb	4
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
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; SUB
	pop	eax
	sub	dword [esp], eax
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
