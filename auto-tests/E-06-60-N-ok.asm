; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 4
	push	ebp
	mov	ebp, esp
	sub	esp, 4
; INT 1
	push	dword 1
; DUP32
	push	dword [esp]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; LABEL _L1
_L1:
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 3
	push	dword 3
; NEG
	neg	dword [esp]
; GE
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setge	cl
	mov	[esp], ecx
; JZ _L2
	pop	eax
	cmp	eax, byte 0
	je	near _L2
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; LABEL _L3
_L3:
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; SUB
	pop	eax
	sub	dword [esp], eax
; DUP32
	push	dword [esp]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; JMP _L1
	jmp	dword _L1
; LABEL _L2
_L2:
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
