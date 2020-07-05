; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 20
	push	ebp
	mov	ebp, esp
	sub	esp, 20
; INT 1
	push	dword 1
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; INT 0
	push	dword 0
; ADD
	pop	eax
	add	dword [esp], eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; DOUBLE 3.00000
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L1_cdk_emitter_internal
_L1_cdk_emitter_internal:
; SDOUBLE 3.00000
	dq	3.00000
; TEXT
segment	.text
; ADDR _L1_cdk_emitter_internal
	push	dword $_L1_cdk_emitter_internal
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; INT 4
	push	dword 4
; ADD
	pop	eax
	add	dword [esp], eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; INT 12
	push	dword 12
; ADD
	pop	eax
	add	dword [esp], eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; INT 16
	push	dword 16
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
