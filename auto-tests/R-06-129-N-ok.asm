; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 8
	push	ebp
	mov	ebp, esp
	sub	esp, 8
; DOUBLE 61.0000
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L1_cdk_emitter_internal
_L1_cdk_emitter_internal:
; SDOUBLE 61.0000
	dq	61.0000
; TEXT
segment	.text
; ADDR _L1_cdk_emitter_internal
	push	dword $_L1_cdk_emitter_internal
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; DOUBLE 62.0000
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L2_cdk_emitter_internal
_L2_cdk_emitter_internal:
; SDOUBLE 62.0000
	dq	62.0000
; TEXT
segment	.text
; ADDR _L2_cdk_emitter_internal
	push	dword $_L2_cdk_emitter_internal
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; DUP64
; SP
	push	esp
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
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
