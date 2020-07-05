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
; INT 2
	push	dword 2
; DUP32
	push	dword [esp]
; ADDR x
	push	dword $x
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; INT 1
	push	dword 1
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; INT 3
	push	dword 3
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
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
