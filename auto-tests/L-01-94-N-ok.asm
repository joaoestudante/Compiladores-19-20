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
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 0
	push	dword 0
; NE
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setne	cl
	mov	[esp], ecx
; JZ _L1
	pop	eax
	cmp	eax, byte 0
	je	near _L1
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; SUB
	pop	eax
	sub	dword [esp], eax
; CALL f
	call	f
; TRASH 4
	add	esp, 4
; LDFVAL32
	push	eax
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
; LABEL _L1
_L1:
; INT 9
	push	dword 9
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
