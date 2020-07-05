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
; JZ _L1
	pop	eax
	cmp	eax, byte 0
	je	near _L1
; INT 5
	push	dword 5
; LOCAL 8
	lea	eax, [ebp+8]
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
; LOCAL 8
	lea	eax, [ebp+8]
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
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L2
_L2:
; SSTRING (value omitted -- see below)
	db	"string1", 0
; TEXT
segment	.text
; ADDR _L2
	push	dword $_L2
; LOCAL 8
	lea	eax, [ebp+8]
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
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; JMP _L3
	jmp	dword _L3
; LABEL _L1
_L1:
