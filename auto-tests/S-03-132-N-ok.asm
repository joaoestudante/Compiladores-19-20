; DATA
segment	.data
; ALIGN
align	4
; LABEL a
a:
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
; ADDR a
	push	dword $a
; INT 4
	push	dword 4
; ADD
	pop	eax
	add	dword [esp], eax
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
; ADDR a
	push	dword $a
; INT 4
	push	dword 4
; ADD
	pop	eax
	add	dword [esp], eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; ADDR a
	push	dword $a
; INT 4
	push	dword 4
; ADD
	pop	eax
	add	dword [esp], eax
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; ADDR a
	push	dword $a
; INT 12
	push	dword 12
; ADD
	pop	eax
	add	dword [esp], eax
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
; ADDR a
	push	dword $a
; INT 12
	push	dword 12
; ADD
	pop	eax
	add	dword [esp], eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; ADDR a
	push	dword $a
; INT 12
	push	dword 12
; ADD
	pop	eax
	add	dword [esp], eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; ADDR a
	push	dword $a
; INT 16
	push	dword 16
; ADD
	pop	eax
	add	dword [esp], eax
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
; LABEL _L1
_L1:
; SSTRING (value omitted -- see below)
	db	"string2", 0
; TEXT
segment	.text
; ADDR _L1
	push	dword $_L1
; DUP32
	push	dword [esp]
; ADDR a
	push	dword $a
; INT 16
	push	dword 16
; ADD
	pop	eax
	add	dword [esp], eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; ADDR a
	push	dword $a
; INT 16
	push	dword 16
; ADD
	pop	eax
	add	dword [esp], eax
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
