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
; INT 5
	push	dword 5
; DUP32
	push	dword [esp]
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; INT 1
	push	dword 1
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; ADD
	pop	eax
	add	dword [esp], eax
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
; INT 3
	push	dword 3
; CALL f
	call	f
; TRASH 4
	add	esp, 4
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
