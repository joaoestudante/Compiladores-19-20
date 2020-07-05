; DATA
segment	.data
; ALIGN
align	4
; LABEL i
i:
; SINT 3
	dd	3
; DATA
segment	.data
; ALIGN
align	4
; LABEL j
j:
; SDOUBLE 61.0000
	dq	61.0000
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L1
_L1:
; SSTRING (value omitted -- see below)
	db	"string1", 0
; TEXT
segment	.text
; ADDR _L1
	push	dword $_L1
; TRASH 4
	add	esp, 4
; DATA
segment	.data
; ALIGN
align	4
; LABEL k
k:
; SADDR _L1
	dd	_L1
; TEXT
segment	.text
; ALIGN
align	4
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
; ADDR i
	push	dword $i
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; INT 5
	push	dword 5
; DUP32
	push	dword [esp]
; ADDR i
	push	dword $i
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; ADDR i
	push	dword $i
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; ADDR j
	push	dword $j
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
; LABEL _L1_cdk_emitter_internal
_L1_cdk_emitter_internal:
; SDOUBLE 62.0000
	dq	62.0000
; TEXT
segment	.text
; ADDR _L1_cdk_emitter_internal
	push	dword $_L1_cdk_emitter_internal
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
; ADDR j
	push	dword $j
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; ADDR j
	push	dword $j
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; ADDR k
	push	dword $k
; LDINT
	pop	eax
	push	dword [eax]
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L2
_L2:
; SSTRING (value omitted -- see below)
	db	"string2", 0
; TEXT
segment	.text
; ADDR _L2
	push	dword $_L2
; DUP32
	push	dword [esp]
; ADDR k
	push	dword $k
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; ADDR k
	push	dword $k
; LDINT
	pop	eax
	push	dword [eax]
; CALL prints
	call	prints
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
; EXTERN printd
extern	printd
; EXTERN printi
extern	printi
; EXTERN prints
extern	prints
